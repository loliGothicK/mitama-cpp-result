#pragma once

#include <mitama/maybe/factory/just_nothing.hpp>
#include <mitama/maybe/fwd/maybe_fwd.hpp>
#include <mitama/mitamagic/format.hpp>
#include <mitama/panic.hpp>
#include <mitama/result/detail/fwd.hpp>
#include <mitama/result/detail/meta.hpp>
#include <mitama/result/factory/failure.hpp>
#include <mitama/result/factory/success.hpp>
#include <mitama/result/traits/impl_traits.hpp>

#include <cassert>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

namespace mitama::mitamagic
{
template <class, class = void>
struct is_pointer_like : std::false_type
{
};

template <class PointerLike>
struct is_pointer_like<
    PointerLike, std::void_t<
                     decltype(std::declval<PointerLike&>().operator->()),
                     decltype(*std::declval<PointerLike&>()),
                     decltype(static_cast<bool>(std::declval<PointerLike&>()))>>
    : std::true_type
{
};

template <class, class = void>
struct element_type;

template <class T>
struct element_type<
    T, std::enable_if_t<
           std::disjunction_v<is_pointer_like<T>, std::is_pointer<T>>>>
{
  using type = std::remove_reference_t<decltype(*std::declval<T>())>;
};
} // namespace mitama::mitamagic

