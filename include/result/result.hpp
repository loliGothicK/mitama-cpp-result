#ifndef MITAMA_RESULT_HPP
#define MITAMA_RESULT_HPP
#include <boost/format.hpp>
#include <functional>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <string_view>

#include <boost/variant.hpp>
#include <boost/optional.hpp>

#include <result/traits/perfect_traits_special_members.hpp>
#include <result/detail/detail.hpp>
#include <result/detail/fwd.hpp>

#define PANIC(...) \
  throw ::mitama::runtime_panic { ::mitama::macro_use, __FILE__, __LINE__, __VA_ARGS__ }

namespace mitama::detail {
  template < class T, class... Ts >
  bool holds_alternative(boost::variant<Ts...> const& var) {
    return boost::get<T>(&var) != nullptr;
  }
}

namespace mitama {

class macro_use_tag_t{};
inline static constexpr macro_use_tag_t macro_use{};

class runtime_panic : public std::runtime_error
{
public:
  template <class... Args>
  runtime_panic(boost::format fmt, Args &&... args) noexcept
      : std::runtime_error((fmt % ... % args).str()) {}

  template <class... Args>
  explicit runtime_panic(macro_use_tag_t, const char *func, int line, std::string fmt, Args &&... args) noexcept
      : std::runtime_error(
            std::string{"runtime panicked at '"} + (boost::format(fmt) % ... % [](auto&& arg [[maybe_unused]]){
              using namespace std::literals::string_view_literals;
              if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::monostate>) {
                return "()"sv;
              }
              else {
                return std::forward<decltype(arg)>(arg);
              }
            }(args)).str() +
            (boost::format("', %1%:%2%") % std::string{func} % line).str()) {}
};

template <class>
struct is_result : std::false_type {};
template <mutability _mut, class T, class E>
struct is_result<basic_result<_mut, T, E>> : std::true_type {};
template <class T>
inline constexpr bool is_result_v = is_result<T>::value;

template <class, class...>
struct is_result_with : std::false_type {};
template <mutability _mut, class T, class E>
struct is_result_with<basic_result<_mut, T, E>, Ok<T>> : std::true_type {};
template <mutability _mut, class T, class E>
struct is_result_with<basic_result<_mut, T, E>, Err<E>> : std::true_type {};
template <mutability _mut, class T, class E>
struct is_result_with<basic_result<_mut, T, E>, Ok<T>, Err<E>> : std::true_type {};

template <class T, class... Requires>
inline constexpr bool is_result_with_v = is_result_with<detail::remove_cvr_t<T>, Requires...>::value;

template <class>
struct is_err_type : std::false_type {};
template <class T>
struct is_err_type<Err<T>> : std::true_type {};
template <class>
struct is_ok_type : std::false_type {};
template <class T>
struct is_ok_type<Ok<T>> : std::true_type {};

} // !namespace mitama

#include <result/detail/result_impl.hpp>

