#pragma once

#include <mitama/mitamagic/format.hpp>
#include <mitama/result/result.hpp>

#include <boost/hana/functional/overload_linearly.hpp>

#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace mitama
{
/// @brief
///   ostream output operator
///
/// @requires
///   Format<T>;
///   Format<E>
///
/// @note
///   Output its contained value with pretty format, and is used by `operator<<`
///   found by ADL.
template <mutability _, class T, class E>
std::enable_if_t<
    std::conjunction_v<trait::formattable<T>, trait::formattable<E>>,
    std::ostream&>
operator<<(std::ostream& os, const basic_result<_, T, E>& res)
{
  auto inner_format = [](const auto& x) -> std::string
  {
    return boost::hana::overload_linearly(
          [](std::monostate) { return "()"; },
          [](std::string_view x) { return fmt::format(fmt::runtime("\"{}\""), x); },
          [](auto const& x) { return fmt::format(fmt::runtime("{}"), x); }
        )(x);
  };
  return res.is_ok()
             ? os << fmt::format("success({})", inner_format(res.unwrap()))
             : os << fmt::format("failure({})", inner_format(res.unwrap_err()));
}

} // namespace mitama

template <mitama::mutability _, class T, class E>
struct fmt::formatter<mitama::basic_result<_, T, E>> : ostream_formatter
{
};