namespace mitama
{

template <class>
struct is_maybe : std::false_type
{
};

template <class T>
struct is_maybe<maybe<T>> : std::true_type
{
};

template <class, class>
struct is_maybe_with : std::false_type
{
};

template <class T>
struct is_maybe_with<maybe<T>, T> : std::true_type
{
};

template <class>
class maybe_transpose_injector
{
public:
  void transpose() = delete;
};

template <mutability _, class T, class E>
class maybe_transpose_injector<maybe<basic_result<_, T, E>>>
{
public:
  basic_result<_, maybe<T>, E> transpose() const&
  {
    const auto* self = static_cast<const maybe<basic_result<_, T, E>>*>(this);
    return self->is_nothing()
               ? basic_result<_, maybe<T>, E>{ success_t{ nothing } }
           : self->unwrap().is_ok()
               ? basic_result<_, maybe<T>, E>{ in_place_ok, std::in_place,
                                               self->unwrap().unwrap() }
               : basic_result<_, maybe<T>, E>{ in_place_err,
                                               self->unwrap().unwrap_err() };
  }
};

template <class T, class = void>
class maybe_unwrap_or_default_injector
{
public:
  void unwrap_or_default() = delete;
};

template <class T>
class maybe_unwrap_or_default_injector<
    maybe<T>, std::enable_if_t<std::disjunction_v<
                  std::is_default_constructible<T>, std::is_aggregate<T>>>>
{
public:
  T unwrap_or_default() const
  {
    const auto* self = static_cast<const maybe<T>*>(this);
    if constexpr (std::is_aggregate_v<T>)
    {
      return self->is_just() ? self->unwrap() : T{};
    }
    else
    {
      return self->is_just() ? self->unwrap() : T();
    }
  }
};

template <class, class = void>
class maybe_flatten_injector
{
public:
  void flatten() = delete;
};

template <class T>
class maybe_flatten_injector<
    maybe<T>, std::enable_if_t<is_maybe<std::decay_t<T>>::value>>
{
public:
  auto flatten() const&
  {
    const auto* self = static_cast<const maybe<T>*>(this);
    return self->is_just() ? self->unwrap().as_ref().cloned() : nothing;
  }
};

template <class, class = void>
class maybe_cloned_injector
{
public:
  void cloned() = delete;
};

template <class T>
class maybe_cloned_injector<
    maybe<T>, std::enable_if_t<std::conjunction_v<
                  std::is_lvalue_reference<T>,
                  std::is_copy_constructible<
                      std::remove_const_t<std::remove_reference_t<T>>>>>>
{
public:
  maybe<std::remove_reference_t<T>> cloned() const
  {
    auto decay_copy = [](auto&& some
                      ) -> std::remove_const_t<std::remove_reference_t<T>>
    { return std::forward<decltype(some)>(some); };
    const auto* self = static_cast<const maybe<T>*>(this);
    return self->is_just() ? maybe<std::remove_reference_t<T>>{ just(
                                 decay_copy(self->unwrap())
                             ) }
                           : nothing;
  }
};

template <class, class = void>
class maybe_replace_injector
{
public:
  void cloned() = delete;
};

template <class T>
class maybe_replace_injector<
    maybe<T>, std::enable_if_t<std::is_copy_constructible_v<
                  std::remove_const_t<std::remove_reference_t<T>>>>>
{
public:
  template <class... Args>
  std::enable_if_t<std::is_constructible_v<T, Args&&...>, maybe<T>>
  replace(Args&&... args) &
  {
    auto* self_mut = static_cast<maybe<T>*>(this);
    auto old = self_mut->as_ref().cloned();
    self_mut->storage_.template emplace<just_t<T>>(
        std::in_place, std::forward<Args>(args)...
    );
    return old;
  }

  template <class F, class... Args>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<F, Args&&...>,
          std::is_constructible<T, std::invoke_result_t<F&&, Args&&...>>>,
      maybe<T>>
  replace_with(F&& f, Args&&... args) &
  {
    auto* self_mut = static_cast<maybe<T>*>(this);
    auto old = self_mut->as_ref().cloned();
    self_mut->storage_.template emplace<just_t<T>>(
        std::invoke(std::forward<F>(f), std::forward<Args>(args)...)
    );
    return old;
  }
};

template <class T>
class maybe : public maybe_transpose_injector<maybe<T>>,
              public maybe_unwrap_or_default_injector<maybe<T>>,
              public maybe_flatten_injector<maybe<T>>,
              public maybe_cloned_injector<maybe<T>>,
              public maybe_replace_injector<maybe<T>>
{
  std::variant<nothing_t, just_t<T>> storage_;
  template <class, class>
  friend class maybe_replace_injector;
  template <class>
  friend class maybe;

public:
  using value_type = std::remove_reference_t<T>;
  using reference_type = std::add_lvalue_reference_t<value_type>;

  ~maybe() = default;
  constexpr maybe() : storage_(std::in_place_type<nothing_t>) {}
  maybe(const maybe&) = default;
  maybe& operator=(const maybe&) = default;
  maybe(maybe&&) = default;
  maybe& operator=(maybe&&) = default;
  maybe(nothing_t) : maybe() {}

  template <
      typename U, std::enable_if_t<
                      std::conjunction_v<
                          std::is_constructible<T, U&&>,
                          std::is_convertible<std::decay_t<U>, T>>,
                      bool> = false>
  maybe(U&& u)
      : storage_(
            std::in_place_type<just_t<T>>, std::in_place, std::forward<U>(u)
        )
  {
  }

  template <
      typename U,
      std::enable_if_t<
          std::conjunction_v<
              std::is_constructible<T, U&&>,
              std::negation<std::is_convertible<std::decay_t<U>, T>>>,
          bool> = false>
  explicit maybe(U&& u)
      : storage_(
            std::in_place_type<just_t<T>>, std::in_place, std::forward<U>(u)
        )
  {
  }

  template <
      class... Args,
      std::enable_if_t<std::is_constructible_v<T, Args&&...>, bool> = false>
  explicit maybe(std::in_place_t, Args&&... args)
      : storage_(
            std::in_place_type<just_t<T>>, std::in_place,
            std::forward<Args>(args)...
        )
  {
  }

  template <
      class U, class... Args,
      std::enable_if_t<
          std::is_constructible_v<T, std::initializer_list<U>, Args&&...>,
          bool> = false>
  explicit maybe(std::in_place_t, std::initializer_list<U> il, Args&&... args)
      : storage_(
            std::in_place_type<just_t<T>>, std::in_place, il,
            std::forward<Args>(args)...
        )
  {
  }

  template <
      class... Args,
      std::enable_if_t<std::is_constructible_v<T, Args...>, bool> = false>
  maybe(just_t<_just_detail::forward_mode<T>, Args...>&& fwd) : maybe()
  {
    std::apply(
        [&](auto&&... args)
        {
          storage_.template emplace<just_t<T>>(
              std::in_place, std::forward<decltype(args)>(args)...
          );
        },
        std::move(fwd)()
    );
  }

  template <
      class... Args,
      std::enable_if_t<std::is_constructible_v<T, Args...>, bool> = false>
  maybe(just_t<_just_detail::forward_mode<>, Args...>&& fwd) : maybe()
  {
    std::apply(
        [&](auto&&... args)
        {
          storage_.template emplace<just_t<T>>(
              std::in_place, std::forward<decltype(args)>(args)...
          );
        },
        std::move(fwd)()
    );
  }

  template <
      class U,
      std::enable_if_t<std::is_constructible_v<T, const U&>, bool> = false>
  maybe(const just_t<U>& j)
      : storage_(std::in_place_type<just_t<T>>, std::in_place, j.get())
  {
  }

  template <
      class U, std::enable_if_t<std::is_constructible_v<T, U&&>, bool> = false>
  maybe(just_t<U>&& j)
      : storage_(
            std::in_place_type<just_t<T>>, std::in_place,
            static_cast<U&&>(j.get())
        )
  {
  }

  explicit operator bool() const
  {
    return is_just();
  }

  value_type* operator->() &
  {
    return &(std::get<just_t<T>>(storage_).get());
  }

  const value_type* operator->() const&
  {
    return &(std::get<just_t<T>>(storage_).get());
  }

  bool is_just() const
  {
    return std::holds_alternative<just_t<T>>(storage_);
  }

  bool is_nothing() const
  {
    return !is_just();
  }

  value_type& unwrap() &
  {
    if (is_nothing())
      PANIC("called `maybe::unwrap()` on a `nothing` value");
    return std::get<just_t<T>>(storage_).get();
  }

  std::add_const_t<std::remove_reference_t<T>>& unwrap() const&
  {
    if (is_nothing())
      PANIC("called `maybe::unwrap()` on a `nothing` value");
    return std::get<just_t<T>>(storage_).get();
  }

  value_type unwrap() &&
  {
    if (is_nothing())
      PANIC("called `maybe::unwrap()` on a `nothing` value");
    return std::move(std::get<just_t<T>>(storage_).get());
  }

  auto as_ref() &
  {
    return is_just() ? maybe<T&>(std::in_place, unwrap()) : nothing;
  }

  auto as_ref() const&
  {
    return is_just() ? maybe<const T&>(std::in_place, unwrap()) : nothing;
  }

  template <class... Args>
  std::enable_if_t<std::is_constructible_v<T, Args&&...>, value_type&>
  get_or_emplace(Args&&... args) &
  {
    return is_just() ? unwrap()
                     : (storage_.template emplace<just_t<T>>(
                            std::in_place, std::forward<Args>(args)...
                        ),
                        unwrap());
  }

  template <class F, class... Args>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<F&&, Args&&...>,
          std::is_constructible<T, std::invoke_result_t<F&&, Args&&...>>>,
      value_type&>
  get_or_emplace_with(F&& f, Args&&... args) &
  {
    return is_just() ? unwrap()
                     : (storage_.template emplace<just_t<T>>(
                            std::in_place,
                            std::invoke(
                                std::forward<F>(f), std::forward<Args>(args)...
                            )
                        ),
                        unwrap());
  }

  template <class U>
  std::enable_if_t<
      meta::has_type<std::common_type<T&, U&&>>::value,
      std::common_type_t<value_type&, U&&>>
  unwrap_or(U&& def) &
  {
    return is_just() ? unwrap() : std::forward<U>(def);
  }

  template <class U>
  std::enable_if_t<
      meta::has_type<std::common_type<const T&, U&&>>::value,
      std::common_type_t<const value_type&, U&&>>
  unwrap_or(U&& def) const&
  {
    return is_just() ? unwrap() : std::forward<U>(def);
  }

  template <class U>
  std::enable_if_t<
      meta::has_type<std::common_type<T&&, U&&>>::value,
      std::common_type_t<value_type&&, U&&>>
  unwrap_or(U&& def) &&
  {
    return is_just() ? std::move(unwrap()) : std::forward<U>(def);
  }

  template <class F>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<F&&>,
          meta::has_type<std::common_type<T&, std::invoke_result_t<F&&>>>>,
      std::common_type_t<const value_type&, std::invoke_result_t<F&&>>>
  unwrap_or_else(F&& f) &
  {
    return is_just() ? unwrap() : std::invoke(std::forward<F>(f));
  }

  template <class F>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<F&&>, meta::has_type<std::common_type<
                                      const T&, std::invoke_result_t<F&&>>>>,
      std::common_type_t<const value_type&, std::invoke_result_t<F&&>>>
  unwrap_or_else(F&& f) const&
  {
    return is_just() ? unwrap() : std::invoke(std::forward<F>(f));
  }

  template <class F>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<F&&>,
          meta::has_type<std::common_type<T&&, std::invoke_result_t<F&&>>>>,
      std::common_type_t<const value_type&, std::invoke_result_t<F&&>>>
  unwrap_or_else(F&& f) &&
  {
    return is_just() ? std::move(unwrap()) : std::invoke(std::forward<F>(f));
  }

  template <
      class F, class... Args,
      std::enable_if_t<std::is_invocable_v<F&&, value_type&, Args&&...>, bool> =
          false>
  auto map(F&& f, Args&&... args) &
  {
    using result_type = std::invoke_result_t<F&&, value_type&, Args&&...>;
    return is_just()
               ? maybe<result_type>{ just(std::invoke(
                     std::forward<F>(f), unwrap(), std::forward<Args>(args)...
                 )) }
               : nothing;
  }

  template <
      class F, class... Args,
      std::enable_if_t<
          std::is_invocable_v<F&&, const value_type&, Args&&...>, bool> = false>
  auto map(F&& f, Args&&... args) const&
  {
    using result_type = std::invoke_result_t<F&&, const value_type&, Args&&...>;
    return is_just()
               ? maybe<result_type>{ just(std::invoke(
                     std::forward<F>(f), unwrap(), std::forward<Args>(args)...
                 )) }
               : nothing;
  }

  template <
      class F, class... Args,
      std::enable_if_t<
          std::is_invocable_v<F&&, value_type&&, Args&&...>, bool> = false>
  auto map(F&& f, Args&&... args) &&
  {
    using result_type = std::invoke_result_t<F&&, value_type&&, Args&&...>;
    return is_just() ? maybe<result_type>{ just(std::invoke(
                           std::forward<F>(f), std::move(unwrap()),
                           std::forward<Args>(args)...
                       )) }
                     : nothing;
  }

  template <class U, class F, class... Args>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<F&&, value_type&, Args&&...>,
          meta::has_type<
              std::common_type<U&&, std::invoke_result_t<F&&, value_type&>>>>,
      std::common_type_t<
          U&&, std::invoke_result_t<F&&, value_type&, Args&&...>>>
  map_or(U&& def, F&& f, Args&&... args) &
  {
    return is_just()
               ? std::invoke(
                     std::forward<F>(f), unwrap(), std::forward<Args>(args)...
                 )
               : std::forward<U>(def);
  }

  template <class U, class F, class... Args>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<F&&, const value_type&, Args&&...>,
          meta::has_type<std::common_type<
              U&&, std::invoke_result_t<F&&, const value_type&, Args&&...>>>>,
      std::common_type_t<U&&, std::invoke_result_t<F&&, const T&, Args&&...>>>
  map_or(U&& def, F&& f, Args&&... args) const&
  {
    return is_just()
               ? std::invoke(
                     std::forward<F>(f), unwrap(), std::forward<Args>(args)...
                 )
               : std::forward<U>(def);
  }

  template <class U, class F, class... Args>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<F&&, T&&, Args&&...>,
          meta::has_type<std::common_type<
              U&&, std::invoke_result_t<F&&, value_type&&, Args&&...>>>>,
      std::common_type_t<
          U&&, std::invoke_result_t<F&&, value_type&&, Args&&...>>>
  map_or(U&& def, F&& f, Args&&... args) &&
  {
    return is_just() ? std::invoke(
                           std::forward<F>(f), std::move(unwrap()),
                           std::forward<Args>(args)...
                       )
                     : std::forward<U>(def);
  }

  template <class D, class F, class... Args>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<D&&>,
          std::is_invocable<F&&, value_type&, Args&&...>,
          meta::has_type<std::common_type<
              std::invoke_result_t<D&&>,
              std::invoke_result_t<F&&, value_type&, Args&&...>>>>,
      std::common_type_t<
          std::invoke_result_t<D&&>,
          std::invoke_result_t<F&&, value_type&, Args&&...>>>
  map_or_else(D&& def, F&& f, Args&&... args) &
  {
    return is_just()
               ? std::invoke(
                     std::forward<F>(f), unwrap(), std::forward<Args>(args)...
                 )
               : std::invoke(std::forward<D>(def));
  }

  template <class D, class F, class... Args>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<D&&>,
          std::is_invocable<F&&, const value_type&, Args&&...>,
          meta::has_type<std::common_type<
              std::invoke_result_t<D&&>,
              std::invoke_result_t<F&&, const value_type&, Args&&...>>>>,
      std::common_type_t<
          std::invoke_result_t<D&&>,
          std::invoke_result_t<F&&, const value_type&, Args&&...>>>
  map_or_else(D&& def, F&& f, Args&&... args) const&
  {
    return is_just()
               ? std::invoke(
                     std::forward<F>(f), unwrap(), std::forward<Args>(args)...
                 )
               : std::invoke(std::forward<D>(def));
  }

  template <class D, class F, class... Args>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<D&&>,
          std::is_invocable<F&&, value_type&&, Args&&...>,
          meta::has_type<std::common_type<
              std::invoke_result_t<D&&>,
              std::invoke_result_t<F&&, value_type&&, Args&&...>>>>,
      std::common_type_t<
          std::invoke_result_t<D&&>,
          std::invoke_result_t<F&&, value_type&&, Args&&...>>>
  map_or_else(D&& def, F&& f, Args&&... args) &&
  {
    return is_just() ? std::invoke(
                           std::forward<F>(f), std::move(unwrap()),
                           std::forward<Args>(args)...
                       )
                     : std::invoke(std::forward<D>(def));
  }

  value_type& expect(std::string_view msg) &
  {
    if (is_just())
    {
      return unwrap();
    }
    else
    {
      PANIC("{}", msg);
    }
  }

  const value_type& expect(std::string_view msg) const&
  {
    if (is_just())
    {
      return unwrap();
    }
    else
    {
      PANIC("{}", msg);
    }
  }

  value_type&& expect(std::string_view msg) &&
  {
    if (is_just())
    {
      return std::move(unwrap());
    }
    else
    {
      PANIC("{}", msg);
    }
  }

  template <class Pred>
  std::enable_if_t<std::is_invocable_r_v<bool, Pred&&, T&>, maybe>
  filter(Pred&& predicate) &
  {
    return is_just() && std::invoke(std::forward<Pred>(predicate), unwrap())
               ? maybe<T>(unwrap())
               : nothing;
  }

  template <class Pred>
  std::enable_if_t<std::is_invocable_r_v<bool, Pred&&, const T&>, maybe>
  filter(Pred&& predicate) const&
  {
    return is_just() && std::invoke(std::forward<Pred>(predicate), unwrap())
               ? maybe<T>(unwrap())
               : nothing;
  }

  template <class Pred>
  std::enable_if_t<std::is_invocable_r_v<bool, Pred&&, T&&>, maybe>
  filter(Pred&& predicate) &&
  {
    return is_just() && std::invoke(std::forward<Pred>(predicate), unwrap())
               ? maybe<T>(std::move(unwrap()))
               : nothing;
  }

  template <class E = std::monostate>
  auto ok_or(E&& err = {}) &
  {
    using ret_t = result<value_type, std::remove_reference_t<E>>;
    return is_just()
               ? ret_t{ success_t{ unwrap() } }
               : ret_t{ failure_t{ std::forward<E>(std::forward<E>(err)) } };
  }

  template <class E = std::monostate>
  auto ok_or(E&& err = {}) const&
  {
    using ret_t = result<value_type, std::remove_reference_t<E>>;
    return is_just()
               ? ret_t{ success_t{ unwrap() } }
               : ret_t{ failure_t{ std::forward<E>(std::forward<E>(err)) } };
  }

  template <class E = std::monostate>
  auto ok_or(E&& err = {}) &&
  {
    using ret_t = result<value_type, std::remove_reference_t<E>>;
    return is_just()
               ? ret_t{ success_t{ std::move(unwrap()) } }
               : ret_t{ failure_t{ std::forward<E>(std::forward<E>(err)) } };
  }

  template <class F>
  std::enable_if_t<
      std::is_invocable_v<F&&>, result<T, std::invoke_result_t<F&&>>>
  ok_or_else(F&& err) const&
  {
    return is_just()
               ? result<T, std::invoke_result_t<F&&>>{ success_t{ unwrap() } }
               : result<T, std::invoke_result_t<F&&>>{ failure_t{
                     std::invoke(std::forward<F>(err)) } };
  }

  template <class F>
  std::enable_if_t<
      std::is_invocable_v<F&&>, result<T, std::invoke_result_t<F&&>>>
  ok_or_else(F&& err) &&
  {
    return is_just() ? result<T, std::invoke_result_t<F&&>>{ success_t{
                           std::move(unwrap()) } }
                     : result<T, std::invoke_result_t<F&&>>{ failure_t{
                           std::invoke(std::forward<F>(err)) } };
  }

  template <class U>
  maybe<U> conj(const maybe<U>& rhs) const
  {
    return is_just() ? rhs : nothing;
  }

  template <class U>
  maybe<U> operator&&(const maybe<U>& rhs) const
  {
    return this->conj(rhs);
  }

  maybe<T> disj(const maybe<T>& rhs) const
  {
    return is_nothing() ? rhs : *this;
  }

  maybe<T> operator||(const maybe<T>& rhs) const
  {
    return this->disj(rhs);
  }

  maybe<T> xdisj(const maybe<T>& rhs) const
  {
    return is_just() ^ rhs.is_just() ? is_just() ? *this : rhs : nothing;
  }

  maybe<T> operator^(const maybe<T>& rhs) const
  {
    return this->xdisj(rhs);
  }

  template <class F, class... Args>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<F&&, T&, Args&&...>,
          is_maybe<std::decay_t<std::invoke_result_t<F&&, T&, Args&&...>>>>,
      std::invoke_result_t<F&&, T&, Args&&...>>
  and_then(F&& f, Args&&... args) &
  {
    return is_just()
               ? std::invoke(
                     std::forward<F>(f), unwrap(), std::forward<Args>(args)...
                 )
               : nothing;
  }

  template <class F, class... Args>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<F&&, const T&, Args&&...>,
          is_maybe<
              std::decay_t<std::invoke_result_t<F&&, const T&, Args&&...>>>>,
      std::invoke_result_t<F&&, const T&, Args&&...>>
  and_then(F&& f, Args&&... args) const&
  {
    return is_just()
               ? std::invoke(
                     std::forward<F>(f), unwrap(), std::forward<Args>(args)...
                 )
               : nothing;
  }

  template <class F, class... Args>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<F&&, T&&, Args&&...>,
          is_maybe<std::decay_t<std::invoke_result_t<F&&, T&&, Args&&...>>>>,
      std::invoke_result_t<F&&, T&, Args&&...>>
  and_then(F&& f, Args&&... args) &&
  {
    return is_just() ? std::invoke(
                           std::forward<F>(f), std::move(unwrap()),
                           std::forward<Args>(args)...
                       )
                     : nothing;
  }

  template <class F, class... Args>
  std::enable_if_t<
      std::conjunction_v<
          std::is_invocable<F&&, Args&&...>,
          is_maybe_with<std::decay_t<std::invoke_result_t<F&&, Args&&...>>, T>>,
      maybe>
  or_else(F&& f, Args&&... args) &
  {
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
  or_else(F&& f, Args&&... args) const&
  {
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
  or_else(F&& f, Args&&... args) &&
  {
    return is_just()
               ? just(std::move(unwrap()))
               : std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
  }

  template <class F, class... Args>
  std::enable_if_t<std::is_invocable_v<F&&, value_type&, Args&&...>>
  and_finally(F&& f, Args&&... args) &
  {
    if (is_just())
      std::invoke(std::forward<F>(f), unwrap(), std::forward<Args>(args)...);
  }

  template <class F, class... Args>
  std::enable_if_t<std::is_invocable_v<F&&, const value_type&>>
  and_finally(F&& f, Args&&... args) const&
  {
    if (is_just())
      std::invoke(std::forward<F>(f), unwrap(), std::forward<Args>(args)...);
  }

  template <class F, class... Args>
  void and_finally(F&& f, Args&&... args) &&
  {
    if constexpr (std::is_lvalue_reference_v<T>)
    {
      static_assert(std::is_invocable_v<F&&, value_type&, Args&&...>);
      if (is_just())
        std::invoke(std::forward<F>(f), unwrap(), std::forward<Args>(args)...);
    }
    else
    {
      static_assert(std::is_invocable_v<F&&, value_type&&>);
      if (is_just())
        std::invoke(std::forward<F>(f), std::move(unwrap()));
    }
  }

  template <class F, class... Args>
  std::enable_if_t<std::is_invocable_v<F&&, Args&&...>>
  or_finally(F&& f, Args&&... args) &
  {
    if (is_nothing())
      std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
  }

  template <class F, class... Args>
  std::enable_if_t<std::is_invocable_v<F&&, Args&&...>>
  or_finally(F&& f, Args&&... args) const&
  {
    if (is_nothing())
      std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
  }

  template <class F, class... Args>
  std::enable_if_t<std::is_invocable_v<F&&, Args&&...>>
  or_finally(F&& f, Args&&... args) &&
  {
    if (is_nothing())
      std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
  }

  template <class F>
  constexpr std::enable_if_t<
      std::disjunction_v<
          std::is_invocable<F, value_type&>, std::is_invocable<F>>,
      maybe&>
  and_peek(F&& f) &
  {
    if (is_just())
    {
      if constexpr (std::is_invocable_v<F, value_type&>)
      {
        std::invoke(std::forward<F>(f), unwrap());
      }
      else
      {
        std::invoke(std::forward<F>(f));
      }
    }
    return *this;
  }

  template <class F>
  std::enable_if_t<
      std::disjunction_v<
          std::is_invocable<F&&, const value_type&>, std::is_invocable<F&&>>,
      const maybe&>
  and_peek(F&& f) const&
  {
    if (is_just())
    {
      if constexpr (std::is_invocable_v<F, value_type&>)
      {
        std::invoke(std::forward<F>(f), unwrap());
      }
      else
      {
        std::invoke(std::forward<F>(f));
      }
    }
    return *this;
  }

  template <class F>
  constexpr std::enable_if_t<std::is_invocable_v<F&&>, maybe&> or_peek(F&& f) &
  {
    if (is_nothing())
      std::invoke(std::forward<F>(f));
    return *this;
  }

  template <class F>
  constexpr std::enable_if_t<std::is_invocable_v<F&&>, const maybe&>
  or_peek(F&& f) const&
  {
    if (is_nothing())
      std::invoke(std::forward<F>(f));
    return *this;
  }
};

template <class T>
maybe(T&&) -> maybe<T>;

template <class T, class U>
std::enable_if_t<meta::is_comparable_with<T, U>::value, bool>
operator==(const maybe<T>& lhs, const maybe<U>& rhs)
{
  return lhs.is_just() && rhs.is_just() ? (lhs.unwrap() == rhs.unwrap())
                                        : lhs.is_nothing() && rhs.is_nothing();
}

template <class T>
bool
operator==(const maybe<T>& lhs, const nothing_t)
{
  return lhs.is_nothing();
}

template <class T>
bool
operator==(const nothing_t, const maybe<T>& rhs)
{
  return rhs.is_nothing();
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<U>>>,
        meta::is_comparable_with<T, U>>,
    bool>
operator==(const maybe<T>& lhs, U&& rhs)
{
  return lhs == just(std::forward<U>(rhs));
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<T>>>,
        meta::is_comparable_with<T, U>>,
    bool>
operator==(T&& lhs, const maybe<U>& rhs)
{
  return just(std::forward<T>(lhs)) == rhs;
}

template <class T, class U>
std::enable_if_t<meta::is_comparable_with<T, U>::value, bool>
operator!=(const maybe<T>& lhs, const maybe<U>& rhs)
{
  return !(lhs == rhs);
}

template <class T>
bool
operator!=(const maybe<T>& lhs, const nothing_t)
{
  return lhs.is_just();
}

template <class T>
bool
operator!=(const nothing_t, const maybe<T>& rhs)
{
  return rhs.is_just();
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<U>>>,
        meta::is_comparable_with<T, U>>,
    bool>
operator!=(const maybe<T>& lhs, U&& rhs)
{
  return lhs != just(std::forward<U>(rhs));
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<T>>>,
        meta::is_comparable_with<T, U>>,
    bool>
operator!=(T&& lhs, const maybe<U>& rhs)
{
  return just(std::forward<T>(lhs)) != rhs;
}

template <class T, class U>
bool
operator<(const maybe<T>& lhs, const maybe<U>& rhs)
{
  return lhs.ok_or() < rhs.ok_or();
}

template <class T>
bool
operator<(nothing_t, const maybe<T>& rhs)
{
  return rhs.is_just();
}

template <class T>
bool
operator<(const maybe<T>&, nothing_t)
{
  return false;
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<U>>>,
        meta::is_less_comparable_with<T, U>>,
    bool>
operator<(const maybe<T>& lhs, U&& rhs)
{
  return lhs < just(std::forward<U>(rhs));
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<T>>>,
        meta::is_less_comparable_with<T, U>>,
    bool>
operator<(T&& lhs, const maybe<U>& rhs)
{
  return just(std::forward<T>(lhs)) < rhs;
}

template <class T, class U>
bool
operator<=(const maybe<T>& lhs, const maybe<U>& rhs)
{
  return lhs.ok_or() <= rhs.ok_or();
}

template <class T>
bool
operator<=(nothing_t, const maybe<T>&)
{
  return true;
}

template <class T>
bool
operator<=(const maybe<T>& lhs, nothing_t)
{
  return lhs.is_nothing();
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<U>>>,
        meta::is_less_comparable_with<T, U>, meta::is_comparable_with<T, U>>,
    bool>
operator<=(const maybe<T>& lhs, U&& rhs)
{
  return lhs <= just(std::forward<U>(rhs));
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<T>>>,
        meta::is_less_comparable_with<T, U>, meta::is_comparable_with<T, U>>,
    bool>
operator<=(T&& lhs, const maybe<U>& rhs)
{
  return just(std::forward<T>(lhs)) <= rhs;
}

template <class T, class U>
bool
operator>(const maybe<T>& lhs, const maybe<U>& rhs)
{
  return lhs.ok_or() > rhs.ok_or();
}

template <class T>
bool
operator>(nothing_t, const maybe<T>&)
{
  return false;
}

template <class T>
bool
operator>(const maybe<T>& lhs, nothing_t)
{
  return lhs.is_just();
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<U>>>,
        meta::is_less_comparable_with<U, T>>,
    bool>
operator>(const maybe<T>& lhs, U&& rhs)
{
  return lhs > just(std::forward<U>(rhs));
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<T>>>,
        meta::is_less_comparable_with<U, T>>,
    bool>
operator>(T&& lhs, const maybe<U>& rhs)
{
  return just(std::forward<T>(lhs)) > rhs;
}

template <class T, class U>
bool
operator>=(const maybe<T>& lhs, const maybe<U>& rhs)
{
  return lhs.ok_or() >= rhs.ok_or();
}

template <class T>
bool
operator>=(nothing_t, const maybe<T>& rhs)
{
  return rhs.is_nothing();
}

template <class T>
bool
operator>=(const maybe<T>&, nothing_t)
{
  return true;
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<U>>>,
        meta::is_less_comparable_with<U, T>, meta::is_comparable_with<U, T>>,
    bool>
operator>=(const maybe<T>& lhs, U&& rhs)
{
  return lhs >= just(std::forward<U>(rhs));
}

template <class T, class U>
std::enable_if_t<
    std::conjunction_v<
        std::negation<is_maybe<std::decay_t<T>>>,
        meta::is_less_comparable_with<U, T>, meta::is_comparable_with<U, T>>,
    bool>
operator>=(T&& lhs, const maybe<U>& rhs)
{
  return just(std::forward<T>(lhs)) >= rhs;
}

/// @brief
///   ostream output operator for maybe<T>
///
/// @requires
///   Format<T>;
///
/// @note
///   Output its contained value with pretty format, and is used by `operator<<`
///   found by ADL.
template <class T>
std::enable_if_t<trait::formattable<T>::value, std::ostream&>
operator<<(std::ostream& os, const maybe<T>& may)
{
  return may.is_just() ? os << fmt::format("just({})", quote_str(may.unwrap()))
                       : os << "nothing";
}

} // namespace mitama

template <class T>
struct fmt::formatter<mitama::maybe<T>> : ostream_formatter
{
};
