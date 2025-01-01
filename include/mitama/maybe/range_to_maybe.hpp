#pragma once

#include <mitama/maybe/maybe.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace mitama::_range_to_maybe_detail
{
using std::begin, std::end;
template <class T, class = std::void_t<>, class = std::void_t<>>
struct is_range : std::false_type
{
};

template <class T>
struct is_range<
    T, std::enable_if_t<std::is_constructible_v<
           bool, decltype(begin(std::declval<T>()) != end(std::declval<T>()))>>>
    : std::true_type
{
  using type = decltype(*begin(std::declval<T>()));
};

template <class Range>
concept is_std_range = requires(Range range) {
  std::begin(range);
  std::end(range);
};

template <class Range>
concept is_adl_range = requires(Range range) {
  begin(range);
  end(range);
};
} // namespace mitama::_range_to_maybe_detail

namespace mitama
{
template <class Range>
auto
range_to_maybe(Range&& range)
{
  if constexpr (_range_to_maybe_detail::is_std_range<Range>)
  {
    return std::begin(range) != std::end(range)
               ? maybe(just(*std::begin(std::forward<Range>(range))))
               : nothing;
  }
  else if constexpr (_range_to_maybe_detail::is_adl_range<Range>)
  {
    return begin(range) != end(range)
               ? maybe(just(*begin(std::forward<Range>(range))))
               : nothing;
  }
  else
  {
    static_assert(!sizeof(Range*), "Unsupported range type"); // always false
  }
}
} // namespace mitama
