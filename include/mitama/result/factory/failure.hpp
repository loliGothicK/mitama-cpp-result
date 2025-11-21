#pragma once

#include <mitama/mitamagic/format.hpp>
#include <mitama/result/detail/fwd.hpp>
#include <mitama/result/detail/meta.hpp>

#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

namespace mitama
{
/// class failure_t:
/// The main use of this class is to propagate unsuccessful results to the
/// constructor of the result class.
template <class E>
class [[nodiscard]] failure_t<E>
{
  template <class, class...>
  friend class failure_t;
  E x;

  template <class U>
  using not_self = std::negation<std::is_same<failure_t, U>>;

public:
  using err_type = E;

  template <class F = E>
    requires std::is_same_v<std::monostate, F>
  constexpr failure_t(std::nullptr_t = nullptr)
  { /* whatever */
  }

  template <class U>
    requires not_self<std::decay_t<U>>::value
             && std::is_constructible_v<E, U> && std::is_convertible_v<U, E>
  constexpr failure_t(U&& u) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::forward<U>(u))
  {
  }

  template <class U>
    requires not_self<std::decay_t<U>>::value
             && std::is_constructible_v<E, U> && (!std::is_convertible_v<U, E>)
  explicit constexpr failure_t(
      U&& u
  ) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::forward<U>(u))
  {
  }

  template <typename U>
    requires(!std::is_same_v<E, U>) && std::is_constructible_v<E, const U&>
            && std::is_convertible_v<const U&, E>
  constexpr failure_t(
      const failure_t<U>& t
  ) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(t.get())
  {
  }

  template <typename U>
    requires(!std::is_same_v<E, U>) && std::is_constructible_v<E, const U&>
            && (!std::is_convertible_v<const U&, E>)
  explicit constexpr failure_t(
      const failure_t<U>& t
  ) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(t.get())
  {
  }

  template <typename U>
    requires(!std::is_same_v<E, U>)
            && std::is_constructible_v<E, U&&> && std::is_convertible_v<U&&, E>
  constexpr failure_t(
      failure_t<U>&& t
  ) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::move(t.get()))
  {
  }

  template <typename U>
    requires(!std::is_same_v<E, U>) && std::is_constructible_v<E, U&&>
            && (!std::is_convertible_v<U &&, E>)
  explicit constexpr failure_t(
      failure_t<U>&& t
  ) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::move(t.get()))
  {
  }

  template <class... Args>
    requires std::is_constructible_v<E, Args...>
  explicit constexpr failure_t(
      std::in_place_t, Args&&... args
  ) noexcept(std::is_nothrow_constructible_v<E, Args...>)
      : x(std::forward<Args>(args)...)
  {
  }

  template <mutability _mut, class T_, class E_>
    requires is_comparable_with<E, E_>::value
  constexpr bool operator==(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_err() ? rhs.unwrap_err() == this->x : false;
  }

  template <class T_>
  constexpr bool operator==(const success_t<T_>&) const
  {
    return false;
  }

  template <class E_>
    requires is_comparable_with<E, E_>::value
  constexpr bool operator==(const failure_t<E_>& rhs) const
  {
    return this->x == rhs.x;
  }

  template <mutability _mut, class T_, class E_>
    requires is_comparable_with<E, E_>::value
  constexpr bool operator!=(const basic_result<_mut, T_, E_>& rhs) const
  {
    return !(*this == rhs);
  }

  template <class T_>
  constexpr bool operator!=(const success_t<T_>&) const
  {
    return false;
  }

  template <class E_>
    requires is_comparable_with<E, E_>::value
  constexpr bool operator!=(const failure_t<E_>& rhs) const
  {
    return !(this->x == rhs.x);
  }

  template <mutability _mut, class T_, class E_>
    requires is_comparable_with<E, E_>::value
  constexpr bool operator<(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_err() ? this->x < rhs.unwrap_err() : true;
  }

  template <class T_>
  constexpr bool operator<(const success_t<T_>&) const
  {
    return true;
  }

  template <class E_>
    requires meta::is_less_comparable_with<E, E_>::value
  constexpr bool operator<(const failure_t<E_>& rhs) const
  {
    return this->x < rhs.x;
  }

  template <mutability _mut, class T_, class E_>
    requires is_comparable_with<E, E_>::value
             && meta::is_less_comparable_with<E, E_>::value
  constexpr bool operator<=(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_err()
               ? (this->x == rhs.unwrap_err()) || (this->x < rhs.unwrap_err())
               : true;
  }

  template <class T_>
  constexpr bool operator<=(const success_t<T_>&) const
  {
    return true;
  }

  template <class E_>
    requires is_comparable_with<E, E_>::value
             && meta::is_less_comparable_with<E, E_>::value
  constexpr bool operator<=(const failure_t<E_>& rhs) const
  {
    return (*this == rhs) || (*this < rhs);
  }

  template <mutability _mut, class T_, class E_>
    requires meta::is_less_comparable_with<E_, E>::value
  constexpr bool operator>(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_err() ? rhs.unwrap_err() < this->x : false;
  }

  template <class T_>
  constexpr bool operator>(const success_t<T_>&) const
  {
    return false;
  }

  template <class E_>
    requires meta::is_comparable_with<E_, E>::value
  constexpr bool operator>(const failure_t<E_>& rhs) const
  {
    return rhs < *this;
  }

  template <mutability _mut, class T_, class E_>
    requires is_comparable_with<E_, E>::value
             && meta::is_less_comparable_with<E_, E>::value
  constexpr bool operator>=(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_err()
               ? (rhs.unwrap_err() == this->x) || (rhs.unwrap_err() < this->x)
               : false;
  }

  template <class T_>
  constexpr bool operator>=(const success_t<T_>&) const
  {
    return false;
  }

  template <class E_>
    requires is_comparable_with<E_, E>::value
             && meta::is_less_comparable_with<E_, E>::value
  constexpr bool operator>=(const failure_t<E_>& rhs) const
  {
    return rhs <= *this;
  }

  constexpr E& get() &
  {
    return x;
  }
  constexpr const E& get() const&
  {
    return x;
  }
  constexpr E&& get() &&
  {
    return std::move(x);
  }
};

