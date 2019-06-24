#ifndef MITAMA_RESULT_TRAIT
#define MITAMA_RESULT_TRAIT

#include <result/detail/meta.hpp>
#include <type_traits>
#include <utility>
#include <iostream>
#include <string>
#include <string_view>

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

template < >
struct formattable<std::monostate>
    : std::true_type
{
};
template < >
struct formattable<const std::monostate>
    : std::true_type
{
};

template <class T>
struct formattable<T, std::void_t<
                          decltype(std::declval<std::ostream &>() << std::declval<std::decay_t<T>>())>>
    : std::true_type
{
};

template <class, class=void>
struct formattable_range : std::false_type
{
};

template <class Range>
struct formattable_range<Range, std::void_t<decltype(std::declval<std::ostream&>() << *std::begin(std::declval<std::decay_t<Range>>()))>>
    : std::conjunction<
        std::negation<std::is_same<std::string, std::decay_t<Range>>>,
        std::negation<std::is_same<std::string_view, std::decay_t<Range>>>
    >
{
};

}

namespace mitama {
inline namespace meta {
namespace detail {
    template <class, class>
    struct is_formattable_tuple_detail;

    template <class Tuple, std::size_t... I>
    struct is_formattable_tuple_detail<Tuple, std::index_sequence<I...>>
        : std::conjunction<trait::formattable<std::tuple_element_t<I, Tuple>>...>
    {};
}}}

namespace mitama::trait {
template <class, class = void> struct formattable_tuple;

template <class Tuple>
struct formattable_tuple<Tuple, std::enable_if_t<meta::is_tuple_like<Tuple>::value>>
    : meta::detail::is_formattable_tuple_detail<Tuple, std::make_index_sequence<std::tuple_size_v<Tuple>>>
{};

}

#endif
