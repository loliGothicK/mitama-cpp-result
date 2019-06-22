#ifndef MITAMA_RESULT_DETAIL
#define MITAMA_RESULT_DETAIL
#include <type_traits>
#include <utility>

namespace mitama::dependent_bool {
  template <class...>
  inline constexpr bool always_false_v = false;
  template <class...>
  inline constexpr bool always_true_v = true;
}

namespace mitama::detail
{
  template < class T >
  struct remove_cvr: std::remove_cv<std::remove_reference_t<T>> {};

  template < class T >
  using remove_cvr_t = typename remove_cvr<T>::type;
}

namespace mitama {
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

} // namespace mitama
#endif