#pragma once

#include <type_traits>
#include <utility>
#include <iostream>

namespace mitama::trait {

// Trait Helpers

template < class ... Requires >
using where = std::enable_if_t<std::conjunction_v<Requires...>, std::nullptr_t>;

inline constexpr std::nullptr_t required = nullptr;

// Formattable Trait
template <class T, class = void>
struct formattable
    : std::false_type
{
};

template <class T>
struct formattable<T, std::void_t<
                          decltype(std::declval<std::ostream &>() << std::declval<T const &>())>>
    : std::true_type
{
};

template <class, class=void>
struct formattable_range : std::false_type
{
};

template <>
struct formattable_range<std::string> : std::false_type
{
};

template <class Range>
struct formattable_range<Range, std::void_t<decltype(std::cout << *std::begin(std::declval<Range const&>()))>>
    : std::true_type
{
};
}