#ifndef MITAMA_MAYBE_HPP
#define MITAMA_MAYBE_HPP

#include <mitama/panic.hpp>
#include <mitama/result/factory/success.hpp>
#include <mitama/result/factory/failure.hpp>
#include <mitama/result/detail/fwd.hpp>
#include <mitama/result/detail/meta.hpp>
#include <mitama/result/traits/impl_traits.hpp>
#include <mitama/maybe/fwd/maybe_fwd.hpp>
#include <mitama/maybe/factory/just_nothing.hpp>

#include <boost/format.hpp>
#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/hana/functional/overload_linearly.hpp>

#include <functional>
#include <memory>
#include <optional>
#include <variant>
#include <type_traits>
#include <tuple>
#include <utility>
#include <string_view>
#include <cassert>

namespace mitama::mitamagic {
template <class, class=void> struct is_pointer_like: std::false_type {};

template <class PointerLike>
struct is_pointer_like<PointerLike,
    std::void_t<
        decltype(std::declval<PointerLike&>().operator->()),
        decltype(*std::declval<PointerLike&>()),
        decltype(bool(std::declval<PointerLike&>()))>>
    : std::true_type
{};

template <class, class=void>
struct element_type;

template <class T>
struct element_type<T, std::enable_if_t<std::disjunction_v<is_pointer_like<T>, std::is_pointer<T>>>> {
    using type = std::remove_reference_t<decltype(*std::declval<T>())>;
};
}

namespace mitama {

template <class>
struct is_maybe: std::false_type {};

template <class T>
struct is_maybe<maybe<T>>: std::true_type {};

template <class, class>
struct is_maybe_with: std::false_type {};

template <class T>
struct is_maybe_with<maybe<T>, T>: std::true_type {};

template <class> class maybe_transpose_injector {
public:
    void transpose() = delete;
};

template <mutability _, class T, class E>
class maybe_transpose_injector<maybe<basic_result<_, T, E>>>
{
public:
    basic_result<_, maybe<T>, E>
    transpose() const& {
        return static_cast<maybe<basic_result<_, T, E>>const*>(this)->is_nothing()
            ? basic_result<_, maybe<T>, E>{success_t{nothing}}
            : static_cast<maybe<basic_result<_, T, E>>const*>(this)->unwrap().is_ok()
                ? basic_result<_, maybe<T>, E>{in_place_ok, std::in_place, static_cast<maybe<basic_result<_, T, E>>const*>(this)->unwrap().unwrap()}
                : basic_result<_, maybe<T>, E>{in_place_err, static_cast<maybe<basic_result<_, T, E>>const*>(this)->unwrap().unwrap_err()};
    }
};

template <class T, class=void>
class maybe_unwrap_or_default_injector {
public:
    void unwrap_or_default() = delete;
};

template <class T>
class maybe_unwrap_or_default_injector<maybe<T>,
    std::enable_if_t<
        std::disjunction_v<
            std::is_default_constructible<T>,
            std::is_aggregate<T>>>>
{
public:
  T unwrap_or_default() const
  {
    if constexpr (std::is_aggregate_v<T>){
      return static_cast<maybe<T> const *>(this)->is_just()
        ? static_cast<maybe<T> const *>(this)->unwrap()
        : T{};
    }
    else {
      return static_cast<maybe<T> const *>(this)->is_just()
        ? static_cast<maybe<T> const *>(this)->unwrap()
        : T();
    }
  }
};

template <class, class=void> class maybe_flatten_injector {
public:
    void flatten() = delete;
};

template <class T>
class maybe_flatten_injector<maybe<T>,
    std::enable_if_t<is_maybe<std::decay_t<T>>::value> >
{
public:
    auto flatten() const& {
        return static_cast<maybe<T>const*>(this)->is_just()
            ? static_cast<maybe<T>const*>(this)->unwrap().as_ref().cloned()
            : nothing;
    }
};


template <class, class=void> class maybe_cloned_injector {
public:
    void cloned() = delete;
};

template <class T>
class maybe_cloned_injector<maybe<T>,
    std::enable_if_t<
        std::conjunction_v<
            std::is_lvalue_reference<T>,
            std::is_copy_constructible<std::remove_const_t<std::remove_reference_t<T>>>>>>
{
public:
    maybe<std::remove_reference_t<T>> cloned() const {
        auto decay_copy = [](auto&& some) -> std::remove_const_t<std::remove_reference_t<T>> { return std::forward<decltype(some)>(some); };
        return static_cast<maybe<T>const*>(this)->is_just()
            ? maybe<std::remove_reference_t<T>>{just(decay_copy(static_cast<maybe<T>const*>(this)->unwrap()))}
            : nothing;
    }
};

template <class, class=void> class maybe_replace_injector {
public:
    void cloned() = delete;
};

template <class T>
class maybe_replace_injector<maybe<T>, std::enable_if_t<std::is_copy_constructible_v<std::remove_const_t<std::remove_reference_t<T>>>>> {
public:
    template <class... Args>
    std::enable_if_t<
        std::is_constructible_v<T, Args&&...>,
    maybe<T>>
    replace(Args&&... args) & {
        auto old = static_cast<maybe<T>*>(this)->as_ref().cloned();
        static_cast<maybe<T>*>(this)->storage_.template emplace<just_t<T>>(std::in_place, std::forward<Args>(args)...);
        return old;
    }

