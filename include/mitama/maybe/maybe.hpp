#ifndef MITAMA_MAYBE_HPP
#define MITAMA_MAYBE_HPP

#include <mitama/panic.hpp>
#include <mitama/result/factory/success.hpp>
#include <mitama/result/factory/failure.hpp>
#include <mitama/result/detail/fwd.hpp>
#include <mitama/result/detail/meta.hpp>
#include <mitama/maybe/fwd/maybe_fwd.hpp>

#include <boost/format.hpp>
#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/hana/functional/overload_linearly.hpp>
#include <boost/optional.hpp>
#include <boost/utility/in_place_factory.hpp>

#include <functional>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>

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

namespace mitama::mitamagic {

template <class T>
class polymorphic_storage {
  public:
    using pointer_type = T*;
    using pointer_const_type = const T*;
    using reference_type = T&;

    virtual ~polymorphic_storage() = default;
    virtual T& deref() = 0;
    virtual bool is_just() const = 0;
    virtual pointer_type get_pointer() = 0;
    virtual pointer_const_type get_pointer() const  = 0;
};

template <class, class=void>
class maybe_view;

template <class T>
class maybe_view<T,
    std::enable_if_t<
        std::disjunction_v<
            mitamagic::is_pointer_like<std::remove_reference_t<T>>,
            std::is_pointer<std::remove_reference_t<T>>>>>
    : public polymorphic_storage<typename mitamagic::element_type<std::decay_t<T>>::type>
{
    T storage_;
  public:
    using reference_type = typename polymorphic_storage<typename mitamagic::element_type<std::decay_t<T>>::type>::reference_type;
    using pointer_type = typename polymorphic_storage<typename mitamagic::element_type<std::decay_t<T>>::type>::pointer_type;
    using pointer_const_type = typename polymorphic_storage<typename mitamagic::element_type<std::decay_t<T>>::type>::pointer_const_type;

    virtual ~maybe_view() = default;
    maybe_view() = delete;

    template <typename U, std::enable_if_t<std::is_convertible_v<U,T>, bool> = false>
    maybe_view(U&& u) : storage_{std::forward<U>(u)} {}

    template <class... Args>
    maybe_view(std::in_place_t, Args&&... args) : storage_{std::forward<Args>(args)...} {}

    template <class U, class... Args>
    maybe_view(std::in_place_type_t<boost::optional<U>>, Args&&... args) : storage_(boost::in_place(std::forward<Args>(args)...)) {}

    template <class U, class... Args>
    maybe_view(std::in_place_type_t<std::shared_ptr<U>>, Args&&... args) : storage_{std::make_shared<U>(std::forward<Args>(args)...)} {}

    template <class U, class... Args>
    maybe_view(std::in_place_type_t<std::unique_ptr<U>>, Args&&... args) : storage_{std::make_unique<U>(std::forward<Args>(args)...)} {}

    bool is_just() const override final {
        return bool(storage_);
    }
    reference_type deref() override final {
        return *storage_;
    }
    pointer_type get_pointer() override final {
        if constexpr (std::is_pointer_v<std::remove_reference_t<T>>) {
            return storage_;
        }
        else {
            return storage_.operator->();
        }
    }
    pointer_const_type get_pointer() const override final {
        if constexpr (std::is_pointer_v<std::remove_reference_t<T>>) {
            return storage_;
        }
        else {
            return storage_.operator->();
        }
    }
};

template <class T> maybe_view(T&&) -> maybe_view<T>;
template <class T, class... Args> maybe_view(std::in_place_type_t<T>, Args&&...) -> maybe_view<T>;

template <class T, class... Args>
inline auto default_maybe_view(Args&&... args) {
    return std::make_shared<maybe_view<boost::optional<T>>>(std::in_place_type<boost::optional<T>>, std::forward<Args>(args)...);
}

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

struct nothing_t {};

template <class T=void>
inline maybe<T> nothing{};

template <>
inline constexpr auto nothing<void> = nothing_t{};

namespace mitamagic {
template <class T, class=void>
struct just_factory {
    template <class... Args>
    static auto invoke(Args&&... args) {
        return maybe<std::remove_reference_t<T>>(std::in_place, std::forward<Args>(args)...);
    }
};

template <class T>
struct just_factory<T,
    std::enable_if_t<
        std::disjunction_v<
            mitamagic::is_pointer_like<std::remove_reference_t<T>>,
            std::is_pointer<std::remove_reference_t<T>>>>>
{
    template <class U>
    static auto invoke(U&& u) {
        return maybe(maybe(std::forward<U>(u)));
    }
};

template <class T>
struct just_factory<std::in_place_type_t<T>>
{
    template <class... Args>
    static auto invoke(Args&&... args) {
        return maybe<T>(std::in_place, std::forward<Args>(args)...);
    }
};
}

class in_place_factory_base {};

template <class... Args>
class in_place_factory: in_place_factory_base {
    std::tuple<Args...> pack;
public:
    constexpr explicit in_place_factory(Args... args): pack(std::forward<Args>(args)...) {}

