#pragma once

#include <mitama/maybe/fwd/maybe_fwd.hpp>
#include <mitama/mitamagic/format.hpp>
#include <mitama/result/detail/meta.hpp>

#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

namespace mitama::_just_detail
{
template <class T = void>
struct forward_mode
{
};
} // namespace mitama::_just_detail

namespace mitama
{

struct [[nodiscard]] nothing_t
{
};

inline constexpr nothing_t nothing{};

constexpr bool
operator==(const nothing_t, const nothing_t)
{
  return true;
}
constexpr bool
operator!=(const nothing_t, const nothing_t)
{
  return false;
}
constexpr bool
operator<(const nothing_t, const nothing_t)
{
  return false;
}
constexpr bool
operator>(const nothing_t, const nothing_t)
{
  return false;
}
constexpr bool
operator<=(const nothing_t, const nothing_t)
{
  return true;
}
constexpr bool
operator>=(const nothing_t, const nothing_t)
{
  return true;
}

inline std::ostream&
operator<<(std::ostream& os, nothing_t)
{
  return os << "nothing";
}

template <class>
struct is_just : std::false_type
{
};

template <class T>
struct is_just<just_t<T>> : std::true_type
{
};

template <class, class>
struct is_just_with : std::false_type
{
};

template <class T>
struct is_just_with<just_t<T>, T> : std::true_type
{
};

/// class just:
/// The main use of this class is to propagate some value to the constructor of
/// the maybe class.
template <class T>
class [[nodiscard]] just_t<T>
{
  template <class...>
  friend class just_t;

  T x;

  template <class U>
  using not_self = std::negation<std::is_same<just_t, U>>;

public:
  using type = T;

  template <class U = T>
    requires std::is_same_v<std::monostate, U>
  constexpr just_t(std::nullptr_t = nullptr)
  { /* whatever */
  }

  template <class U>
    requires not_self<std::decay_t<U>>::value
             && std::is_constructible_v<T, U> && std::is_convertible_v<U, T>
  constexpr just_t(U&& u) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::forward<U>(u))
  {
  }

