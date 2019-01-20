#ifndef MITAMA_RESULT_DETAIL
#define MITAMA_RESULT_DETAIL
#include <type_traits>
#include <utility>

namespace mitama
{
template <class...>
inline constexpr bool always_false_v = false;
template <class...>
inline constexpr bool always_true_v = true;

template <class, class = void>
struct is_comparable : std::false_type
{
};

template <class T>
struct is_comparable<T, std::void_t<decltype(std::declval<T const &>() == std::declval<T const &>())>> : std::true_type
{
};

template <class, class, class = void>
struct is_comparable_with : std::false_type
{
};

template <class T, class U>
struct is_comparable_with<T, U, std::void_t<decltype(std::declval<T const &>() == std::declval<U const &>())>> : std::true_type
{
};

struct protean_bool
{
  constexpr operator std::true_type() const { return {}; }
  constexpr operator std::false_type() const { return {}; }
};

template <class T, class... Ts>
struct Overload : T, Overload<Ts...>
{
  Overload(T a, Ts... xs) : T{a}, Overload<Ts...>{xs...} {}
  using T::operator();
  using Overload<Ts...>::operator();
};

template <class T>
struct Overload<T> : T
{
  Overload(T a) : T{a} {}
  using T::operator();
};

template <class... F>
inline constexpr Overload<F...>
make_overload(F &&... f)
{
  return {{std::forward<F>(f)}...};
}
template <class T>
struct type_transfer
{
  using type = T;
};

#define MITAMA_HAS_FUNC(XXX, ...)                                                                   \
  bool(false                                                                                        \
           ? ::mitama::make_overload(                                                               \
                 [](auto x) -> decltype(&decltype(x)::type::XXX, std::true_type{}) { return {}; },  \
                 [](...) -> std::false_type { return {}; })(::mitama::type_transfer<__VA_ARGS__>{}) \
           : ::mitama::protean_bool{})
} // namespace mitama
#endif