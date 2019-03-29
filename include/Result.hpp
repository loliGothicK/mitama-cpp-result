/**
 * @file Result.hpp
 * @brief emulation Rust std::result::Result
 * @author いなむのみたま
 */

#ifndef MITAMA_RESULT_HPP
#define MITAMA_RESULT_HPP
#include <boost/format.hpp>
#include <functional>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include "traits/perfect_traits_special_members.hpp"
#include "result/detail.hpp"
#include "result/fwd.hpp"

#define PANIC(...) \
  throw ::mitama::runtime_panic { macro_use, __FILE__, __LINE__, __VA_ARGS__ }

class macro_use_tag_t{};
inline static constexpr macro_use_tag_t macro_use{};

namespace mitama
{
class runtime_panic : public std::runtime_error
{
public:
  template <class... Args>
  runtime_panic(boost::format fmt, Args &&... args) noexcept
      : std::runtime_error((fmt % ... % args).str()) {}

  template <class... Args>
  explicit runtime_panic(macro_use_tag_t, const char *func, int line, std::string fmt, Args &&... args) noexcept
      : std::runtime_error(
            std::string{"runtime panicked at '"} + (boost::format(fmt) % ... % args).str() +
            (boost::format("', %1%:%2%") % std::string{func} % line).str()) {}
};

template <class>
struct is_result : std::false_type
{
};
template <class T, class E>
struct is_result<Result<T, E>> : std::true_type
{
};
template <class T>
inline constexpr bool is_result_v = is_result<T>::value;

template <class, class...>
struct is_result_with : std::false_type
{
};
template <class T, class E>
struct is_result_with<Result<T, E>, Ok<T>> : std::true_type
{
};
template <class T, class E>
struct is_result_with<Result<T, E>, Err<E>> : std::true_type
{
};
template <class T, class E>
struct is_result_with<Result<T, E>, Ok<T>, Err<E>> : std::true_type
{
};
template <class T, class... Requires>
inline constexpr bool is_result_with_v = is_result_with<T, Requires...>::value;

template <class>
struct is_err_type : std::false_type
{
};
template <class T>
struct is_err_type<Err<T>> : std::true_type
{
};
template <class>
struct is_ok_type : std::false_type
{
};
template <class T>
struct is_ok_type<Ok<T>> : std::true_type
{
};
} // namespace mitama

#include "result/result_impl.hpp"

namespace mitama
{
template <class T = std::monostate>
class[[nodiscard]] Ok
    : private ::mitamagic::perfect_trait_copy_move<
          std::is_copy_constructible_v<T>,
          std::conjunction_v<std::is_copy_constructible<T>, std::is_copy_assignable<T>>,
          std::is_move_constructible_v<T>,
          std::conjunction_v<std::is_move_constructible<T>, std::is_move_assignable<T>>,
          Ok<T>>,
      public result::printer_friend_injector<Ok<T>>
{
  template <class>
  friend class Ok;
  T x;
  template <class, class, class>
  friend class Result;
  template <class, class>
  friend class result::printer_friend_injector;
  template <class, class>
  friend class result::transpose_friend_injector;
  template <class... Requiers>
  using where = std::enable_if_t<std::conjunction_v<Requiers...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<Ok, U>>;
public:
  using ok_type = T;
  using err_type = void;

  Ok() = default;

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<T, U>,
                  std::is_convertible<U, T>> = required>
  Ok(U && u) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::forward<U>(u)) {}

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit Ok(U && u) noexcept(std::is_nothrow_constructible_v<T, U>)
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
  constexpr auto or_else(Op &&) noexcept
  {
    return *this;
  }

  template < class U = T >
  constexpr std::enable_if_t<std::is_copy_constructible_v<U>, T>
  unwrap_or_default() noexcept
  {
    return x;
  }

  template <class O>
  T unwrap_or_else(O &&) const noexcept
  {
    return x;
  }

