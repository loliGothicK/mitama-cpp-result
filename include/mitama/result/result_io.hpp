#pragma once

#include <mitama/mitamagic/format.hpp>
#include <mitama/result/result.hpp>

#include <iostream>

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
             ? os << fmt::format("ok({})", quote_str(res.unwrap()))
             : os << fmt::format("err({})", quote_str(res.unwrap_err()));
}

} // namespace mitama

template <mitama::mutability _, class T, class E>
struct fmt::formatter<mitama::basic_result<_, T, E>> : ostream_formatter
{
};
