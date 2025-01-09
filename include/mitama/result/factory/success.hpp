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
/// class success_t:
/// The main use of this class is to propagate successful results to the
/// constructor of the result class.
template <class T>
class [[nodiscard]] success_t<T>
{
  template <class, class...>
  friend class success_t;
  T x;

  template <class U>
  using not_self = std::negation<std::is_same<success_t, U>>;

public:
  using ok_type = T;

  template <class U = T>
    requires std::is_same_v<std::monostate, U>
  constexpr success_t(std::nullptr_t = nullptr)
  { /* whatever */
  }

  template <class U>
    requires not_self<std::decay_t<U>>::value
             && std::is_constructible_v<T, U> && std::is_convertible_v<U, T>
  constexpr success_t(U&& u) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::forward<U>(u))
  {
  }

  template <class U>
    requires not_self<std::decay_t<U>>::value
             && std::is_constructible_v<T, U> && (!std::is_convertible_v<U, T>)
  explicit constexpr success_t(U&& u
  ) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::forward<U>(u))
  {
  }

  template <typename U>
    requires(!std::is_same_v<T, U>) && std::is_constructible_v<T, const U&>
            && std::is_convertible_v<const U&, T>
  constexpr success_t(const success_t<U>& t
  ) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(t.get())
  {
  }

  template <typename U>
    requires(!std::is_same_v<T, U>) && std::is_constructible_v<T, const U&>
            && (!std::is_convertible_v<const U&, T>)
  explicit constexpr success_t(const success_t<U>& t
  ) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(t.get())
  {
  }

  template <typename U>
    requires(!std::is_same_v<T, U>)
            && std::is_constructible_v<T, U&&> && std::is_convertible_v<U&&, T>
  constexpr success_t(success_t<U>&& t
  ) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::move(t.get()))
  {
  }

  template <typename U>
    requires(!std::is_same_v<T, U>) && std::is_constructible_v<T, U&&>
            && (!std::is_convertible_v<U &&, T>)
  explicit constexpr success_t(success_t<U>&& t
  ) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::move(t.get()))
  {
  }

  template <class... Args>
    requires std::is_constructible_v<T, Args...>
  explicit constexpr success_t(
      std::in_place_t, Args&&... args
  ) noexcept(std::is_nothrow_constructible_v<T, Args...>)
      : x(std::forward<Args>(args)...)
  {
  }

  template <mutability _mut, class T_, class E_>
    requires meta::is_comparable_with<T, T_>::value
  constexpr bool operator==(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_ok() ? rhs.unwrap() == this->x : false;
  }

  template <class T_>
    requires meta::is_comparable_with<T, T_>::value
  constexpr bool operator==(const success_t<T_>& rhs) const
  {
    return this->x == rhs.x;
  }

  template <class E_>
  constexpr bool operator==(const failure_t<E_>&) const
  {
    return false;
  }

  template <mutability _mut, class T_, class E_>
    requires meta::is_comparable_with<T, T_>::value
  constexpr bool operator!=(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_ok() ? !(rhs.unwrap() == this->x) : true;
  }

  template <class T_>
    requires meta::is_comparable_with<T, T_>::value
  constexpr bool operator!=(const success_t<T_>& rhs) const
  {
    return !(this->x == rhs.x);
  }

  template <class E_>
  constexpr bool operator!=(const failure_t<E_>&) const
  {
    return true;
  }

  template <mutability _mut, class T_, class E_>
    requires meta::is_less_comparable_with<T, T_>::value
  constexpr bool operator<(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_ok() ? this->x < rhs.unwrap() : false;
  }

  template <class T_>
    requires meta::is_less_comparable_with<T, T_>::value
  constexpr bool operator<(const success_t<T_>& rhs) const
  {
    return this->x < rhs.x;
  }

  template <class E_>
  constexpr bool operator<(const failure_t<E_>&) const
  {
    return false;
  }

  template <mutability _mut, class T_, class E_>
    requires meta::is_comparable_with<T, T_>::value
             && meta::is_less_comparable_with<T, T_>::value
  constexpr bool operator<=(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_ok() ? (this->x == rhs.unwrap()) || (this->x < rhs.unwrap())
                       : false;
  }

  template <class T_>
    requires meta::is_comparable_with<T, T_>::value
             && meta::is_less_comparable_with<T, T_>::value
  constexpr bool operator<=(const success_t<T_>& rhs) const
  {
    return (this->x == rhs.x) || (this->x < rhs.x);
  }

  template <class E_>
  constexpr bool operator<=(const failure_t<E_>&) const
  {
    return false;
  }

  template <mutability _mut, class T_, class E_>
    requires meta::is_less_comparable_with<T_, T>::value
  constexpr bool operator>(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_ok() ? rhs.unwrap() < this->x : true;
  }

  template <class T_>
    requires meta::is_less_comparable_with<T_, T>::value
  constexpr bool operator>(const success_t<T_>& rhs) const
  {
    return rhs < *this;
  }

  template <class E_>
  constexpr bool operator>(const failure_t<E_>&) const
  {
    return true;
  }

  template <mutability _mut, class T_, class E_>
    requires meta::is_comparable_with<T_, T>::value
             && meta::is_less_comparable_with<T_, T>::value
  constexpr bool operator>=(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_ok() ? (rhs.unwrap() == this->x) || (rhs.is_ok() < this->x)
                       : true;
  }

  template <class T_>
    requires meta::is_comparable_with<T, T_>::value
  constexpr bool operator>=(const success_t<T_>& rhs) const
  {
    return rhs <= *this;
  }

  template <class E_>
  constexpr bool operator>=(const failure_t<E_>&) const
  {
    return true;
  }

  constexpr T& get() &
  {
    return x;
  }
  constexpr const T& get() const&
  {
    return x;
  }
  constexpr T&& get() &&
  {
    return std::move(x);
  }
};

