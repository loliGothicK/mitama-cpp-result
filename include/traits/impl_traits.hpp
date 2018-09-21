#pragma once

#include <type_traits>
#include <utility>
#include <iostream>

namespace rust_std::trait {

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



}