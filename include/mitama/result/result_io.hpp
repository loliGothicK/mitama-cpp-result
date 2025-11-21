#pragma once

#include <mitama/mitamagic/format.hpp>
#include <mitama/result/result.hpp>

#include <iostream>
#include <string>

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
std::ostream&
operator<<(std::ostream& os, const basic_result<_, T, E>& res)
{
  return res.is_ok()
             ? os << std::format("success({})", quote_str(res.unwrap()))
             : os << std::format("failure({})", quote_str(res.unwrap_err()));
}

} // namespace mitama

template <mitama::mutability _, class T, class E>
struct std::formatter<mitama::basic_result<_, T, E>, char>
    : std::formatter<std::string, char>
{
  template <class FormatContext>
  auto
  format(const mitama::basic_result<_, T, E>& value, FormatContext& ctx) const
  {
    auto rendered =
        value.is_ok()
            ? std::format("success({})", quote_str(value.unwrap()))
            : std::format("failure({})", quote_str(value.unwrap_err()));
    return std::formatter<std::string, char>::format(rendered, ctx);
  }
};
