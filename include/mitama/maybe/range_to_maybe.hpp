#pragma once

#include <mitama/maybe/maybe.hpp>

#include <boost/hana/functional/overload_linearly.hpp>

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
} // namespace mitama::_range_to_maybe_detail

namespace mitama
{
template <class Range>
auto
range_to_maybe(Range&& range)
{
  return std::invoke(
      boost::hana::overload_linearly(
          [](auto&& range
          ) -> decltype(std::begin(range) != std::end(range), maybe(just(*std::begin(std::forward<Range>(range)))))
          {
            return std::begin(range) != std::end(range)
                       ? maybe(just(*std::begin(std::forward<Range>(range))))
                       : nothing;
          },
          [](auto&& range
          ) -> decltype(begin(range) != end(range), maybe(just(*begin(std::forward<Range>(range)))))
          {
            return begin(range) != end(range)
                       ? maybe(just(*begin(std::forward<Range>(range))))
                       : nothing;
          }
      ),
      std::forward<Range>(range)
  );
}
} // namespace mitama