  template <class T_, class E_>
  std::enable_if_t<
      mitama::is_comparable_with<T, T_>::value,
      bool>
  operator==(Result<T_, E_> const &rhs) const
  {
    return rhs.is_ok() ? rhs.unwrap() == this->x : false;
  }

  template <class T_>
  std::enable_if_t<
      mitama::is_comparable_with<T, T_>::value,
      bool>
  operator==(Ok<T_> const &rhs) const
  {
    return this->x == rhs.x;
  }

  template <class E_>
  bool operator==(Err<E_> const &) const
  {
    return false;
  }

};

template <class T>
Ok(T &&)->Ok<std::decay_t<T>>;

template <class E>
class[[nodiscard]] Err
    : private ::mitamagic::perfect_trait_copy_move<
          std::is_copy_constructible_v<E>,
          std::conjunction_v<std::is_copy_constructible<E>, std::is_copy_assignable<E>>,
          std::is_move_constructible_v<E>,
          std::conjunction_v<std::is_move_constructible<E>, std::is_move_assignable<E>>,
          Err<E>>,
      public result::printer_friend_injector<Err<E>>
{
  template <class>
  friend class Err;
  E x;
  template <class, class, class>
  friend class Result;
  template <class, class>
  friend class result::printer_friend_injector;
  template <class, class>
  friend class result::transpose_friend_injector;
  template <class... Requiers>
  using where = std::enable_if_t<std::conjunction_v<Requiers...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<Err, U>>;
public:
  using ok_type = void;
  using err_type = E;

  Err() = default;

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<E, U>,
                  std::is_convertible<U, E>> = required>
  Err(U && u) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::forward<U>(u)) {}

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<E, U>,
                  std::negation<std::is_convertible<U, E>>> = required>
  explicit Err(U && u) noexcept(std::is_nothrow_constructible_v<E, U>)
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
  constexpr auto and_then(Op &&)
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
  auto unwrap_or_else(O && op) const noexcept
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

  template <class T_, class E_>
  std::enable_if_t<
      mitama::is_comparable_with<E, E_>::value,
      bool>
  operator==(Result<T_, E_> const &rhs) const
  {
    return rhs.is_err() ? rhs.unwrap_err() == this->x : false;
  }

  template <class E_>
  std::enable_if_t<
      mitama::is_comparable_with<E, E_>::value,
      bool>
  operator==(Err<E_> const &rhs) const
  {
    return this->x == rhs.x;
  }

  template <class T_>
  bool operator==(Ok<T_> const &) const
  {
    return false;
  }
};

template <class E>
Err(E &&)->Err<std::decay_t<E>>;

inline constexpr auto None = std::nullopt;

template <class T>
constexpr std::optional<std::decay_t<T>> Some(T &&x)
{
  return {std::forward<T>(x)};
}

class in_place_ok_t
{
};

inline constexpr in_place_ok_t in_place_ok = {};

class in_place_err_t
{
};

inline constexpr in_place_err_t in_place_err = {};

