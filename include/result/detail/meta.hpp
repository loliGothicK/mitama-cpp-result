#ifndef MITAMA_RESULT_DETAIL_META_HPP
#define MITAMA_RESULT_DETAIL_META_HPP
#include <type_traits>

namespace mitama {
inline namespace meta {

template < class T > struct remove_cvr: std::remove_cv<std::remove_reference_t<T>> {};
template < class T > using remove_cvr_t = typename remove_cvr<T>::type;

template < class T, class = void >
struct has_type: std::false_type {};

template < class T >
struct has_type<T, std::void_t<typename std::decay_t<T>::type>>: std::true_type {};

template < class T, class = void >
struct has_value: std::false_type {};

template < class T >
struct has_value<T, std::void_t<decltype(std::decay_t<T>::value)>>: std::true_type {};

template < class T > struct force_add_const: std::add_const<T> {};
template < class T > struct force_add_const<T&> { using type = std::add_const_t<T>&; };
template < class T > struct force_add_const<T&&> { using type = std::add_const_t<T>&&; };
template < class T > using force_add_const_t = typename force_add_const<T>::type;


template <class, class = void>
struct is_comparable : std::false_type {};
template <class T>
struct is_comparable<T, std::void_t<decltype(std::declval<T const &>() == std::declval<T const &>())>> : std::true_type {};

template <class, class, class = void>
struct is_comparable_with : std::false_type {};
template <class T, class U>
struct is_comparable_with<T, U, std::void_t<decltype(std::declval<T const &>() == std::declval<U const &>())>> : std::true_type {};

}}

namespace mitama {
inline namespace meta {
namespace detail {
  template <class AlwaysVoid, class... Types> struct has_common_type: std::false_type {};
  template <class... Types>
  struct has_common_type<std::enable_if_t<::mitama::meta::has_type<std::common_type<Types...>>::value>, Types...>: std::true_type {};
}}}

namespace mitama{
inline namespace meta {
  template <class... Types>
  struct has_common_type: detail::has_common_type<void, Types...> {};
}
}


#endif