  template <class U>
    requires not_self<std::decay_t<U>>::value
             && std::is_constructible_v<T, U> && (!std::is_convertible_v<U, T>)
  explicit constexpr just_t(
      U&& u
  ) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::forward<U>(u))
  {
  }

  template <typename U>
    requires(!std::is_same_v<T, U>) && std::is_constructible_v<T, const U&>
            && std::is_convertible_v<const U&, T>
  constexpr just_t(
      const just_t<U>& t
  ) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(t.get())
  {
  }

  template <typename U>
    requires(!std::is_same_v<T, U>) && std::is_constructible_v<T, const U&>
            && (!std::is_convertible_v<const U&, T>)
  explicit constexpr just_t(
      const just_t<U>& t
  ) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(t.get())
  {
  }

  template <typename U>
    requires(!std::is_same_v<T, U>)
            && std::is_constructible_v<T, U&&> && std::is_convertible_v<U&&, T>
  constexpr just_t(
      just_t<U>&& t
  ) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::move(t.get()))
  {
  }

  template <typename U>
    requires(!std::is_same_v<T, U>) && std::is_constructible_v<T, U&&>
            && (!std::is_convertible_v<U &&, T>)
  explicit constexpr just_t(
      just_t<U>&& t
  ) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::move(t.get()))
  {
  }

  template <class... Args>
    requires std::is_constructible_v<T, Args...>
  explicit constexpr just_t(
      std::in_place_t, Args&&... args
  ) noexcept(std::is_nothrow_constructible_v<T, Args...>)
      : x(std::forward<Args>(args)...)
  {
  }

  friend constexpr bool operator==(const nothing_t, const just_t&)
  {
    return false;
  }

  friend constexpr bool operator==(const just_t&, const nothing_t)
  {
    return false;
  }

  template <class U>
    requires meta::is_comparable_with<T, U>::value
  constexpr bool operator==(const just_t<U>& rhs) const
  {
    return this->x == rhs.get();
  }

  template <class U>
    requires meta::is_comparable_with<T, U>::value
  friend constexpr bool operator==(const maybe<U>& lhs, const just_t& rhs)
  {
    return lhs.is_just() && (lhs.unwrap() == rhs.get());
  }

  template <class U>
    requires meta::is_comparable_with<T, U>::value
  friend constexpr bool operator==(const just_t& lhs, const maybe<U>& rhs)
  {
    return rhs.is_just() && (lhs.get() == rhs.unwrap());
  }

  friend constexpr bool operator!=(const nothing_t, const just_t&)
  {
    return true;
  }

  friend constexpr bool operator!=(const just_t&, const nothing_t)
  {
    return true;
  }

  template <class U>
    requires meta::is_comparable_with<T, U>::value
  constexpr bool operator!=(const just_t<U>& rhs) const
  {
    return !(this->x == rhs.get());
  }

  template <class U>
    requires meta::is_comparable_with<T, U>::value
  friend constexpr bool operator!=(const maybe<U>& lhs, const just_t& rhs)
  {
    return lhs.is_nothing() || !(lhs.unwrap() == rhs.get());
  }

  template <class U>
    requires meta::is_comparable_with<T, U>::value
  friend constexpr bool operator!=(const just_t& lhs, const maybe<U>& rhs)
  {
    return rhs.is_nothing() || !(lhs.get() == rhs.unwrap());
  }

  friend constexpr bool operator<(const nothing_t, const just_t&)
  {
    return true;
  }

  friend constexpr bool operator<(const just_t&, const nothing_t)
  {
    return false;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
  constexpr bool operator<(const just_t<U>& rhs) const
  {
    return this->x < rhs.get();
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
  friend constexpr bool operator<(const just_t& lhs, const maybe<U>& rhs)
  {
    return rhs.is_just() ? lhs.get() < rhs.unwrap() : false;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
  friend constexpr bool operator<(const maybe<U>& lhs, const just_t& rhs)
  {
    return lhs.is_just() ? lhs.unwrap() < rhs.get() : true;
  }

  friend constexpr bool operator<=(const nothing_t, const just_t&)
  {
    return true;
  }

  friend constexpr bool operator<=(const just_t&, const nothing_t)
  {
    return false;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_comparable_with<T, U>::value
  constexpr bool operator<=(const just_t<U>& rhs) const
  {
    return this->x < rhs.get() || this->x == rhs.get();
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_comparable_with<T, U>::value
  friend constexpr bool operator<=(const just_t& lhs, const maybe<U>& rhs)
  {
    return rhs.is_just()
               ? (lhs.get() < rhs.unwrap() || lhs.get() == rhs.unwrap())
               : false;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_comparable_with<T, U>::value
  friend constexpr bool operator<=(const maybe<U>& lhs, const just_t& rhs)
  {
    return lhs.is_just()
               ? (lhs.unwrap() < rhs.get() || lhs.unwrap() == rhs.get())
               : true;
  }

  friend constexpr bool operator>(const nothing_t, const just_t&)
  {
    return false;
  }

  friend constexpr bool operator>(const just_t&, const nothing_t)
  {
    return true;
  }

  template <class U>
    requires meta::is_comparable_with<T, U>::value
  constexpr bool operator>(const just_t<U>& rhs) const
  {
    return rhs < *this;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
  friend constexpr bool operator>(const just_t& lhs, const maybe<U>& rhs)
  {
    return rhs < lhs;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
  friend constexpr bool operator>(const maybe<U>& lhs, const just_t& rhs)
  {
    return rhs < lhs;
  }

  friend constexpr bool operator>=(const nothing_t, const just_t&)
  {
    return false;
  }

  friend constexpr bool operator>=(const just_t&, const nothing_t)
  {
    return true;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_comparable_with<T, U>::value
  constexpr bool operator>=(const just_t<U>& rhs) const
  {
    return rhs <= *this;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_comparable_with<T, U>::value
  friend constexpr bool operator>=(const just_t& lhs, const maybe<U>& rhs)
  {
    return rhs <= lhs;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_comparable_with<T, U>::value
  friend constexpr bool operator>=(const maybe<U>& lhs, const just_t& rhs)
  {
    return rhs <= lhs;
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
class [[nodiscard]] just_t<T&>
{
  template <class...>
  friend class just_t;

  std::reference_wrapper<T> x;

  template <class U>
  using not_self = std::negation<std::is_same<just_t, U>>;

public:
  using type = T&;

  constexpr just_t() = delete;

  explicit constexpr just_t(T& ref) : x(ref) {}
  explicit constexpr just_t(std::in_place_t, T& ref) : x(ref) {}

  template <class U>
  explicit constexpr just_t(U& value) : x(value)
  {
  }
  template <class U>
  explicit constexpr just_t(std::in_place_t, U& value) : x(value)
  {
  }

  explicit constexpr just_t(just_t&&) = default;
  explicit constexpr just_t(const just_t&) = default;
  constexpr just_t& operator=(just_t&&) = default;
  constexpr just_t& operator=(const just_t&) = default;

  friend constexpr bool operator==(const nothing_t, const just_t&)
  {
    return false;
  }

  friend constexpr bool operator==(const just_t&, const nothing_t)
  {
    return false;
  }

  template <class U>
    requires meta::is_comparable_with<T, U>::value
  constexpr bool operator==(const just_t<U>& rhs) const
  {
    return this->x == rhs.get();
  }

  template <class U>
    requires meta::is_comparable_with<T, U>::value
  friend constexpr bool operator==(const maybe<U>& lhs, const just_t& rhs)
  {
    return lhs.is_just() && (lhs.unwrap() == rhs.get());
  }

  template <class U>
    requires meta::is_comparable_with<T, U>::value
  friend constexpr bool operator==(const just_t& lhs, const maybe<U>& rhs)
  {
    return rhs.is_just() && (lhs.get() == rhs.unwrap());
  }

  friend constexpr bool operator!=(const nothing_t, const just_t&)
  {
    return true;
  }

  friend constexpr bool operator!=(const just_t&, const nothing_t)
  {
    return true;
  }

  template <class U>
    requires meta::is_comparable_with<T, U>::value
  constexpr bool operator!=(const just_t<U>& rhs) const
  {
    return !(this->x == rhs.get());
  }

  template <class U>
    requires meta::is_comparable_with<T, U>::value
  friend constexpr bool operator!=(const maybe<U>& lhs, const just_t& rhs)
  {
    return lhs.is_nothing() || !(lhs.unwrap() == rhs.get());
  }

  template <class U>
    requires meta::is_comparable_with<T, U>::value
  friend constexpr bool operator!=(const just_t& lhs, const maybe<U>& rhs)
  {
    return rhs.is_nothing() || !(lhs.get() == rhs.unwrap());
  }

  friend constexpr bool operator<(const nothing_t, const just_t&)
  {
    return true;
  }

  friend constexpr bool operator<(const just_t&, const nothing_t)
  {
    return false;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
  constexpr bool operator<(const just_t<U>& rhs) const
  {
    return this->x < rhs.get();
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
  friend constexpr bool operator<(const just_t& lhs, const maybe<U>& rhs)
  {
    return rhs.is_just() ? lhs.get() < rhs.unwrap() : false;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
  friend constexpr bool operator<(const maybe<U>& lhs, const just_t& rhs)
  {
    return lhs.is_just() ? lhs.unwrap() < rhs.get() : true;
  }

  friend constexpr bool operator<=(const nothing_t, const just_t&)
  {
    return true;
  }

  friend constexpr bool operator<=(const just_t&, const nothing_t)
  {
    return false;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_comparable_with<T, U>::value
  constexpr bool operator<=(const just_t<U>& rhs) const
  {
    return this->x < rhs.get() || this->x == rhs.get();
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_comparable_with<T, U>::value
  friend constexpr bool operator<=(const just_t& lhs, const maybe<U>& rhs)
  {
    return rhs.is_just()
               ? (lhs.get() < rhs.unwrap() || lhs.get() == rhs.unwrap())
               : false;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_comparable_with<T, U>::value
  friend constexpr bool operator<=(const maybe<U>& lhs, const just_t& rhs)
  {
    return lhs.is_just()
               ? (lhs.unwrap() < rhs.get() || lhs.unwrap() == rhs.get())
               : true;
  }

  friend constexpr bool operator>(const nothing_t, const just_t&)
  {
    return false;
  }

  friend constexpr bool operator>(const just_t&, const nothing_t)
  {
    return true;
  }

  template <class U>
    requires meta::is_comparable_with<T, U>::value
  constexpr bool operator>(const just_t<U>& rhs) const
  {
    return rhs < *this;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
  friend constexpr bool operator>(const just_t& lhs, const maybe<U>& rhs)
  {
    return rhs < lhs;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
  friend constexpr bool operator>(const maybe<U>& lhs, const just_t& rhs)
  {
    return rhs < lhs;
  }

  friend constexpr bool operator>=(const nothing_t, const just_t&)
  {
    return false;
  }

  friend constexpr bool operator>=(const just_t&, const nothing_t)
  {
    return true;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_comparable_with<T, U>::value
  constexpr bool operator>=(const just_t<U>& rhs) const
  {
    return rhs <= *this;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_comparable_with<T, U>::value
  friend constexpr bool operator>=(const just_t& lhs, const maybe<U>& rhs)
  {
    return rhs <= lhs;
  }

  template <class U>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_comparable_with<T, U>::value
  friend constexpr bool operator>=(const maybe<U>& lhs, const just_t& rhs)
  {
    return rhs <= lhs;
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

template <class T>
inline std::ostream&
operator<<(std::ostream& os, const just_t<T>& j)
{
  return os << fmt::format("just({})", quote_str(j.get()));
}

template <class Target = void, class... Types>
constexpr auto
just(Types&&... v)
{
  if constexpr (sizeof...(Types) > 1)
  {
    return just_t<_just_detail::forward_mode<Target>, Types&&...>{
      std::forward<Types>(v)...
    };
  }
  else
  {
    if constexpr (!std::is_void_v<Target>)
      return just_t<_just_detail::forward_mode<Target>, Types&&...>{
        std::forward<Types>(v)...
      };
    else
      return just_t<Types...>{ std::forward<Types>(v)... };
  }
}

template <class Target = void, class T, class... Types>
constexpr auto
just(std::initializer_list<T> il, Types&&... v)
{
  return just_t<
      _just_detail::forward_mode<Target>, std::initializer_list<T>, Types&&...>{
    il, std::forward<Types>(v)...
  };
}

template <class T, class... Args>
class [[nodiscard]] just_t<_just_detail::forward_mode<T>, Args...>
{
  std::tuple<Args...> args;

public:
  constexpr explicit just_t(Args... args) : args(std::forward<Args>(args)...) {}

  constexpr auto operator()() &&
  {
    return std::apply(
        [](auto&&... fwd)
        { return std::forward_as_tuple(std::forward<decltype(fwd)>(fwd)...); },
        args
    );
  }
};

} // namespace mitama

template <class T>
struct fmt::formatter<mitama::just_t<T>> : ostream_formatter
{
};
