#pragma once

#include <mitama/result/detail/meta.hpp>

#include <iostream>
#include <type_traits>
#include <utility>
#include <variant>

namespace mitama::trait
{

/// Atomic Constraint
template <class T, class = void>
struct formattable_element : std::false_type
{
};
/// Higher Kind Constraint
template <class, class = void>
struct formattable_range : std::false_type
{
};
/// Higher Kind Constraint
template <class, class = void>
struct formattable_dictionary : std::false_type
{
};
/// Higher Kind Constraint
template <class, class = void>
struct formattable_tuple : std::false_type
{
};
/// Constraits Facade
template <class T, class = void>
struct formattable : std::false_type
{
};

template <class T>
  requires formattable_element<T>::value || formattable_range<T>::value
           || formattable_dictionary<T>::value || formattable_tuple<T>::value
struct formattable<T> : std::true_type
{
};

template <>
struct formattable_element<std::monostate> : std::true_type
{
};

template <>
struct formattable_element<const std::monostate> : std::true_type
{
};

template <class T>
struct formattable_element<
    T,
    std::void_t<
        decltype(std::declval<std::ostream&>() << std::declval<std::decay_t<T>>())>>
    : std::true_type
{
};

template <class Range>
  requires meta::is_range<Range>::value
struct formattable_range<Range>
    : formattable<typename std::remove_cvref_t<Range>::value_type>
{
};
} // namespace mitama::trait

namespace mitama
{
inline namespace meta
{
  namespace detail
  {
    template <class, class>
    struct is_formattable_tuple_detail : std::false_type
    {
    };

    template <class Tuple, std::size_t... I>
    struct is_formattable_tuple_detail<Tuple, std::index_sequence<I...>>
        : std::conjunction<
              trait::formattable<std::tuple_element_t<I, Tuple>>...>
    {
    };
  } // namespace detail
} // namespace meta
} // namespace mitama

namespace mitama::trait
{

template <class Tuple>
  requires meta::is_tuple_like<Tuple>::value
struct formattable_tuple<Tuple>
    : meta::detail::is_formattable_tuple_detail<
          Tuple, std::make_index_sequence<std::tuple_size_v<Tuple>>>
{
};

template <class Dict>
struct formattable_dictionary<
    Dict, std::void_t<
              typename std::remove_cvref_t<Dict>::key_type,
              typename std::remove_cvref_t<Dict>::mapped_type>>
    : std::conjunction<
          formattable<typename std::remove_cvref_t<Dict>::key_type>,
          formattable<typename std::remove_cvref_t<Dict>::mapped_type>>
{
};

} // namespace mitama::trait
