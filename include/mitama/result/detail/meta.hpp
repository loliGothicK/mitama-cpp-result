#ifndef MITAMA_RESULT_DETAIL_META_HPP
#define MITAMA_RESULT_DETAIL_META_HPP
#include <type_traits>
#include <utility>
#include <tuple>

namespace mitama::inline meta {

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
struct is_comparable<T, std::void_t<decltype(std::declval<T const& >() == std::declval<T const &>())>> : std::true_type {};

template <class, class, class = void>
struct is_comparable_with : std::false_type {};
template <class T, class U>
struct is_comparable_with<T, U, std::void_t<decltype(std::declval<T const& >() == std::declval<U const &>())>> : std::true_type {};

template <class, class = void>
struct is_less_comparable : std::false_type {};
template <class T>
struct is_less_comparable<T, std::void_t<decltype(std::declval<T const& >() < std::declval<T const &>())>> : std::true_type {};

template <class, class, class = void>
struct is_less_comparable_with : std::false_type {};
template <class T, class U>
struct is_less_comparable_with<T, U, std::void_t<decltype(std::declval<T const& >() < std::declval<U const &>())>> : std::true_type {};


namespace detail {
  template <class AlwaysVoid, class... Types> struct has_common_type: std::false_type {};
  template <class... Types>
  struct has_common_type<std::enable_if_t<::mitama::meta::has_type<std::common_type<Types...>>::value>, Types...>: std::true_type {};
}

template <class... Types>
struct has_common_type: detail::has_common_type<void, Types...> {};

namespace detail {
template < class, class, class = void > struct is_tuple_like_detail: std::false_type {};

template < class T, std::size_t... I >
struct is_tuple_like_detail<T, std::index_sequence<I...>, std::void_t<decltype(std::get<I>(std::declval<T>()))...>>
  : std::conjunction<has_type<std::tuple_element<I, T>>...>
{};

template < class TupleLike >
struct is_tuple_like_impl: is_tuple_like_detail<TupleLike, std::make_index_sequence<std::tuple_size_v<TupleLike>>> {};
}

/// is_tuple_like
/// requires
///   std::tuple_element<0, T>
///   std::tuple_size_v<T>
///   (T t) { std::get<0>(t) }
template < class T >
struct is_tuple_like
  : std::conjunction<
      has_value<std::tuple_size<T>>,
      detail::is_tuple_like_impl<T>
    >
{};

/// is_range
template <class, class = void> struct is_range: std::false_type {};
template <class Range>
struct is_range<Range, std::void_t<decltype(*std::begin(std::declval<std::decay_t<Range>>()), std::begin(std::declval<std::decay_t<Range>>()) != std::end(std::declval<std::decay_t<Range>>()))>>
  : std::true_type {};

}

#endif