template <class T, class E>
class[[nodiscard]] Result<T, E,
                          trait::where<std::is_destructible<T>,
                                       std::is_destructible<E>>>
    : private ::mitamagic::perfect_trait_copy_move<
          std::conjunction_v<std::is_copy_constructible<T>, std::is_copy_constructible<E>>,
          std::conjunction_v<std::is_copy_constructible<T>, std::is_copy_assignable<T>, std::is_copy_constructible<E>, std::is_copy_assignable<E>>,
          std::conjunction_v<std::is_move_constructible<T>, std::is_move_constructible<E>>,
          std::conjunction_v<std::is_move_constructible<T>, std::is_move_assignable<T>, std::is_move_constructible<E>, std::is_move_assignable<E>>,
          Result<T, E>>,
      public result::printer_friend_injector<Result<T, E>>,
      public result::unwrap_or_default_friend_injector<Result<T, E>>,
      public result::transpose_friend_injector<Result<T, E>>
{
  std::variant<Ok<T>, Err<E>> storage_;
  template <class, class>
  friend class result::printer_friend_injector;
  template <class, class>
  friend class result::transpose_friend_injector;

  template <class... Requiers>
  using where = std::enable_if_t<std::conjunction_v<Requiers...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class T_, class E_>
  using not_self = std::negation<std::is_same<Result, Result<T_, E_>>>;
public:
  using ok_type = T;
  using err_type = E;

  constexpr Result() noexcept = delete;

  template <class U,
            where<std::is_constructible<T, U>,
                  std::is_convertible<U, T>> = required>
  Result(Ok<U> const &ok)
      : storage_{std::in_place_type<Ok<T>>, ok.x}
  {
  }

  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit Result(Ok<U> const &ok)
      : storage_{std::in_place_type<Ok<T>>, ok.x}
  {
  }

  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit Result(Ok<U> && ok)
      : storage_{std::in_place_type<Ok<T>>, ok.x}
  {
  }

  template <class U,
            where<std::is_constructible<E, U>,
                  std::is_convertible<U, E>> = required>
  Result(Err<U> const &err)
      : storage_{std::in_place_type<Err<E>>, err.x}
  {
  }

  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit Result(Err<U> const &err)
      : storage_{std::in_place_type<Err<E>>, err.x}
  {
  }

  template <class U,
            where<std::is_constructible<E, U>,
                  std::is_convertible<U, E>> = required>
  Result(Err<U> && err)
      : storage_{std::in_place_type<Err<E>>, err.x}
  {
  }

  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit Result(Err<U> && err)
      : storage_{std::in_place_type<Err<E>>, err.x}
  {
  }

  template <class... Args,
            where<std::is_constructible<T, Args...>> = required>
  explicit Result(in_place_ok_t, Args && ... args)
      : storage_{std::in_place_type<Ok<T>>, std::forward<Args>(args)...}
  {
  }

  template <class... Args,
            where<std::is_constructible<E, Args...>> = required>
  explicit Result(in_place_err_t, Args && ... args)
      : storage_{std::in_place_type<Err<E>>, std::forward<Args>(args)...}
  {
  }

  template <class U, class... Args,
            where<std::is_constructible<T, std::initializer_list<U>, Args...>> = required>
  explicit Result(in_place_ok_t, std::initializer_list<U> il, Args && ... args)
      : storage_{std::in_place_type<Ok<T>>, il, std::forward<Args>(args)...}
  {
  }

  template <class U, class... Args,
            where<std::is_constructible<E, Args...>> = required>
  explicit Result(in_place_err_t, std::initializer_list<U> il, Args && ... args)
      : storage_{std::in_place_type<Err<E>>, il, std::forward<Args>(args)...}
  {
  }

  constexpr bool is_ok() const noexcept { return std::holds_alternative<Ok<T>>(storage_); }

  constexpr bool is_err() const noexcept { return std::holds_alternative<Err<E>>(storage_); }

  template <class U = T>
  constexpr std::enable_if_t<std::is_same_v<U, T> && std::is_copy_constructible_v<U>, std::optional<T>>
  ok() const &
  {
    if (is_ok())
    {
      return std::optional<T>{std::get<Ok<T>>(storage_).x};
    }
    else
    {
      return std::optional<T>{std::nullopt};
    }
  }

  template <class U = T>
  constexpr std::enable_if_t<std::is_same_v<U, T> && std::is_move_constructible_v<U>, std::optional<T>>
  ok() &&
  {
    if (is_ok())
    {
      return std::optional<T>{std::get<Ok<T>>(std::move(storage_)).x};
    }
    else
    {
      return std::optional<T>{std::nullopt};
    }
  }

  template <class F = E>
  constexpr std::enable_if_t<std::is_same_v<F, E> && std::is_copy_constructible_v<E>, std::optional<E>>
  err() const &
  {
    if (is_err())
    {
      return std::optional<E>{std::get<Err<E>>(storage_).x};
    }
    else
    {
      return std::optional<E>{std::nullopt};
    }
  }

  template <class F = E>
  constexpr std::enable_if_t<std::is_same_v<F, E> && std::is_move_constructible_v<E>, std::optional<E>>
  err() &&
  {
    if (is_err())
    {
      return std::optional<E>{std::get<Err<E>>(std::move(storage_)).x};
    }
    else
    {
      return std::optional<E>{std::nullopt};
    }
  }

  template <class O, class U = T, class F = E>
  constexpr auto map(O && op) const &->std::enable_if_t<std::conjunction_v<std::is_invocable<O, T>, std::is_same<U, T>, std::is_same<F, E>, std::is_copy_constructible<U>, std::is_copy_constructible<F>>,
                                                        Result<std::invoke_result_t<O, T>, E>>
  {
    using result_type = Result<std::invoke_result_t<O, T>, E>;
    return is_ok()
               ? static_cast<result_type>(Ok{std::invoke(std::forward<O>(op), std::get<Ok<T>>(storage_).x)})
               : static_cast<result_type>(Err{std::get<Err<E>>(storage_).x});
  }

  template <class O, class U = T, class F = E>
  constexpr auto map(O && op) &&->std::enable_if_t<std::conjunction_v<std::is_invocable<O, T>, std::is_same<U, T>, std::is_same<F, E>, std::is_move_constructible<U>, std::is_move_constructible<F>>,
                                                        Result<std::invoke_result_t<O, T>, E>>
  {
    using result_type = Result<std::invoke_result_t<O, T>, E>;
    return is_ok()
               ? static_cast<result_type>(Ok{std::invoke(std::forward<O>(op), std::get<Ok<T>>(std::move(storage_)).x)})
               : static_cast<result_type>(Err{std::get<Err<E>>(std::move(storage_)).x});
  }

  template <class Map, class Fallback, class U = T, class F = E>
  constexpr auto map_or_else(Fallback&& _fallback, Map&& _map) const&
      -> std::enable_if_t<std::conjunction_v<std::is_invocable<Map, T>,
                                             std::is_invocable<Fallback, E>,
                                             std::is_convertible<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>,
                                             std::is_convertible<std::invoke_result_t<Fallback, E>, std::invoke_result_t<Map, T>>,
                                             std::is_same<U, T>,
                                             std::is_same<F, E>,
                                             std::is_move_constructible<U>,
                                             std::is_move_constructible<F>>,
                          std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>>
  {
    using result_type = std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>;
    return is_ok()
               ? static_cast<result_type>(std::invoke(std::forward<Map>(_map), std::get<Ok<T>>(storage_).x))
               : static_cast<result_type>(std::invoke(std::forward<Fallback>(_fallback), std::get<Err<E>>(storage_).x));
  }

  template <class Map, class Fallback, class U = T, class F = E>
  constexpr auto map_or_else(Fallback&& _fallback, Map&& _map) &&
      -> std::enable_if_t<std::conjunction_v<std::is_invocable<Map, T>,
                                             std::is_invocable<Fallback, E>,
                                             std::is_convertible<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>,
                                             std::is_convertible<std::invoke_result_t<Fallback, E>, std::invoke_result_t<Map, T>>,
                                             std::is_same<U, T>,
                                             std::is_same<F, E>,
                                             std::is_move_constructible<U>,
                                             std::is_move_constructible<F>>,
                          std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>>
  {
    using result_type = std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>;
    return is_ok()
               ? static_cast<result_type>(std::invoke(std::forward<Map>(_map), std::get<Ok<T>>(std::move(storage_)).x))
               : static_cast<result_type>(std::invoke(std::forward<Fallback>(_fallback), std::get<Err<E>>(std::move(storage_)).x));
  }

  template <class O, class U = T, class F = E>
  constexpr auto map_err(O && op) const &->std::enable_if_t<std::conjunction_v<std::is_invocable<O, E>, std::is_same<U, T>, std::is_same<F, E>, std::is_copy_constructible<U>, std::is_copy_constructible<F>>,
                                                            Result<T, std::invoke_result_t<O, E>>>
  {
    using result_type = Result<T, std::invoke_result_t<O, E>>;
    return is_err()
               ? static_cast<result_type>(Err{std::invoke(std::forward<O>(op), std::get<Err<E>>(storage_).x)})
               : static_cast<result_type>(Ok{std::get<Ok<T>>(storage_).x});
  }

  template <class O, class U = T, class F = E>
  constexpr auto map_err(O && op) &&->std::enable_if_t<std::conjunction_v<std::is_invocable<O, E>, std::is_same<U, T>, std::is_same<F, E>, std::is_move_constructible<U>, std::is_move_constructible<F>>,
                                                       Result<T, std::invoke_result_t<O, E>>>
  {
    using result_type = Result<T, std::invoke_result_t<O, E>>;
    return is_err()
               ? static_cast<result_type>(Err{std::invoke(std::forward<O>(op), std::get<Err<E>>(std::move(storage_)).x)})
               : static_cast<result_type>(Ok{std::get<Ok<T>>(std::move(storage_)).x});
  }

  template <class O, class U = T, class F = E>
  constexpr auto and_then(O && op) const &->std::enable_if_t<std::conjunction_v<is_result_with<std::invoke_result_t<O, T>, Err<F>>, std::is_same<U, T>, std::is_same<F, E>, std::is_copy_constructible<U>, std::is_copy_constructible<F>>,
                                                             std::invoke_result_t<O, T>>
  {
    using result_type = std::invoke_result_t<O, T>;
    return is_ok()
               ? std::invoke(std::forward<O>(op), std::get<Ok<T>>(storage_).x)
               : static_cast<result_type>(Err{typename result_type::err_type(std::get<Err<E>>(storage_).x)});
  }

  template <class O, class U = T, class F = E>
  constexpr auto and_then(O && op) &&->std::enable_if_t<std::conjunction_v<is_result_with<std::invoke_result_t<O, T>, Err<F>>, std::is_same<U, T>, std::is_same<F, E>, std::is_move_constructible<U>, std::is_move_constructible<F>>,
                                                        std::invoke_result_t<O, T>>
  {
    using result_type = std::invoke_result_t<O, T>;
    return is_ok()
               ? std::invoke(std::forward<O>(op), std::get<Ok<T>>(std::move(storage_)).x)
               : static_cast<result_type>(Err{typename result_type::err_type(std::get<Err<E>>(std::move(storage_)).x)});
  }

  template <class O, class U = T, class F = E>
  constexpr auto or_else(O && op) const &->std::enable_if_t<std::conjunction_v<is_result_with<std::invoke_result_t<O, T>, Ok<U>>, std::is_same<U, T>, std::is_same<F, E>, std::is_copy_constructible<U>, std::is_copy_constructible<F>>,
                                                            std::invoke_result_t<O, E>>
  {
    using result_type = std::invoke_result_t<O, E>;
    return is_err()
               ? std::invoke(std::forward<O>(op), std::get<Err<E>>(storage_).x)
               : static_cast<result_type>(Ok{typename result_type::ok_type(std::get<Ok<T>>(storage_).x)});
  }

  template <class O, class U = T, class F = E>
  constexpr auto or_else(O && op) &&->std::enable_if_t<std::conjunction_v<is_result_with<std::invoke_result_t<O, T>, Ok<U>>, std::is_same<U, T>, std::is_same<F, E>, std::is_move_constructible<U>, std::is_move_constructible<F>>,
                                                       std::invoke_result_t<O, E>>
  {
    using result_type = std::invoke_result_t<O, E>;
    return is_err()
               ? std::invoke(std::forward<O>(op), std::get<Err<E>>(std::move(storage_)).x)
               : static_cast<result_type>(Ok{typename result_type::ok_type(std::get<Ok<T>>(std::move(storage_)).x)});
  }

  template <class U>
  constexpr auto operator&&(Result<U, E> const &res) const &->Result<U, E>
  {
    using result_type = Result<U, E>;
    return this->is_err()
               ? static_cast<result_type>(Err{std::get<Err<E>>(storage_).x})
               : res.is_err() ? static_cast<result_type>(Err{res.unwrap_err()})
                              : static_cast<result_type>(Ok{res.unwrap()});
  }

  template <class F>
  constexpr auto operator||(Result<T, F> const &res) const &->Result<T, F>
  {
    using result_type = Result<T, F>;
    return this->is_ok()
               ? static_cast<result_type>(Ok{std::get<Ok<T>>(storage_).x})
               : res.is_ok() ? static_cast<result_type>(Ok{res.unwrap()})
                             : static_cast<result_type>(Err{res.unwrap_err()});
  }

  T unwrap_or(T const &optb) const noexcept
  {
    return is_ok() ? std::get<Ok<T>>(storage_).x : optb;
  }

  template <class O>
  auto unwrap_or_else(O && op) const
      ->std::enable_if_t<std::is_invocable_r_v<T, O, E>, T>
  {
    if constexpr (std::is_invocable_r_v<T, O, E>) {
      return is_ok() ? std::get<Ok<T>>(storage_).x : std::invoke(std::forward<O>(op), std::get<Err<E>>(storage_).x);
    }
    else if constexpr (std::is_invocable_r_v<T, O>) {
      return is_ok() ? std::get<Ok<T>>(storage_).x : std::invoke(std::forward<O>(op));
    }
    else {
      static_assert(::mitama::dependent_bool::always_false_v<O>, "invalid argument: designated function object is not invocable");
    }
  }

  T unwrap() const
  {
    if constexpr (trait::formattable<E>::value)
    {
      return is_ok()
                 ? std::get<Ok<T>>(storage_).x
                 : PANIC(R"(called `Result::unwrap() on an `Err` value: %1%)", std::get<Err<E>>(storage_).x);
    }
    else
    {
      return is_ok()
                 ? std::get<Ok<T>>(storage_).x
                 : PANIC(R"(called `Result::unwrap() on an `Err`)");
    }
  }

  E unwrap_err() const
  {
    if constexpr (trait::formattable<T>::value)
    {
      return is_err()
                 ? std::get<Err<E>>(storage_).x
                 : PANIC(R"(called `Result::unwrap_err() on an `Ok` value: %1%)", std::get<Ok<T>>(storage_).x);
    }
    else
    {
      return is_err()
                 ? std::get<Err<E>>(storage_).x
                 : PANIC(R"(called `Result::unwrap_err() on an `Ok` value)");
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

  template <class T_, class E_>
  std::enable_if_t<
      std::conjunction_v<mitama::is_comparable_with<T, T_>, mitama::is_comparable_with<E, E_>>,
      bool>
  operator==(Result<T_, E_> const &rhs) const &
  {
    return std::visit(::mitama::detail::overload(
                          [](Ok<T> const &l, Ok<T_> const &r) { return l.x == r.x; },
                          [](Err<E> const &l, Err<E_> const &r) { return l.x == r.x; },
                          [](auto &&...) { return false; }),
                      this->storage_, rhs.storage_);
  }

  template <class T_>
  std::enable_if_t<
      mitama::is_comparable_with<T, T_>::value,
      bool>
  operator==(Ok<T_> const &rhs) const
  {
    return this->is_ok() ? this->unwrap() == rhs.x : false;
  }

  template <class E_>
  std::enable_if_t<
      mitama::is_comparable_with<E, E_>::value,
      bool>
  operator==(Err<E_> const &rhs) const
  {
    return this->is_err() ? this->unwrap_err() == rhs.x : false;
  }

};

} // namespace mitama
#endif