    template <class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F, Args&&...>,
            std::is_constructible<T, std::invoke_result_t<F&&, Args&&...>>>,
    maybe<T>>
    replace_with(F&& f, Args&&... args) & {
        auto old = static_cast<maybe<T>*>(this)->as_ref().cloned();
        static_cast<maybe<T>*>(this)->storage_.template emplace<just_t<T>>(std::invoke(std::forward<F>(f), std::forward<Args>(args)...));
        return old;
    }
};

template <class T>
class maybe
    : public maybe_transpose_injector<maybe<T>>
    , public maybe_unwrap_or_default_injector<maybe<T>>
    , public maybe_flatten_injector<maybe<T>>
    , public maybe_cloned_injector<maybe<T>>
    , public maybe_replace_injector<maybe<T>>
{
    std::variant<nothing_t, just_t<T>> storage_;
    template<class, class> friend class maybe_replace_injector;
    template <class> friend class maybe;

  public:
    using value_type = std::remove_reference_t<T>;
    using reference_type = std::add_lvalue_reference_t<value_type>;

    ~maybe() = default;
    constexpr maybe(): storage_(std::in_place_type<nothing_t>) {}
    maybe(maybe const&) = default;
    maybe& operator=(maybe const&) = default;
    maybe(maybe&&) = default;
    maybe& operator=(maybe&&) = default;
    maybe(nothing_t): maybe() {}

    template <typename U,
        std::enable_if_t<
            std::conjunction_v<
                std::is_constructible<T, U&&>,
                std::is_convertible<std::decay_t<U>, T>>,
    bool> = false>
    maybe(U&& u) : storage_(std::in_place_type<just_t<T>>, std::in_place, std::forward<U>(u)) {}

    template <typename U,
        std::enable_if_t<
            std::conjunction_v<
                std::is_constructible<T, U&&>,
                std::negation<std::is_convertible<std::decay_t<U>, T>>>,
    bool> = false>
    explicit maybe(U&& u) : storage_(std::in_place_type<just_t<T>>, std::in_place, std::forward<U>(u)) {}

    template <class... Args,
        std::enable_if_t<
            std::is_constructible_v<T, Args&&...>,
        bool> = false>
    explicit maybe(std::in_place_t, Args&&... args)
        : storage_(std::in_place_type<just_t<T>>, std::in_place, std::forward<Args>(args)...) {}

    template <class U, class... Args,
        std::enable_if_t<
            std::is_constructible_v<T, std::initializer_list<U>, Args&&...>,
        bool> = false>
    explicit maybe(std::in_place_t, std::initializer_list<U> il, Args&&... args)
        : storage_(std::in_place_type<just_t<T>>, std::in_place, il, std::forward<Args>(args)...) {}

    template <class... Args,
        std::enable_if_t<
            std::is_constructible_v<T, Args...>,
        bool> = false>
    maybe(just_t<_just_detail::forward_mode<T>, Args...>&& fwd)
        : maybe()
    {
        std::apply([&](auto&&... args){ storage_.template emplace<just_t<T>>(std::in_place, std::forward<decltype(args)>(args)...); }, std::move(fwd)());
    }

    template <class... Args,
        std::enable_if_t<
            std::is_constructible_v<T, Args...>,
        bool> = false>
    maybe(just_t<_just_detail::forward_mode<>, Args...>&& fwd)
        : maybe()
    {
        std::apply([&](auto&&... args){ storage_.template emplace<just_t<T>>(std::in_place, std::forward<decltype(args)>(args)...); }, std::move(fwd)());
    }

    template <class U,
        std::enable_if_t<
            std::is_constructible_v<T, U const&>,
        bool> = false>
    maybe(just_t<U> const& j)
        : storage_(std::in_place_type<just_t<T>>, std::in_place, j.get()) {}

    template <class U,
        std::enable_if_t<
            std::is_constructible_v<T, U&&>,
        bool> = false>
    maybe(just_t<U>&& j)
        : storage_(std::in_place_type<just_t<T>>, std::in_place, static_cast<U&&>(j.get())) {}

    explicit operator bool() const {
        return is_just();
    }

    value_type* operator->() & {
        return &(std::get<just_t<T>>(storage_).get());
    }

    value_type const* operator->() const& {
        return &(std::get<just_t<T>>(storage_).get());
    }

    bool is_just() const {
        return std::holds_alternative<just_t<T>>(storage_);
    }

    bool is_nothing() const {
        return !is_just();
    }

    value_type& unwrap() & {
        if (is_nothing())
            PANIC("called `maybe::unwrap()` on a `nothing` value");
        return std::get<just_t<T>>(storage_).get();
    }

    std::add_const_t<std::remove_reference_t<T>>&
    unwrap() const& {
        if (is_nothing())
            PANIC("called `maybe::unwrap()` on a `nothing` value");
        return std::get<just_t<T>>(storage_).get();
    }

    value_type unwrap() && {
        if (is_nothing())
            PANIC("called `maybe::unwrap()` on a `nothing` value");
        return std::move(std::get<just_t<T>>(storage_).get());
    }

    auto as_ref() & {
        return is_just()
            ? maybe<T&>(std::in_place, unwrap())
            : nothing;
    }

    auto as_ref() const& {
        return is_just()
            ? maybe<const T&>(std::in_place, unwrap())
            : nothing;
    }

    template <class... Args>
    std::enable_if_t<
        std::is_constructible_v<T, Args&&...>,
    value_type&>
    get_or_emplace(Args&&... args) & {
        return is_just()
            ? unwrap()
            : (storage_.template emplace<just_t<T>>(std::in_place, std::forward<Args>(args)...), unwrap());
    }

    template <class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&, Args&&...>,
            std::is_constructible<T, std::invoke_result_t<F&&, Args&&...>>>,
    value_type&>
    get_or_emplace_with(F&& f, Args&&... args) & {
        return is_just()
            ? unwrap()
            : (storage_.template emplace<just_t<T>>(std::in_place, std::invoke(std::forward<F>(f), std::forward<Args>(args)...)), unwrap());
    }

    template <class U>
    std::enable_if_t<
        meta::has_type<std::common_type<T&, U&&>>::value,
    std::common_type_t<value_type&, U&&>>
    unwrap_or(U&& def) & {
        return is_just() ? unwrap() : std::forward<U>(def);
    }

    template <class U>
    std::enable_if_t<
        meta::has_type<std::common_type<T const&, U&&>>::value,
    std::common_type_t<value_type const&, U&&>>
    unwrap_or(U&& def) const& {
        return is_just() ? unwrap() : std::forward<U>(def);
    }

    template <class U>
    std::enable_if_t<
        meta::has_type<std::common_type<T&&, U&&>>::value,
    std::common_type_t<value_type&&, U&&>>
    unwrap_or(U&& def) && {
        return is_just() ? std::move(unwrap()) : std::forward<U>(def);
    }

    template <class F>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&>,
            meta::has_type<std::common_type<T&, std::invoke_result_t<F&&>>>>,
    std::common_type_t<value_type const&, std::invoke_result_t<F&&>>>
    unwrap_or_else(F&& f) & {
        return is_just() ? unwrap() : std::invoke(std::forward<F>(f));
    }

    template <class F>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&>,
            meta::has_type<std::common_type<T const&, std::invoke_result_t<F&&>>>>,
    std::common_type_t<value_type const&, std::invoke_result_t<F&&>>>
    unwrap_or_else(F&& f) const& {
        return is_just() ? unwrap() : std::invoke(std::forward<F>(f));
    }

    template <class F>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&>,
            meta::has_type<std::common_type<T&&, std::invoke_result_t<F&&>>>>,
    std::common_type_t<value_type const&, std::invoke_result_t<F&&>>>
    unwrap_or_else(F&& f) && {
        return is_just() ? std::move(unwrap()) : std::invoke(std::forward<F>(f));
    }

    template <class F, class... Args,
        std::enable_if_t<
            std::is_invocable_v<F&&, value_type&, Args&&...>, bool> = false>
    auto map(F&& f, Args&&... args) & {
        using result_type = std::invoke_result_t<F&&, value_type&, Args&&...>;
        return is_just()
            ? maybe<result_type>{just(std::invoke(std::forward<F>(f), unwrap(), std::forward<Args>(args)...))}
            : nothing;
    }

    template <class F, class... Args,
        std::enable_if_t<
            std::is_invocable_v<F&&, value_type const&, Args&&...>, bool> = false>
    auto map(F&& f, Args&&... args) const& {
        using result_type = std::invoke_result_t<F&&, value_type const&, Args&&...>;
        return is_just()
            ? maybe<result_type>{just(std::invoke(std::forward<F>(f), unwrap(), std::forward<Args>(args)...))}
            : nothing;
    }

    template <class F, class... Args,
        std::enable_if_t<
            std::is_invocable_v<F&&, value_type&&, Args&&...>, bool> = false>
    auto map(F&& f, Args&&... args) && {
        using result_type = std::invoke_result_t<F&&, value_type&&, Args&&...>;
        return is_just()
            ? maybe<result_type>{just(std::invoke(std::forward<F>(f), std::move(unwrap()), std::forward<Args>(args)...))}
            : nothing;
    }

    template <class U, class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&, value_type&, Args&&...>,
            meta::has_type<std::common_type<U&&, std::invoke_result_t<F&&, value_type&>>>>,
    std::common_type_t<U&&, std::invoke_result_t<F&&, value_type&, Args&&...>>>
    map_or(U&& def, F&& f, Args&&... args) & {
        return is_just()
            ? std::invoke(std::forward<F>(f), unwrap(), std::forward<Args>(args)...)
            : std::forward<U>(def);
    }

    template <class U, class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&, value_type const&, Args&&...>,
            meta::has_type<std::common_type<U&&, std::invoke_result_t<F&&, value_type const&, Args&&...>>>>,
    std::common_type_t<U&&, std::invoke_result_t<F&&, T const&, Args&&...>>>
    map_or(U&& def, F&& f, Args&&... args) const& {
        return is_just()
            ? std::invoke(std::forward<F>(f), unwrap(), std::forward<Args>(args)...)
            : std::forward<U>(def);
    }

    template <class U, class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&, T&&, Args&&...>,
            meta::has_type<std::common_type<U&&, std::invoke_result_t<F&&, value_type&&, Args&&...>>>>,
    std::common_type_t<U&&, std::invoke_result_t<F&&, value_type&&, Args&&...>>>
    map_or(U&& def, F&& f, Args&&... args) && {
        return is_just()
            ? std::invoke(std::forward<F>(f), std::move(unwrap()), std::forward<Args>(args)...)
            : std::forward<U>(def);
    }

    template <class D, class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<D&&>,
            std::is_invocable<F&&, T&, Args&&...>,
            meta::has_type<std::common_type<std::invoke_result_t<D&&>, std::invoke_result_t<F&&, value_type&, Args&&...>>>>,
    std::common_type_t<std::invoke_result_t<D&&>, std::invoke_result_t<F&&, value_type&, Args&&...>>>
    map_or_else(D&& def, F&& f, Args&&... args) & {
        return is_just()
            ? std::invoke(std::forward<F>(f), unwrap(), std::forward<Args>(args)...)
            : std::invoke(std::forward<D>(def));
    }

    template <class D, class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<D&&>,
            std::is_invocable<F&&, value_type const&, Args&&...>,
            meta::has_type<std::common_type<std::invoke_result_t<D&&>, std::invoke_result_t<F&&, value_type const&, Args&&...>>>>,
    std::common_type_t<std::invoke_result_t<D&&>, std::invoke_result_t<F&&, value_type const&, Args&&...>>>
    map_or_else(D&& def, F&& f, Args&&... args) const& {
        return is_just()
            ? std::invoke(std::forward<F>(f), unwrap(), std::forward<Args>(args)...)
            : std::invoke(std::forward<D>(def));
    }

    template <class D, class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<D&&>,
            std::is_invocable<F&&, T&&, Args&&...>,
            meta::has_type<std::common_type<std::invoke_result_t<D&&>, std::invoke_result_t<F&&, value_type&&, Args&&...>>>>,
    std::common_type_t<std::invoke_result_t<D&&>, std::invoke_result_t<F&&, value_type&&, Args&&...>>>
    map_or_else(D&& def, F&& f, Args&&... args) && {
        return is_just()
            ? std::invoke(std::forward<F>(f), std::move(unwrap()), std::forward<Args>(args)...)
            : std::invoke(std::forward<D>(def));
    }

    value_type& expect(std::string_view msg) & {
        if (is_just()) {
            return unwrap();
        }
        else {
            PANIC("%1%", msg);
        }
    }

    value_type const& expect(std::string_view msg) const& {
        if (is_just()) {
            return unwrap();
        }
        else {
            PANIC("%1%", msg);
        }
    }

    value_type&& expect(std::string_view msg) && {
        if (is_just()) {
            return std::move(unwrap());
        }
        else {
            PANIC("%1%", msg);
        }
    }

    template <class Pred>
    std::enable_if_t<
        std::is_invocable_r_v<bool, Pred&&, T&>,
    maybe>
    filter(Pred&& predicate) & {
        return is_just() && std::invoke(std::forward<Pred>(predicate), unwrap())
            ? maybe<T>(unwrap())
            : nothing;
    }

    template <class Pred>
    std::enable_if_t<
        std::is_invocable_r_v<bool, Pred&&, T const&>,
    maybe>
    filter(Pred&& predicate) const& {
        return is_just() && std::invoke(std::forward<Pred>(predicate), unwrap())
            ? maybe<T>(unwrap())
            : nothing;
    }

    template <class Pred>
    std::enable_if_t<
        std::is_invocable_r_v<bool, Pred&&, T&&>,
    maybe>
    filter(Pred&& predicate) && {
        return is_just() && std::invoke(std::forward<Pred>(predicate), unwrap())
            ? maybe<T>(std::move(unwrap()))
            : nothing;
    }

    template <class E>
    auto ok_or(E&& err) const& {
        return is_just()
            ? result<T, E>{success_t{unwrap()}}
            : result<T, E>{failure_t{std::forward<E>(err)}};
    }

    template <class E>
    auto ok_or(E&& err) && {
        return is_just()
            ? result<T, E>{success_t{std::move(unwrap())}}
            : result<T, E>{failure_t{std::forward<E>(err)}};
    }

    auto ok_or() const& {
        return is_just()
            ? result<T>{success_t{unwrap()}}
            : result<T>{failure_t<>{}};
    }

    auto ok_or() && {
        return is_just()
            ? result<T>{success_t{std::move(unwrap())}}
            : result<T>{failure_t<>{}};
    }

    template <class F>
    std::enable_if_t<
        std::is_invocable_v<F&&>,
    result<T, std::invoke_result_t<F&&>>>
    ok_or_else(F&& err) const& {
        return is_just()
            ? result<T, std::invoke_result_t<F&&>>{success_t{unwrap()}}
            : result<T, std::invoke_result_t<F&&>>{failure_t{std::invoke(std::forward<F>(err))}};
    }

    template <class F>
    std::enable_if_t<
        std::is_invocable_v<F&&>,
    result<T, std::invoke_result_t<F&&>>>
    ok_or_else(F&& err) && {
        return is_just()
            ? result<T, std::invoke_result_t<F&&>>{success_t{std::move(unwrap())}}
            : result<T, std::invoke_result_t<F&&>>{failure_t{std::invoke(std::forward<F>(err))}};
    }

    template <class U>
    maybe<U> conj(maybe<U> const& rhs) const {
        return is_just() ? rhs : nothing;
    }

    template <class U>
    maybe<U> operator&&(maybe<U> const& rhs) const {
        return this->conj(rhs);
    }

    maybe<T> disj(maybe<T> const& rhs) const {
        return is_nothing() ? rhs : *this;
    }

    maybe<T> operator||(maybe<T> const& rhs) const {
        return this->disj(rhs);
    }

    maybe<T> xdisj(maybe<T> const& rhs) const {
        return is_just() ^ rhs.is_just()
            ? is_just() ? *this
                        : rhs
                        : nothing;
    }

    maybe<T> operator^(maybe<T> const& rhs) const {
        return this->xdisj(rhs);
    }

    template <class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&, T&, Args&&...>,
            is_maybe<std::decay_t<std::invoke_result_t<F&&, T&, Args&&...>>>>,
    std::invoke_result_t<F&&, T&, Args&&...>>
    and_then(F&& f, Args&&... args) & {
        return is_just()
            ? std::invoke(std::forward<F>(f), unwrap(), std::forward<Args>(args)...)
            : nothing;
    }

    template <class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&, T const&, Args&&...>,
            is_maybe<std::decay_t<std::invoke_result_t<F&&, T const&, Args&&...>>>>,
    std::invoke_result_t<F&&, T const&, Args&&...>>
    and_then(F&& f, Args&&... args) const& {
        return is_just()
            ? std::invoke(std::forward<F>(f), unwrap(), std::forward<Args>(args)...)
            : nothing;
    }

    template <class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&, T&&, Args&&...>,
            is_maybe<std::decay_t<std::invoke_result_t<F&&, T&&, Args&&...>>>>,
    std::invoke_result_t<F&&, T&, Args&&...>>
    and_then(F&& f, Args&&... args) && {
        return is_just()
            ? std::invoke(std::forward<F>(f), std::move(unwrap()), std::forward<Args>(args)...)
            : nothing;
    }

    template <class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&, Args&&...>,
            is_maybe_with<std::decay_t<std::invoke_result_t<F&&, Args&&...>>, T>>,
    maybe>
    or_else(F&& f, Args&&... args) & {
        return is_just()
            ? just(unwrap())
            : std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    }

    template <class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&, Args&&...>,
            is_maybe_with<std::decay_t<std::invoke_result_t<F&&, Args&&...>>, T>>,
    maybe>
    or_else(F&& f, Args&&... args) const& {
        return is_just()
            ? just(unwrap())
            : std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    }

    template <class F, class... Args>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&, Args&&...>,
            is_maybe_with<std::decay_t<std::invoke_result_t<F&&, Args&&...>>, T>>,
    maybe>
    or_else(F&& f, Args&&... args) && {
        return is_just()
            ? just(std::move(unwrap()))
            : std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    }

    template <class F, class... Args>
    std::enable_if_t<std::is_invocable_v<F&&, value_type&, Args&&...>>
    and_finally(F&& f, Args&&... args) & {
        if (is_just())
            std::invoke(std::forward<F>(f), unwrap(), std::forward<Args>(args)...);
    }

    template <class F, class... Args>
    std::enable_if_t<std::is_invocable_v<F&&, value_type const&>>
    and_finally(F&& f, Args&&... args) const& {
        if (is_just())
            std::invoke(std::forward<F>(f), unwrap(), std::forward<Args>(args)...);
    }

    template <class F, class... Args>
    void and_finally(F&& f, Args&&... args) && {
        if constexpr (std::is_lvalue_reference_v<T>) {
            static_assert(std::is_invocable_v<F&&, value_type&, Args&&...>);
            if (is_just())
                std::invoke(std::forward<F>(f), unwrap(), std::forward<Args>(args)...);
        }
        else {
            static_assert(std::is_invocable_v<F&&, value_type&&>);
            if (is_just())
                std::invoke(std::forward<F>(f), std::move(unwrap()));
        }
    }

    template <class F, class... Args>
    std::enable_if_t<std::is_invocable_v<F&&, Args&&...>>
    or_finally(F&& f, Args&&... args) & {
        if (is_nothing())
            std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    }

    template <class F, class... Args>
    std::enable_if_t<std::is_invocable_v<F&&, Args&&...>>
    or_finally(F&& f, Args&&... args) const& {
        if (is_nothing())
            std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    }

    template <class F, class... Args>
    std::enable_if_t<std::is_invocable_v<F&&, Args&&...>>
    or_finally(F&& f, Args&&... args) && {
        if (is_nothing())
            std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    }

    template <class F>
    constexpr
    std::enable_if_t<
        std::disjunction_v<
            std::is_invocable<F, value_type&>,
            std::is_invocable<F>>,
    maybe&>
    and_peek(F&& f) &
    {
        if (is_just()) {
            if constexpr (std::is_invocable_v<F, value_type&>) {
                std::invoke(std::forward<F>(f), unwrap());
            }
            else {
                std::invoke(std::forward<F>(f));
            }
        }
        return *this;
    }

    template <class F>
    std::enable_if_t<
        std::disjunction_v<
            std::is_invocable<F&&, value_type const&>,
            std::is_invocable<F&&>>,
    maybe const&>
    and_peek(F&& f) const&
    {
        if (is_just()) {
            if constexpr (std::is_invocable_v<F, value_type&>) {
                std::invoke(std::forward<F>(f), unwrap());
            }
            else {
                std::invoke(std::forward<F>(f));
            }
        }
        return *this;
    }

    template <class F>
    constexpr
    std::enable_if_t<
        std::is_invocable_v<F&&>,
    maybe&>
    or_peek(F&& f) &
    {
        if (is_nothing()) std::invoke(std::forward<F>(f));
        return *this;
    }

    template <class F>
    constexpr
    std::enable_if_t<
        std::is_invocable_v<F&&>,
    maybe const&>
    or_peek(F&& f) const &
    {
        if (is_nothing()) std::invoke(std::forward<F>(f));
        return *this;
    }

};

template <class T>
maybe(T&&) -> maybe<T>;

template <class T, class U>
std::enable_if_t<meta::is_comparable_with<T, U>::value,
bool>
operator==(maybe<T> const& lhs, maybe<U> const& rhs) {
    return lhs.is_just() && rhs.is_just()
        ? (lhs.unwrap() == rhs.unwrap())
        : lhs.is_nothing() && rhs.is_nothing();
}

template <class T>
bool operator==(maybe<T> const& lhs, const nothing_t) {
    return lhs.is_nothing();
}

template <class T>
bool operator==(const nothing_t, maybe<T> const& rhs) {
    return rhs.is_nothing();
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<U>>>,
        meta::is_comparable_with<T, U>>,
bool>
operator==(maybe<T> const& lhs, U&& rhs) {
    return lhs == just(std::forward<U>(rhs));
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<T>>>,
        meta::is_comparable_with<T, U>>,
bool>
operator==(T&& lhs, maybe<U> const& rhs) {
    return just(std::forward<T>(lhs)) == rhs;
}


template <class T, class U>
std::enable_if_t<meta::is_comparable_with<T, U>::value,
bool>
operator!=(maybe<T> const& lhs, maybe<U> const& rhs) {
    return !(lhs == rhs);
}

template <class T>
bool operator!=(maybe<T> const& lhs, const nothing_t) {
    return lhs.is_just();
}

template <class T>
bool operator!=(const nothing_t, maybe<T> const& rhs) {
    return rhs.is_just();
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<U>>>,
        meta::is_comparable_with<T, U>>,
bool>
operator!=(maybe<T> const& lhs, U&& rhs) {
    return lhs != just(std::forward<U>(rhs));
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<T>>>,
        meta::is_comparable_with<T, U>>,
bool>
operator!=(T&& lhs, maybe<U> const& rhs) {
    return just(std::forward<T>(lhs)) != rhs;
}


template <class T, class U>
bool operator<(maybe<T> const& lhs, maybe<U> const& rhs) {
    return lhs.ok_or() < rhs.ok_or();
}

template <class T>
bool operator<(nothing_t, maybe<T> const& rhs) {
    return rhs.is_just();
}

template <class T>
bool operator<(maybe<T> const&, nothing_t) {
    return false;
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<U>>>,
        meta::is_less_comparable_with<T, U>>,
bool>
operator<(maybe<T> const& lhs, U&& rhs) {
    return lhs < just(std::forward<U>(rhs));
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<T>>>,
        meta::is_less_comparable_with<T, U>>,
bool>
operator<(T&& lhs, maybe<U> const& rhs) {
    return just(std::forward<T>(lhs)) < rhs;
}


template <class T, class U>
bool operator<=(maybe<T> const& lhs, maybe<U> const& rhs) {
    return lhs.ok_or() <= rhs.ok_or();
}

template <class T>
bool operator<=(nothing_t, maybe<T> const&) {
    return true;
}

template <class T>
bool operator<=(maybe<T> const& lhs, nothing_t) {
    return lhs.is_nothing();
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<U>>>,
        meta::is_less_comparable_with<T, U>,
        meta::is_comparable_with<T, U>>,
bool>
operator<=(maybe<T> const& lhs, U&& rhs) {
    return lhs <= just(std::forward<U>(rhs));
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<T>>>,
        meta::is_less_comparable_with<T, U>,
        meta::is_comparable_with<T, U>>,
bool>
operator<=(T&& lhs, maybe<U> const& rhs) {
    return just(std::forward<T>(lhs)) <= rhs;
}


template <class T, class U>
bool operator>(maybe<T> const& lhs, maybe<U> const& rhs) {
    return lhs.ok_or() > rhs.ok_or();
}

template <class T>
bool operator>(nothing_t, maybe<T> const&) {
    return false;
}

template <class T>
bool operator>(maybe<T> const& lhs, nothing_t) {
    return lhs.is_just();
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<U>>>,
        meta::is_less_comparable_with<U, T>>,
bool>
operator>(maybe<T> const& lhs, U&& rhs) {
    return lhs > just(std::forward<U>(rhs));
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<T>>>,
        meta::is_less_comparable_with<U, T>>,
bool>
operator>(T&& lhs, maybe<U> const& rhs) {
    return just(std::forward<T>(lhs)) > rhs;
}


template <class T, class U>
bool operator>=(maybe<T> const& lhs, maybe<U> const& rhs) {
    return lhs.ok_or() >= rhs.ok_or();
}

template <class T>
bool operator>=(nothing_t, maybe<T> const& rhs) {
    return rhs.is_nothing();
}

template <class T>
bool operator>=(maybe<T> const&, nothing_t) {
    return true;
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<U>>>,
        meta::is_less_comparable_with<U, T>,
        meta::is_comparable_with<U, T>>,
bool>
operator>=(maybe<T> const& lhs, U&& rhs) {
    return lhs >= just(std::forward<U>(rhs));
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<T>>>,
        meta::is_less_comparable_with<U, T>,
        meta::is_comparable_with<U, T>>,
bool>
operator>=(T&& lhs, maybe<U> const& rhs) {
    return just(std::forward<T>(lhs)) >= rhs;
}


/// @brief
///   ostream output operator for maybe<T>
///
/// @requires
///   Format<T>;
///
/// @note
///   Output its contained value with pretty format, and is used by `operator<<` found by ADL.
template <class T>
std::enable_if_t<trait::formattable<T>::value,
std::ostream&>
operator<<(std::ostream& os, maybe<T> const& may) {
  using namespace std::string_literals;
  using namespace std::string_view_literals;
  auto inner_format = boost::hana::fix(boost::hana::overload_linearly(
      [](auto, auto const& x) -> std::enable_if_t<trait::formattable_element<std::decay_t<decltype(x)>>::value, std::string> {
        return boost::hana::overload_linearly(
          [](std::monostate) { return "()"s; },
          [](std::string_view x) { return (boost::format("\"%1%\"") % x).str(); },
          [](auto const& x) { return (boost::format("%1%") % x).str(); })
        (x);
      },
      [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_dictionary<std::decay_t<decltype(x)>>::value, std::string> {
        if (x.empty()) return "{}"s;
        using std::begin, std::end;
        auto iter = begin(x);
        std::string str = "{"s + (boost::format("%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
        while (++iter != end(x)) {
          str += (boost::format(",%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
        }
        return str += "}";
      },
      [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_range<std::decay_t<decltype(x)>>::value, std::string> {
        if (x.empty()) return "[]"s;
        using std::begin, std::end;
        auto iter = begin(x);
        std::string str = "["s + _fmt(*iter);
        while (++iter != end(x)) {
          str += (boost::format(",%1%") % _fmt(*iter)).str();
        }
        return str += "]";
      },
      [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_tuple<std::decay_t<decltype(x)>>::value, std::string> {
        if constexpr (std::tuple_size_v<std::decay_t<decltype(x)>> == 0) {
          return "()"s;
        }
        else {
          return std::apply(
            [_fmt](auto const& head, auto const&... tail) {
              return "("s + _fmt(head) + ((("," + _fmt(tail))) + ...) + ")"s;
            }, x);
        }
      }));
  return may.is_just() ? os << boost::format("just(%1%)") % inner_format( may.unwrap() )
                       : os << "nothing"sv;
}

}
#endif