    template <class T>
    decltype(auto) apply(std::in_place_type_t<T>) const {
        return std::apply([](auto&&... args){ return T(std::forward<decltype(args)>(args)...); }, pack);
    }

    template <class T>
    void* apply(std::in_place_type_t<T>, void* address) const {
        return std::apply([address](auto&&... args) -> void* { return new(address) T(std::forward<decltype(args)>(args)...); }, pack);
    }

    template <class F>
    decltype(auto) apply(F&& supplier) const {
        return std::apply(std::forward<F>(supplier), pack);
    }
};

template <class... Args>
constexpr auto in_place(Args&&... args) {
    return ::mitama::in_place_factory<Args&&...>{std::forward<Args>(args)...};
}

template <class Target=void, class... Ini>
inline auto just(Ini&&... ini) {
    if constexpr (!std::is_void_v<Target>) {
        return mitamagic::just_factory<std::in_place_type_t<Target>>::invoke(std::forward<Ini>(ini)...);
    }
    else if constexpr (sizeof...(Ini) == 1 && std::is_base_of_v<::mitama::in_place_factory_base, std::decay_t<std::tuple_element_t<0, std::tuple<Ini...>>>>) {
        return [](auto&& factory, auto&&...){
            return std::forward<decltype(factory)>(factory);
        }(std::forward<Ini>(ini)...);
    }
    else if constexpr (sizeof...(Ini) == 1) {
        return mitamagic::just_factory<std::tuple_element_t<0, std::tuple<Ini...>>>::invoke(std::forward<Ini>(ini)...);
    }
    else {
        static_assert([]{ return false; }(),
            "Error: Given variadic initializers without Target. Please try `just<Target>(args...)`.");
    }
}

template <class> class maybe_transpose_injector {
public:
    void transpose() = delete;
};

template <mutability _, class T, class E>
class maybe_transpose_injector<maybe<basic_result<_, T, E>>>
{
public:
    basic_result<_, maybe<std::remove_reference_t<T>>, E>
    transpose() const& {
        return
            static_cast<maybe<basic_result<_, T, E>>const*>(this)->is_nothing()
                ? basic_result<_, maybe<std::remove_reference_t<T>>, E>{success{nothing<>}}
                : static_cast<maybe<basic_result<_, T, E>>const*>(this)->unwrap().is_ok()
                    ? basic_result<_, maybe<std::remove_reference_t<T>>, E>{success{just(static_cast<maybe<basic_result<_, T, E>>const*>(this)->unwrap().unwrap())}}
                    : basic_result<_, maybe<std::remove_reference_t<T>>, E>{failure{static_cast<maybe<basic_result<_, T, E>>const*>(this)->unwrap().unwrap_err()}};
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

template <class> class maybe_flatten_injector {
public:
    void flatten() = delete;
};

template <class T>
class maybe_flatten_injector<maybe<maybe<T>>>
{
public:
    maybe<T>
    flatten() const& {
        return static_cast<maybe<maybe<T>>const*>(this)->is_just()
            && static_cast<maybe<maybe<T>>const*>(this)->unwrap().is_just()
                ? just(static_cast<maybe<maybe<T>>const*>(this)->unwrap().unwrap())
                : nothing<>;
    }
};

template <class, class=void> class maybe_copy_injector {
public:
    void copied() = delete;
    void replace() = delete;
};

template <class T>
class maybe_copy_injector<maybe<T>, std::enable_if_t<std::is_copy_constructible_v<T>>> {
public:
    maybe<T> copied() const& {
        return static_cast<maybe<T>const*>(this)->is_just()
            ? just<T>([](auto&& some) -> std::decay_t<decltype(some)> { return std::forward<decltype(some)>(some); }(static_cast<maybe<T>const*>(this)->unwrap()))
            : nothing<>;
    }

    maybe<T> replace(T value) & {
        auto old = static_cast<maybe<T>*>(this)->copied();
        if (!static_cast<maybe<T>*>(this)->storage_) {
            static_cast<maybe<T>*>(this)->storage_ = std::make_shared<mitamagic::maybe_view<boost::optional<T>>>(boost::optional<T>{value});
        }
        else {
            *(static_cast<maybe<T>*>(this)->storage_->get_pointer()) = value;
        }
        return old;
    }
};

template <class T>
class maybe
    : public maybe_transpose_injector<maybe<T>>
    , public maybe_unwrap_or_default_injector<maybe<T>>
    , public maybe_flatten_injector<maybe<T>>
    , public maybe_copy_injector<maybe<T>>
{
    std::shared_ptr<mitamagic::polymorphic_storage<T>> storage_;
    template<class, class>
    friend class maybe_copy_injector;
  public:
    using value_type = T;
    ~maybe() = default;
    maybe() = default;
    maybe(maybe const&) = default;
    maybe& operator=(maybe const&) = default;
    maybe(maybe&&) = default;
    maybe& operator=(maybe&&) = default;
    maybe(const nothing_t): maybe() {}

    template <typename U,
        std::enable_if_t<
            !std::is_base_of_v<mitama::in_place_factory_base, std::decay_t<U>> &&
            std::disjunction_v<
                mitamagic::is_pointer_like<std::remove_reference_t<U>>,
                std::is_pointer<std::remove_reference_t<U>>>, bool> = false>
    maybe(U&& u) : storage_(std::make_shared<mitamagic::maybe_view<U>>(std::forward<U>(u))) {}

    template <typename U,
        std::enable_if_t<
            std::conjunction_v<
                std::is_constructible<T, U&&>,
                std::negation<std::is_base_of<mitama::in_place_factory_base, std::decay_t<U>>>,
                std::disjunction<
                    mitamagic::is_pointer_like<std::remove_reference_t<U>>,
                    std::is_pointer<std::remove_reference_t<U>>>>,
        bool> = false>
    maybe(U&& u) : storage_(mitamagic::default_maybe_view<T>(std::forward<U>(u))) {}

    template <class... Args,
        std::enable_if_t<
            std::is_constructible_v<T, Args&&...>,
        bool> = false>
    explicit maybe(std::in_place_t, Args&&... args)
        : storage_(mitamagic::default_maybe_view<T>(std::forward<Args>(args)...)) {}

    template <class U, class... Args,
        std::enable_if_t<
            std::is_constructible_v<T, std::initializer_list<U>, Args&&...>,
        bool> = false>
    explicit maybe(std::in_place_t, std::initializer_list<U> il, Args&&... args)
        : storage_(mitamagic::default_maybe_view<T>(il, std::forward<Args>(args)...)) {}

    template <class InplaceFactory,
        std::enable_if_t<
            std::is_base_of_v<mitama::in_place_factory_base, std::decay_t<InplaceFactory>>,
        bool> = false>
    maybe(InplaceFactory&& factory)
        : storage_(std::forward<InplaceFactory>(factory).apply([](auto&&... args){
            return mitamagic::default_maybe_view<T>(std::forward<decltype(args)>(args)...);
        }))
    {}

    template <class F,
        std::enable_if_t<
            std::is_invocable_v<F&&, T&>,
        bool> = false>
    auto operator>>(F&& f) const& {
        static_assert(std::is_invocable_v<F&&, T&>,
            "specified functor is not invocable.");
        using result_type = std::remove_reference_t<std::invoke_result_t<F&&, T&>>;

        if constexpr (std::disjunction_v<
            std::is_constructible<std::invoke_result_t<F&&, T&>, decltype(nullptr)>,
            std::is_constructible<std::invoke_result_t<F&&, T&>, std::nullopt_t>,
            std::is_constructible<std::invoke_result_t<F&&, T&>, boost::none_t>>) {

            return this->is_just()
                ? maybe<typename mitamagic::element_type<std::decay_t<result_type>>::type>{std::invoke(std::forward<F>(f), storage_->deref())}
                : nothing<>;
        }
        else {
            return this->is_just()
                ? maybe<result_type>{boost::optional<result_type>(std::invoke(std::forward<F>(f), storage_->deref()))}
                : nothing<>;
        }
    }

    explicit operator bool() const {
        return this->is_just();
    }

    decltype(auto) operator->() & {
        return storage_->get_pointer();
    }

    decltype(auto) operator->() const& {
        return storage_->get_pointer();
    }

    bool is_just() const {
        return bool(storage_) && storage_->is_just();
    }

    bool is_nothing() const {
        return !this->is_just();
    }

    decltype(auto) unwrap() & {
        if (is_just())
            return storage_->deref();
        else
            PANIC("called `maybe::unwrap()` on a `nothing` value");
    }

    decltype(auto) unwrap() const& {
        if (is_just())
            return storage_->deref();
        else
            PANIC("called `maybe::unwrap()` on a `nothing` value");
    }

    T& get_or_insert(T const& v) & {
        if (!storage_) {
            storage_ = std::make_shared<mitamagic::maybe_view<boost::optional<T>>>(boost::optional<T>{v});
            return storage_->deref();
        }
        else {
            return this->is_just()
                ? storage_->deref()
                : (*storage_->get_pointer() = v);
        }
    }

    template <class F>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&>,
            std::is_assignable<T&, std::invoke_result_t<F&&>>>,
    T&>
    get_or_insert_with(F&& f) & {
        if (!storage_) {
            storage_ = std::make_shared<mitamagic::maybe_view<boost::optional<T>>>(boost::optional<T>{std::invoke(std::forward<F>(f))});
            return storage_->deref();
        }
        else {
            return this->is_just()
                ? storage_->deref()
                : (*storage_->get_pointer() = std::invoke(std::forward<F>(f)));
        }
    }

    T unwrap_or(T const& def) const {
        return is_just() ? storage_->deref() : def;
    }

    template <class F>
    std::enable_if_t<
        std::is_invocable_r_v<T, F&&>,
    T>
    unwrap_or_else(F&& f) const {
        return is_just() ? storage_->deref() : std::invoke(std::forward<F>(f));
    }

    template <class F,
        std::enable_if_t<
            std::is_invocable_v<F&&, T&>, bool> = false>
    auto map(F&& f) const {
        return *this >> std::forward<F>(f);
    }

    template <class U, class F>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&, T&>,
            meta::has_type<std::common_type<U&&, std::invoke_result_t<F&&, T&>>>>,
    std::common_type_t<U&&, std::invoke_result_t<F&&, T&>>>
    map_or(U&& def, F&& f) const {
        return is_just()
            ? std::invoke(std::forward<F>(f), storage_->deref())
            : std::forward<U>(def);
    }

    template <class D, class F>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<D&&>,
            std::is_invocable<F&&, T&>,
            meta::has_type<std::common_type<std::invoke_result_t<D&&>, std::invoke_result_t<F&&, T&>>>>,
    std::common_type_t<std::invoke_result_t<D&&>, std::invoke_result_t<F&&, T&>>>
    map_or_else(D&& def, F&& f) const {
        return is_just()
            ? std::invoke(std::forward<F>(f), storage_->deref())
            : std::invoke(std::forward<D>(def));
    }

    T& expect(std::string_view msg) {
        if (this->is_just()) {
            return this->unwrap();
        }
        else {
            PANIC("%1%: %2%", msg);
        }
    }

    template <class Pred,
        std::enable_if_t<
            std::is_invocable_r_v<bool, Pred&&, T&>, bool> = false>
    maybe filter(Pred&& predicate) const {
        return this->is_just() && std::invoke(std::forward<Pred>(predicate), storage_->deref())
            ? maybe(boost::optional<T>{storage_->deref()})
            : maybe(boost::optional<T>{boost::none});
    }

    template <class E = std::monostate>
    auto ok_or(E const& err = std::monostate{}) const {
        return this->is_just()
            ? result<T, E>{success{storage_->deref()}}
            : result<T, E>{failure{err}};
    }

    template <class F>
    std::enable_if_t<
        std::is_invocable_v<F&&>,
    result<T, std::invoke_result_t<F&&>>>
    ok_or_else(F&& err) const {
        return this->is_just()
            ? result<T, std::invoke_result_t<F&&>>{success{storage_->deref()}}
            : result<T, std::invoke_result_t<F&&>>{failure{std::invoke(std::forward<F>(err))}};
    }

    template <class F>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&, T&>,
            is_maybe<std::decay_t<std::invoke_result_t<F&&, T&>>>>,
    std::invoke_result_t<F&&, T&> >
    and_then(F&& f) const {
        return this->is_just()
            ? std::invoke(std::forward<F>(f),storage_->deref())
            : nothing<>;
    }

    template <class F>
    std::enable_if_t<
        std::conjunction_v<
            std::is_invocable<F&&>,
            is_maybe_with<std::decay_t<std::invoke_result_t<F&&>>, T>>,
    maybe>
    or_else(F&& f) const {
        return this->is_just()
            ? just(unwrap())
            : std::invoke(std::forward<F>(f));
    }

    template <class F>
    std::enable_if_t<std::is_invocable_v<F&&, T&>>
    and_finally(F&& f) const& {
        if (this->is_just())
            std::invoke(std::forward<F>(f), storage_->deref());
    }
};

template <class T, class U>
std::enable_if_t<meta::is_comparable_with<T, U>::value,
bool>
operator==(maybe<T> const& lhs, maybe<U> const& rhs) {
    return lhs && rhs && (lhs.unwrap() == rhs.unwrap());
}

template <class T, class U>
std::enable_if_t<meta::is_comparable_with<T, U>::value,
bool>
operator!=(maybe<T> const& lhs, maybe<U> const& rhs) {
    return !(lhs == rhs);
}

template <class T>
bool operator==(maybe<T> const& lhs, const nothing_t) {
    return lhs.is_nothing();
}

template <class T>
bool operator==(const nothing_t, maybe<T> const& rhs) {
    return rhs.is_nothing();
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
bool operator<(maybe<T> const& lhs, maybe<U> const& rhs) {
    return lhs.ok_or() < rhs.ok_or();
}

template <class T, class U>
bool operator<=(maybe<T> const& lhs, maybe<U> const& rhs) {
    return lhs.ok_or() <= rhs.ok_or();
}

template <class T, class U>
bool operator>(maybe<T> const& lhs, maybe<U> const& rhs) {
    return lhs.ok_or() > rhs.ok_or();
}

template <class T, class U>
bool operator>=(maybe<T> const& lhs, maybe<U> const& rhs) {
    return lhs.ok_or() >= rhs.ok_or();
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

template <class T> maybe(T&&) -> maybe<typename mitamagic::element_type<std::decay_t<T>>::type>;

}
#endif