namespace mitama {

template <class T>
class [[nodiscard]] Ok
    : private ::mitamagic::perfect_trait_copy_move<
          std::is_copy_constructible_v<T>,
          std::conjunction_v<std::is_copy_constructible<T>, std::is_copy_assignable<T>>,
          std::is_move_constructible_v<T>,
          std::conjunction_v<std::is_move_constructible<T>, std::is_move_assignable<T>>,
          Ok<T>>,
      public printer_friend_injector<Ok<T>>
{
  template <class>
  friend class Ok;
  T x;
  template <mutability, class, class, class>
  friend class basic_result;
  template <class, class>
  friend class printer_friend_injector;
  template <class, class>
  friend class transpose_friend_injector;
  template <class, class>
  friend class deref_friend_injector;

  template <class... Requiers>
  using where = std::enable_if_t<std::conjunction_v<Requiers...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<Ok, U>>;
public:
  using ok_type = T;

  template <class U = T>
  constexpr Ok(std::enable_if_t<std::is_same_v<std::monostate, U>, std::nullptr_t> = nullptr)
  { /* whatever */ }

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<T, U>,
                  std::is_convertible<U, T>> = required>
  constexpr Ok(U && u) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::forward<U>(u)) {}

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit constexpr Ok(U && u) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::forward<U>(u)) {}

  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, const U &>,
                  std::is_convertible<const U &, T>> = required>
  constexpr Ok(const Ok<U> &t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(t.x) {}

  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, const U &>,
                  std::negation<std::is_convertible<const U &, T>>> = required>
  explicit constexpr Ok(const Ok<U> &t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(t.x) {}

  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, U &&>,
                  std::is_convertible<U &&, T>> = required>
  constexpr Ok(Ok<U> && t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::move(t.x)) {}

  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, U &&>,
                  std::negation<std::is_convertible<U &&, T>>> = required>
  explicit constexpr Ok(Ok<U> && t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::move(t.x)) {}

  template <class... Args,
            where<std::is_constructible<T, Args...>> = required>
  explicit constexpr Ok(std::in_place_t, Args && ... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
      : x(std::forward<Args>(args)...) {}

  constexpr std::true_type is_ok() const noexcept { return {}; }

  constexpr std::false_type is_err() const noexcept { return {}; }

  template <class Op>
  constexpr auto and_then(Op && op) noexcept(noexcept(std::declval<Op>()(std::declval<T &>())))
      ->std::enable_if_t<is_result_v<std::invoke_result_t<Op, T>>,
                         std::invoke_result_t<Op, T>>
  {
    return std::forward<Op>(op)(this->x);
  }

  template <class Op>
  constexpr decltype(auto) or_else(Op &&) noexcept
  {
    return *this;
  }

  template < class U = T >
  constexpr std::enable_if_t<std::is_copy_constructible_v<U>, T>
  unwrap_or_default() noexcept
  {
    return std::forward<T>(x);
  }

  template <class O>
  constexpr decltype(auto) unwrap_or_else(O &&) const noexcept
  {
    return x;
  }

  template <mutability _mut, class T_, class E_>
  std::enable_if_t<
      is_comparable_with<T, T_>::value,
      bool>
  constexpr operator==(basic_result<_mut, T_, E_> const &rhs) const
  {
    return rhs.is_ok() ? rhs.unwrap() == this->x : false;
  }

  template <class T_>
  std::enable_if_t<
      is_comparable_with<T, T_>::value,
      bool>
  constexpr operator==(Ok<T_> const &rhs) const
  {
    return this->x == rhs.x;
  }

  template <class E_>
  constexpr bool operator==(Err<E_> const &) const
  {
    return false;
  }

};

template <class T>
Ok(T &&)->Ok<T>;

template <class E>
class [[nodiscard]] Err
    : private ::mitamagic::perfect_trait_copy_move<
          std::is_copy_constructible_v<E>,
          std::conjunction_v<std::is_copy_constructible<E>, std::is_copy_assignable<E>>,
          std::is_move_constructible_v<E>,
          std::conjunction_v<std::is_move_constructible<E>, std::is_move_assignable<E>>,
          Err<E>>,
      public printer_friend_injector<Err<E>>
{
  template <class>
  friend class Err;
  E x;
  template <mutability, class, class, class>
  friend class basic_result;
  template <class, class>
  friend class printer_friend_injector;
  template <class, class>
  friend class transpose_friend_injector;
  template <class, class>
  friend class deref_friend_injector;

  template <class... Requiers>
  using where = std::enable_if_t<std::conjunction_v<Requiers...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<Err, U>>;
public:
  using err_type = E;

  template <class F = E>
  constexpr Err(std::enable_if_t<std::is_same_v<std::monostate, F>, std::nullptr_t> = nullptr)
  { /* whatever */ }

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<E, U>,
                  std::is_convertible<U, E>> = required>
  constexpr Err(U && u) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::forward<U>(u)) {}

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<E, U>,
                  std::negation<std::is_convertible<U, E>>> = required>
  explicit constexpr Err(U && u) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::forward<U>(u)) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, const U &>,
                  std::is_convertible<const U &, E>> = required>
  constexpr Err(const Err<U> &t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(t.x) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, const U &>,
                  std::negation<std::is_convertible<const U &, E>>> = required>
  explicit constexpr Err(const Err<U> &t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(t.x) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, U &&>,
                  std::is_convertible<U &&, E>> = required>
  constexpr Err(Err<U> && t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::move(t.x)) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, U &&>,
                  std::negation<std::is_convertible<U &&, E>>> = required>
  explicit constexpr Err(Err<U> && t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::move(t.x)) {}

  template <class... Args,
            where<std::is_constructible<E, Args...>> = required>
  explicit constexpr Err(std::in_place_t, Args && ... args) noexcept(std::is_nothrow_constructible_v<E, Args...>)
      : x(std::forward<Args>(args)...) {}

  constexpr std::false_type is_ok() const { return {}; }

  constexpr std::true_type is_err() const { return {}; }

  template <class Op>
  constexpr decltype(auto) and_then(Op &&)
  {
    return *this;
  }

  template <class Op>
  constexpr auto or_else(Op && op)
      ->std::enable_if_t<is_result_v<std::invoke_result_t<Op, E>>,
                         std::invoke_result_t<Op, E>>
  {
    return std::forward<Op>(op)(this->x);
  }

  template <class O>
  constexpr decltype(auto) unwrap_or_else(O && op) const noexcept
  {
    if constexpr (std::is_invocable_v<O, E>) {
      return std::invoke(std::forward<O>(op), x);
    }
    else if constexpr (std::is_invocable_v<O>) {
      return std::invoke(std::forward<O>(op));
    }
    else {
      static_assert(dependent_bool::always_false_v<O>, "invalid argument: designated function object is not invocable");
    }
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
      is_comparable_with<E, E_>::value,
      bool>
  operator==(basic_result<_mut, T_, E_> const &rhs) const
  {
    return rhs.is_err() ? rhs.unwrap_err() == this->x : false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
      is_comparable_with<E, E_>::value,
      bool>
  operator==(Err<E_> const &rhs) const
  {
    return this->x == rhs.x;
  }

  template <class T_>
  constexpr bool operator==(Ok<T_> const &) const
  {
    return false;
  }
};

template <class E>
Err(E &&)->Err<E>;

inline auto None = boost::none;

template <class T>
inline boost::optional<std::decay_t<T>> Some(T &&x)
{
  return {std::forward<T>(x)};
}

template <class T = std::monostate, class E = std::monostate>
using result = basic_result<mutability::constant, T, E>;
template <class T = std::monostate, class E = std::monostate>
using mut_result = basic_result<mutability::mut, T, E>;

template <mutability _mutability, class T, class E>
class [[nodiscard]] basic_result<_mutability, T, E,
  /* bounded types requirements */ 
  trait::where< 
    std::disjunction<
      std::conjunction<
        std::is_copy_constructible<detail::remove_cvr_t<T>>, 
        std::is_copy_constructible<detail::remove_cvr_t<E>>
      >,
      std::conjunction<
        std::is_move_constructible<detail::remove_cvr_t<T>>, 
        std::is_move_constructible<detail::remove_cvr_t<E>>
      >
    >,
    std::is_nothrow_destructible<detail::remove_cvr_t<T>>,
    std::is_nothrow_destructible<detail::remove_cvr_t<E>>
  >>
  : private ::mitamagic::perfect_trait_copy_move<
        std::conjunction_v<std::is_copy_constructible<boost::variant<Ok<T>, Err<E>>>,
        std::conjunction_v<std::is_copy_constructible<boost::variant<Ok<T>, Err<E>>>, std::is_copy_assignable<boost::variant<Ok<T>, Err<E>>>>,
        std::conjunction_v<std::is_move_constructible<boost::variant<Ok<T>, Err<E>>>,
        std::conjunction_v<std::is_move_constructible<boost::variant<Ok<T>, Err<E>>>, std::is_move_assignable<boost::variant<Ok<T>, Err<E>>>>,
        basic_result<_mutability, T, E>>,
    public printer_friend_injector<basic_result<_mutability, T, E>>,
    public unwrap_or_default_friend_injector<basic_result<_mutability, T, E>>,
    public transpose_friend_injector<basic_result<_mutability, T, E>>,
    public deref_friend_injector<basic_result<_mutability, T, E>>
{
  boost::variant<Ok<T>, Err<E>> storage_;
  template <class, class>
  friend class printer_friend_injector;
  template <class, class>
  friend class transpose_friend_injector;
  template <class, class>
  friend class deref_friend_injector;
  template <mutability, class, class, class>
  friend class basic_result;

  template <class... Requiers>
  using where = std::enable_if_t<std::conjunction_v<Requiers...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <mutability _mut, class T_, class E_>
  using not_self = std::negation<std::is_same<basic_result, basic_result<_mut, T_, E_>>>;
public:
  using ok_type = T;
  using err_type = E;
  using ok_reference_type = std::remove_reference_t<T>&;
  using err_reference_type = std::remove_reference_t<E>&;
  using ok_const_reference_type = detail::remove_cvr_t<T> const&;
  using err_const_reference_type = detail::remove_cvr_t<E> const&;

  constexpr basic_result() noexcept = delete;

  template <class U,
            where<std::is_constructible<T, U>,
                  std::is_convertible<U, T>> = required>
  basic_result(Ok<U> const &ok)
      : storage_{ok}
  {
  }

  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit basic_result(Ok<U> const &ok)
      : storage_{ok}
  {
  }

  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit basic_result(Ok<U> && ok)
      : storage_{ok}
  {
  }

  template <class U,
            where<std::is_constructible<E, U>,
                  std::is_convertible<U, E>> = required>
  basic_result(Err<U> const &err)
      : storage_{err}
  {
  }

  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit basic_result(Err<U> const &err)
      : storage_{err}
  {
  }

  template <class U,
            where<std::is_constructible<E, U>,
                  std::is_convertible<U, E>> = required>
  basic_result(Err<U> && err)
      : storage_{err}
  {
  }

  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit basic_result(Err<U> && err)
      : storage_{err}
  {
  }

  template <class... Args,
            where<std::is_constructible<T, Args...>> = required>
  explicit basic_result(in_place_ok_t, Args && ... args)
      : storage_{Ok<T>{std::forward<Args>(args)...}}
  {
  }

  template <class... Args,
            where<std::is_constructible<E, Args...>> = required>
  explicit basic_result(in_place_err_t, Args && ... args)
      : storage_{Err<E>{std::forward<Args>(args)...}}
  {
  }

  template <class U, class... Args,
            where<std::is_constructible<T, std::initializer_list<U>, Args...>> = required>
  explicit basic_result(in_place_ok_t, std::initializer_list<U> il, Args && ... args)
      : storage_{Ok<T>{il, std::forward<Args>(args)...}}
  {
  }

  template <class U, class... Args,
            where<std::is_constructible<E, Args...>> = required>
  explicit basic_result(in_place_err_t, std::initializer_list<U> il, Args && ... args)
      : storage_{Err<E>{il, std::forward<Args>(args)...}}
  {
  }

  constexpr bool is_ok() const noexcept { return ::mitama::detail::holds_alternative<Ok<T>>(storage_); }

  constexpr bool is_err() const noexcept { return ::mitama::detail::holds_alternative<Err<E>>(storage_); }

  explicit constexpr operator bool() const noexcept { return ::mitama::detail::holds_alternative<Ok<T>>(storage_); }
  constexpr bool operator !() const noexcept { return ::mitama::detail::holds_alternative<Err<E>>(storage_); }

  template <class U = T>
  constexpr std::enable_if_t<std::is_same_v<U, T> && std::is_copy_constructible_v<U>, boost::optional<T>>
  ok() const & {
    if (is_ok()) {
      return boost::optional<T>{boost::get<Ok<T>>(storage_).x};
    }
    else {
      return boost::optional<T>{boost::none};
    }
  }

  template <class U = T>
  constexpr std::enable_if_t<std::is_same_v<U, T> && std::is_move_constructible_v<U>, boost::optional<T>>
  ok() && {
    if (is_ok()) {
      return boost::optional<T>{boost::get<Ok<T>>(std::move(storage_)).x};
    }
    else {
      return boost::optional<T>{boost::none};
    }
  }

  template <class F = E>
  constexpr std::enable_if_t<std::is_same_v<F, E> && std::is_copy_constructible_v<E>, boost::optional<E>>
  err() const & {
    if (is_err()) {
      return boost::optional<E>{boost::get<Err<E>>(storage_).x};
    }
    else {
      return boost::optional<E>{boost::none};
    }
  }

  template <class F = E>
  constexpr std::enable_if_t<std::is_same_v<F, E> && std::is_move_constructible_v<E>, boost::optional<E>>
  err() && {
    if (is_err()) {
      return boost::optional<E>{boost::get<Err<E>>(std::move(storage_)).x};
    }
    else {
      return boost::optional<E>{boost::none};
    }
  }

  constexpr auto as_ref() const&
    -> basic_result<_mutability, detail::remove_cvr_t<T> const&, detail::remove_cvr_t<E> const&>
  {
    if ( is_ok() )
      return basic_result<_mutability, detail::remove_cvr_t<T> const&, detail::remove_cvr_t<E> const&>{in_place_ok, boost::get<Ok<T>>(storage_).x};
    else
      return basic_result<_mutability, detail::remove_cvr_t<T> const&, detail::remove_cvr_t<E> const&>{in_place_err, boost::get<Err<E>>(storage_).x};
  }

  constexpr auto as_ref() &
    -> basic_result<_mutability, std::remove_reference_t<T>&, std::remove_reference_t<E>&>
  {
    if ( is_ok() )
      return basic_result<_mutability, std::remove_reference_t<T>&, std::remove_reference_t<E>&>{in_place_ok, boost::get<Ok<T>>(storage_).x};
    else
      return basic_result<_mutability, std::remove_reference_t<T>&, std::remove_reference_t<E>&>{in_place_err, boost::get<Err<E>>(storage_).x};
  }

  template <class O, class U = T, class F = E>
  constexpr auto map(O && op) const &
    -> std::enable_if_t<
        std::conjunction_v<
          std::is_invocable<O, T>,
          std::is_same<U, T>,
          std::is_same<F, E>,
          std::is_copy_constructible<U>,
          std::is_copy_constructible<F>
        >,
        basic_result<_mutability, std::invoke_result_t<O, T>, E>>
  {
    using result_type = basic_result<_mutability, std::invoke_result_t<O, T>, E>;
    return is_ok()
               ? static_cast<result_type>(Ok{std::invoke(std::forward<O>(op), boost::get<Ok<T>>(storage_).x)})
               : static_cast<result_type>(Err{boost::get<Err<E>>(storage_).x});
  }

  template <class O, class U = T, class F = E>
  constexpr auto map(O && op) &&->std::enable_if_t<std::conjunction_v<std::is_invocable<O, T>, std::is_same<U, T>, std::is_same<F, E>, std::is_move_constructible<U>, std::is_move_constructible<F>>,
                                                   basic_result<_mutability, std::invoke_result_t<O, T>, E>>
  {
    using result_type = basic_result<_mutability, std::invoke_result_t<O, T>, E>;
    return is_ok()
               ? static_cast<result_type>(Ok{std::invoke(std::forward<O>(op), boost::get<Ok<T>>(std::move(storage_)).x)})
               : static_cast<result_type>(Err{boost::get<Err<E>>(std::move(storage_)).x});
  }

  template <class Map, class Fallback, class U = T, class F = E>
  constexpr auto map_or_else(Fallback&& _fallback, Map&& _map) const&
    -> std::enable_if_t<
          std::conjunction_v<std::is_invocable<Map, T>,
            std::is_invocable<Fallback, E>,
            std::is_convertible<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>,
            std::is_convertible<std::invoke_result_t<Fallback, E>, std::invoke_result_t<Map, T>>,
            std::is_same<U, T>,
            std::is_same<F, E>,
            std::is_move_constructible<U>,
            std::is_move_constructible<F>
          >,
          std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>>
  {
    using result_type = std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>;
    return is_ok()
               ? static_cast<result_type>(std::invoke(std::forward<Map>(_map), boost::get<Ok<T>>(storage_).x))
               : static_cast<result_type>(std::invoke(std::forward<Fallback>(_fallback), boost::get<Err<E>>(storage_).x));
  }

  template <class Map, class Fallback, class U = T, class F = E>
  constexpr auto map_or_else(Fallback&& _fallback, Map&& _map) &&
    -> std::enable_if_t<
          std::conjunction_v<std::is_invocable<Map, T>,
            std::is_invocable<Fallback, E>,
            std::is_convertible<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>,
            std::is_convertible<std::invoke_result_t<Fallback, E>, std::invoke_result_t<Map, T>>,
            std::is_same<U, T>,
            std::is_same<F, E>,
            std::is_move_constructible<U>,
            std::is_move_constructible<F>
          >,
          std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>>
  {
    using result_type = std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>;
    return is_ok()
               ? static_cast<result_type>(std::invoke(std::forward<Map>(_map), boost::get<Ok<T>>(std::move(storage_)).x))
               : static_cast<result_type>(std::invoke(std::forward<Fallback>(_fallback), boost::get<Err<E>>(std::move(storage_)).x));
  }

  template <class O, class U = T, class F = E>
  constexpr auto map_err(O && op) const &
    -> std::enable_if_t<
        std::conjunction_v<
          std::is_invocable<O, E>,
          std::is_same<U, T>,
          std::is_same<F, E>,
          std::is_copy_constructible<U>,
          std::is_copy_constructible<F>
        >,
        basic_result<_mutability, T, std::invoke_result_t<O, E>>>
  {
    using result_type = basic_result<_mutability, T, std::invoke_result_t<O, E>>;
    return is_err()
               ? static_cast<result_type>(Err{std::invoke(std::forward<O>(op), boost::get<Err<E>>(storage_).x)})
               : static_cast<result_type>(Ok{boost::get<Ok<T>>(storage_).x});
  }

  template <class O, class U = T, class F = E>
  constexpr auto map_err(O && op) &&
    -> std::enable_if_t<
        std::conjunction_v<
          std::is_invocable<O, E>,
          std::is_same<U, T>,
          std::is_same<F, E>,
          std::is_move_constructible<U>,
          std::is_move_constructible<F>
        >,
        basic_result<_mutability, T, std::invoke_result_t<O, E>>>
  {
    using result_type = basic_result<_mutability, T, std::invoke_result_t<O, E>>;
    return is_err()
               ? static_cast<result_type>(Err{std::invoke(std::forward<O>(op), boost::get<Err<E>>(std::move(storage_)).x)})
               : static_cast<result_type>(Ok{boost::get<Ok<T>>(std::move(storage_)).x});
  }

  template <class O, class U = T, class F = E>
  constexpr auto and_then(O && op) const &
    -> std::enable_if_t<
        std::conjunction_v<
          is_result_with<std::invoke_result_t<O, T>, Err<F>>,
          std::is_same<U, T>,
          std::is_same<F, E>,
          std::is_copy_constructible<U>,
          std::is_copy_constructible<F>
        >,
        std::invoke_result_t<O, T>>
  {
    using result_type = std::invoke_result_t<O, T>;
    return is_ok()
               ? std::invoke(std::forward<O>(op), boost::get<Ok<T>>(storage_).x)
               : static_cast<result_type>(Err{typename result_type::err_type(boost::get<Err<E>>(storage_).x)});
  }

  template <class O, class U = T, class F = E>
  constexpr auto and_then(O && op) &&
    -> std::enable_if_t<
        std::conjunction_v<
          is_result_with<std::invoke_result_t<O, T>, Err<F>>,
          std::is_same<U, T>,
          std::is_same<F, E>,
          std::is_move_constructible<U>,
          std::is_move_constructible<F>
        >,
        std::invoke_result_t<O, T>>
  {
    using result_type = std::invoke_result_t<O, T>;
    return is_ok()
               ? std::invoke(std::forward<O>(op), boost::get<Ok<T>>(std::move(storage_)).x)
               : static_cast<result_type>(Err{typename result_type::err_type(boost::get<Err<E>>(std::move(storage_)).x)});
  }

  template <class O, class U = T, class F = E>
  constexpr auto or_else(O && op) const &
    -> std::enable_if_t<
        std::conjunction_v<
          is_result_with<std::invoke_result_t<O, T>, Ok<U>>,
          std::is_same<U, T>,
          std::is_same<F, E>,
          std::is_copy_constructible<U>,
          std::is_copy_constructible<F>
        >,
        std::invoke_result_t<O, E>>
  {
    using result_type = std::invoke_result_t<O, E>;
    return is_err()
               ? std::invoke(std::forward<O>(op), boost::get<Err<E>>(storage_).x)
               : static_cast<result_type>(Ok{typename result_type::ok_type(boost::get<Ok<T>>(storage_).x)});
  }

  template <class O, class U = T, class F = E>
  constexpr auto or_else(O && op) &&
    -> std::enable_if_t<
        std::conjunction_v<
          is_result_with<std::invoke_result_t<O, T>, Ok<U>>,
          std::is_same<U, T>,
          std::is_same<F, E>,
          std::is_move_constructible<U>,
          std::is_move_constructible<F>
        >,
        std::invoke_result_t<O, E>>
  {
    using result_type = std::invoke_result_t<O, E>;
    return is_err()
               ? std::invoke(std::forward<O>(op), boost::get<Err<E>>(std::move(storage_)).x)
               : static_cast<result_type>(Ok{typename result_type::ok_type(boost::get<Ok<T>>(std::move(storage_)).x)});
  }

  template <mutability _mut, class U>
  constexpr decltype(auto) operator&&(basic_result<_mut, U, E> const &res) const &
  {
    using result_type = basic_result<_mutability, U, E>;
    return this->is_err()
               ? static_cast<result_type>(Err{boost::get<Err<E>>(storage_).x})
               : res.is_err() ? static_cast<result_type>(Err{res.unwrap_err()})
                              : static_cast<result_type>(Ok{res.unwrap()});
  }

  template <mutability _mut, class F>
  constexpr decltype(auto) operator||(basic_result<_mut, T, F> const &res) const &
  {
    using result_type = basic_result<_mutability, T, F>;
    return this->is_ok()
               ? static_cast<result_type>(Ok{boost::get<Ok<T>>(storage_).x})
               : res.is_ok() ? static_cast<result_type>(Ok{res.unwrap()})
                             : static_cast<result_type>(Err{res.unwrap_err()});
  }

  decltype(auto) unwrap_or(T const &optb) const noexcept
  {
    return is_ok() ? boost::get<Ok<T>>(storage_).x : optb;
  }

  template <class O>
  auto unwrap_or_else(O && op) const
    -> std::enable_if_t<std::is_invocable_r_v<T, O, E>, T>
  {
    if constexpr (std::is_invocable_r_v<T, O, E>) {
      return is_ok() ? boost::get<Ok<T>>(storage_).x : std::invoke(std::forward<O>(op), boost::get<Err<E>>(storage_).x);
    }
    else if constexpr (std::is_invocable_r_v<T, O>) {
      return is_ok() ? boost::get<Ok<T>>(storage_).x : std::invoke(std::forward<O>(op));
    }
    else {
      static_assert(::mitama::dependent_bool::always_false_v<O>, "invalid argument: designated function object is not invocable");
    }
  }

  decltype(auto) unwrap() const
  {
    if constexpr (trait::formattable<E>::value) {
      return is_ok()
                 ? static_cast<T>(boost::get<Ok<T>>(storage_).x)
                 : PANIC(R"(called `basic_result::unwrap() on an `Err` value: %1%)", boost::get<Err<E>>(storage_).x);
    }
    else {
      return is_ok()
                 ? static_cast<T>(boost::get<Ok<T>>(storage_).x)
                 : PANIC(R"(called `basic_result::unwrap() on an `Err`)");
    }
  }

  decltype(auto) unwrap_err() const
  {
    if constexpr (trait::formattable<T>::value) {
      return is_err()
                 ? static_cast<E>(boost::get<Err<E>>(storage_).x)
                 : PANIC(R"(called `basic_result::unwrap_err() on an `Ok` value: %1%)", boost::get<Ok<T>>(storage_).x);
    }
    else {
      return is_err()
                 ? static_cast<E>(boost::get<Err<E>>(storage_).x)
                 : PANIC(R"(called `basic_result::unwrap_err() on an `Ok` value)");
    }
  }

  template <class U = T, std::enable_if_t<std::is_same_v<T, U> && (std::is_default_constructible_v<U> || std::is_aggregate_v<U>), std::nullptr_t> = nullptr>
  T unwrap_or_default() const
  {
    if constexpr (std::is_aggregate_v<T>){
      return is_ok() ? unwrap() : T{};
    }
    else {
      return is_ok() ? unwrap() : T();
    }
  }

  void expect(std::string_view msg) const {
    if ( is_err() )
      PANIC("%1%: %2%", msg, unwrap_err());
  }

  void expect_err(std::string_view msg) const {
    if ( is_ok() )
      PANIC("%1%: %2%", msg, unwrap());
  }

  template <mutability _mut, class T_, class E_>
  std::enable_if_t<
      std::conjunction_v<is_comparable_with<T, T_>, is_comparable_with<E, E_>>,
      bool>
  operator==(basic_result<_mut, T_, E_> const &rhs) const &
  {
    return boost::apply_visitor(
      ::mitama::detail::overload(
        [](Ok<T> const &l, Ok<T_> const &r) { return l.x == r.x; },
        [](Err<E> const &l, Err<E_> const &r) { return l.x == r.x; },
        [](auto &&...) { return false; }),
      this->storage_, rhs.storage_);
  }

  template <class T_>
  std::enable_if_t<
      is_comparable_with<T, T_>::value,
      bool>
  operator==(Ok<T_> const &rhs) const
  {
    return this->is_ok() ? this->unwrap() == rhs.x : false;
  }

  template <class E_>
  std::enable_if_t<
      is_comparable_with<E, E_>::value,
      bool>
  operator==(Err<E_> const &rhs) const
  {
    return this->is_err() ? this->unwrap_err() == rhs.x : false;
  }

};

} // namespace mitama

#define TRY(target) \
  if (auto res = target; res.is_err()) \
    return ::mitama::Err(res.unwrap_err());

#define TRY_LET(var, target) \
  if (auto var = target; var.is_err()) \
    return ::mitama::Err(var.unwrap_err());
  
#endif