template <class E>
class [[nodiscard]] failure_t<E&>
{
  template <class, class...>
  friend class failure_t;
  std::reference_wrapper<E> x;

  template <class U>
  using not_self = std::negation<std::is_same<failure_t, U>>;

public:
  using err_type = E&;

  failure_t() = delete;
  explicit constexpr failure_t(E& err) : x(err) {}
  explicit constexpr failure_t(std::in_place_t, E& err) : x(err) {}

  template <class U>
  explicit constexpr failure_t(U& value) : x(value)
  {
  }
  template <class U>
  explicit constexpr failure_t(std::in_place_t, U& value) : x(value)
  {
  }

  explicit constexpr failure_t(failure_t&&) = default;
  explicit constexpr failure_t(const failure_t&) = default;
  constexpr failure_t& operator=(failure_t&&) = default;
  constexpr failure_t& operator=(const failure_t&) = default;

  template <mutability _mut, class T_, class E_>
    requires is_comparable_with<E, E_>::value
  constexpr bool operator==(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_err() ? rhs.unwrap_err() == this->x : false;
  }

  template <class T_>
  constexpr bool operator==(const success_t<T_>&) const
  {
    return false;
  }

  template <class E_>
    requires is_comparable_with<E, E_>::value
  constexpr bool operator==(const failure_t<E_>& rhs) const
  {
    return this->x == rhs.x;
  }

  template <mutability _mut, class T_, class E_>
    requires is_comparable_with<E, E_>::value
  constexpr bool operator!=(const basic_result<_mut, T_, E_>& rhs) const
  {
    return !(*this == rhs);
  }

  template <class T_>
  constexpr bool operator!=(const success_t<T_>&) const
  {
    return false;
  }

  template <class E_>
    requires is_comparable_with<E, E_>::value
  constexpr bool operator!=(const failure_t<E_>& rhs) const
  {
    return !(this->x == rhs.x);
  }

  template <mutability _mut, class T_, class E_>
    requires is_comparable_with<E, E_>::value
  constexpr bool operator<(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_err() ? this->x < rhs.unwrap_err() : true;
  }