template <class T>
class [[nodiscard]] success_t<T&>
{
  template <class, class...>
  friend class success_t;
  std::reference_wrapper<T> x;

  template <class U>
  using not_self = std::negation<std::is_same<success_t, U>>;

public:
  using ok_type = T&;

  success_t() = delete;
  explicit constexpr success_t(T& ok) : x(ok) {}
  explicit constexpr success_t(std::in_place_t, T& ok) : x(ok) {}

  template <class U>
  explicit constexpr success_t(U& value) : x(value)
  {
  }
  template <class U>
  explicit constexpr success_t(std::in_place_t, U& value) : x(value)
  {
  }

  explicit constexpr success_t(success_t&&) = default;
  explicit constexpr success_t(const success_t&) = default;
  constexpr success_t& operator=(success_t&&) = default;
  constexpr success_t& operator=(const success_t&) = default;

  template <mutability _mut, class T_, class E_>
    requires meta::is_comparable_with<T, T_>::value
  constexpr bool operator==(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_ok() ? rhs.unwrap() == this->x : false;
  }

  template <class T_>
    requires meta::is_comparable_with<T, T_>::value
  constexpr bool operator==(const success_t<T_>& rhs) const
  {
    return this->x == rhs.x;
  }

  template <class E_>
  constexpr bool operator==(const failure_t<E_>&) const
  {
    return false;
  }

  template <mutability _mut, class T_, class E_>
    requires meta::is_comparable_with<T, T_>::value
  constexpr bool operator!=(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_ok() ? !(rhs.unwrap() == this->x) : true;
  }

  template <class T_>
    requires meta::is_comparable_with<T, T_>::value
  constexpr bool operator!=(const success_t<T_>& rhs) const
  {
    return !(this->x == rhs.x);
  }

  template <class E_>
  constexpr bool operator!=(const failure_t<E_>&) const
  {
    return true;
  }

  template <mutability _mut, class T_, class E_>
    requires meta::is_less_comparable_with<T, T_>::value
  constexpr bool operator<(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_ok() ? this->x < rhs.unwrap() : false;
  }

  template <class T_>
    requires meta::is_less_comparable_with<T, T_>::value
  constexpr bool operator<(const success_t<T_>& rhs) const
  {
    return this->x < rhs.x;
  }

  template <class E_>
  constexpr bool operator<(const failure_t<E_>&) const
  {
    return false;
  }

  template <mutability _mut, class T_, class E_>
    requires meta::is_comparable_with<T, T_>::value
             && meta::is_less_comparable_with<T, T_>::value
  constexpr bool operator<=(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_ok() ? (this->x == rhs.unwrap()) || (this->x < rhs.unwrap())
                       : false;
  }

  template <class T_>
    requires meta::is_comparable_with<T, T_>::value
             && meta::is_less_comparable_with<T, T_>::value
  constexpr bool operator<=(const success_t<T_>& rhs) const
  {
    return (this->x == rhs.x) || (this->x < rhs.x);
  }

  template <class E_>
  constexpr bool operator<=(const failure_t<E_>&) const
  {
    return false;
  }

  template <mutability _mut, class T_, class E_>
    requires meta::is_less_comparable_with<T_, T>::value
  constexpr bool operator>(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_ok() ? rhs.unwrap() < this->x : true;
  }

  template <class T_>
    requires meta::is_less_comparable_with<T_, T>::value
  constexpr bool operator>(const success_t<T_>& rhs) const
  {
    return rhs < *this;
  }

  template <class E_>
  constexpr bool operator>(const failure_t<E_>&) const
  {
    return true;
  }

  template <mutability _mut, class T_, class E_>
    requires meta::is_comparable_with<T_, T>::value
             && meta::is_less_comparable_with<T_, T>::value
  constexpr bool operator>=(const basic_result<_mut, T_, E_>& rhs) const
  {
    return rhs.is_ok() ? (rhs.unwrap() == this->x) || (rhs.is_ok() < this->x)
                       : true;
  }

  template <class T_>
    requires meta::is_comparable_with<T, T_>::value
  constexpr bool operator>=(const success_t<T_>& rhs) const
  {
    return rhs <= *this;
  }

  template <class E_>
  constexpr bool operator>=(const failure_t<E_>&) const
  {
    return true;
  }

  constexpr T& get() &
  {
    return x.get();
  }
  constexpr const T& get() const&
  {
    return x.get();
  }
  constexpr T& get() &&
  {
    return x.get();
  }
};

template <class T, class... Args>
class [[nodiscard]] success_t<_result_detail::forward_mode<T>, Args...>
{
  std::tuple<Args...> args;

public:
  constexpr explicit success_t(Args... args) : args(std::forward<Args>(args)...)
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
success(Types&&... v)
{
  if constexpr (!std::is_void_v<Target>)
  {
    if constexpr (sizeof...(Types) < 2)
      return success_t<Target>{ std::forward<Types>(v)... };
    else
      return success_t<_result_detail::forward_mode<Target>, Types&&...>{
        std::forward<Types>(v)...
      };
  }
  else
  {
    if constexpr (sizeof...(Types) == 0)
      return success_t<>{};
    else if constexpr (sizeof...(Types) == 1)
      return success_t<Types...>{ std::forward<Types>(v)... };
    else
      return success_t<_result_detail::forward_mode<>, Types&&...>{
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
template <class T>
inline std::ostream&
operator<<(std::ostream& os, const success_t<T>& ok)
{
  return os << fmt::format("success({})", quote_str(ok.get()));
}

} // namespace mitama

template <class T>
struct fmt::formatter<mitama::success_t<T>> : ostream_formatter
{
};
