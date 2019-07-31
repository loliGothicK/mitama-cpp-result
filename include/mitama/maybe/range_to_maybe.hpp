#ifndef MITAMA_MAYBE_RANGE_TO_MAYBE_HPP
#define MITAMA_MAYBE_RANGE_TO_MAYBE_HPP
#include <mitama/maybe/maybe.hpp>
#include <type_traits>
#include <utility>
#include <iterator>

namespace mitama::_range_to_maybe_detail {
    using std::begin, std::end;
    template <class T, class = std::void_t<>, class = std::void_t<>>
    struct is_range: std::false_type {};

    template <class T>
    struct is_range<T,
        std::enable_if_t<
            std::is_constructible_v<bool, decltype(begin(std::declval<T>()) != end(std::declval<T>()))>>>
        : std::true_type
    {
        using type = decltype(*begin(std::declval<T>()));
    };
}

namespace mitama {
    template <class Range>
    auto range_to_maybe(Range&& range) {
        using std::begin, std::end;
        return begin(range) != end(range) ? maybe(just(*begin(std::forward<Range>(range)))) : nothing;
    }
}

#endif