  template <class T_>
  constexpr bool operator<(const success_t<T_>&) const
  {
    return true;
  }

  template <class E_>
    requires meta::is_less_comparable_with<E, E_>::value
  constexpr bool operator<(const failure_t<E_>& rhs) const
  {
    return this->x < rhs.x;
  }

  template <mutability _mut, class T_, class E_>
    requires is_comparable_with<E, E_>::value
             && meta::is_less_comparable_with<E, E_>::value
  constexpr bool operator<=(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_err()
               ? (this->x == rhs.unwrap_err()) || (this->x < rhs.unwrap_err())
               : true;
  }

  template <class T_>
  constexpr bool operator<=(const success_t<T_>&) const
  {
    return true;
  }

  template <class E_>
    requires is_comparable_with<E, E_>::value
             && meta::is_less_comparable_with<E, E_>::value
  constexpr bool operator<=(const failure_t<E_>& rhs) const
  {
    return (*this == rhs) || (*this < rhs);
  }

  template <mutability _mut, class T_, class E_>
    requires meta::is_less_comparable_with<E_, E>::value
  constexpr bool operator>(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_err() ? rhs.unwrap_err() < this->x : false;
  }

  template <class T_>
  constexpr bool operator>(const success_t<T_>&) const
  {
    return false;
  }

  template <class E_>
    requires meta::is_comparable_with<E_, E>::value
  constexpr bool operator>(const failure_t<E_>& rhs) const
  {
    return rhs < *this;
  }

  template <mutability _mut, class T_, class E_>
    requires is_comparable_with<E_, E>::value
             && meta::is_less_comparable_with<E_, E>::value
  constexpr bool operator>=(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_err()
               ? (rhs.unwrap_err() == this->x) || (rhs.unwrap_err() < this->x)
               : false;
  }

  template <class T_>
  constexpr bool operator>=(const success_t<T_>&) const
  {
    return false;
  }

  template <class E_>
    requires is_comparable_with<E_, E>::value
             && meta::is_less_comparable_with<E_, E>::value
  constexpr bool operator>=(const failure_t<E_>& rhs) const
  {
    return rhs <= *this;
  }

  constexpr E& get() &
  {
    return x.get();
  }
  constexpr const E& get() const&
  {
    return x.get();
  }
  constexpr E& get() &&
  {
    return x.get();
  }
};

template <class T, class... Args>
class [[nodiscard]] failure_t<_result_detail::forward_mode<T>, Args...>
{
  std::tuple<Args...> args;

public:
  constexpr explicit failure_t(Args... args) : args(std::forward<Args>(args)...)
  {
  }

  constexpr auto operator()() &&
  {
    return std::apply(
        [](auto&&... fwd)
        { return std::forward_as_tuple(std::forward<decltype(fwd)>(fwd)...); },
        args
    );
  }
};

template <class Target = void, class... Types>
inline constexpr auto
failure(Types&&... v)
{
  if constexpr (!std::is_void_v<Target>)
  {
    if constexpr (sizeof...(Types) < 2)
      return failure_t<Target>{ std::forward<Types>(v)... };
    else
      return failure_t<_result_detail::forward_mode<Target>, Types&&...>{
        std::forward<Types>(v)...
      };
  }
  else
  {
    if constexpr (sizeof...(Types) == 0)
      return failure_t<>{};
    else if constexpr (sizeof...(Types) == 1)
      return failure_t<Types...>{ std::forward<Types>(v)... };
    else
      return failure_t<_result_detail::forward_mode<>, Types&&...>{
        std::forward<Types>(v)...
      };
  }
}

/// @brief
///   ostream output operator
///
/// @requires
///   Format<T>;
///   Format<E>
///
/// @note
///   Output its contained value with pretty format, and is used by `operator<<`
///   found by ADL.
template <class E>
inline std::ostream&
operator<<(std::ostream& os, const failure_t<E>& err)
{
  return os << fmt::format("failure({})", quote_str(err.get()));
}

} // namespace mitama

template <class T>
struct fmt::formatter<mitama::failure_t<T>> : ostream_formatter
{
};
