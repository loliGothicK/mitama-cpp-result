#pragma once

#include <mitama/mitamagic/format.hpp>

#include <fmt/core.h>
#include <fmt/std.h>
#include <source_location>
#include <stdexcept>
#include <utility>

// FIXME: When we drop fmt < 100200 support, remove this entire block.
#if FMT_VERSION < 100200

/*
 * The following std::source_location formatter implementation is derived from
 * the {fmt} library:
 * https://github.com/fmtlib/fmt
 *
 * Copyright (c) 2012 - present, Victor Zverovich and {fmt} contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * --- Optional exception to the license ---
 *
 * As an exception, if, as a result of your compiling your source code, portions
 * of this Software are embedded into a machine-executable object form of such
 * source code, you may redistribute such embedded portions in such object form
 * without including the above copyright and permission notices.
 *
 * All other code in this file is separately licensed.
 */

template <>
struct formatter<std::source_location>
{
  template <typename ParseContext>
  FMT_CONSTEXPR auto parse(ParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const std::source_location& loc, FormatContext& ctx) const
      -> decltype(ctx.out())
  {
    auto out = ctx.out();
    out = detail::write(out, loc.file_name());
    out = detail::write(out, ':');
    out = detail::write<char>(out, loc.line());
    out = detail::write(out, ':');
    out = detail::write<char>(out, loc.column());
    out = detail::write(out, ": ");
    out = detail::write(out, loc.function_name());
    return out;
  }
};
#endif

namespace mitama
{

class runtime_panic : public std::runtime_error
{
public:
  template <class... Args>
  explicit runtime_panic(
      const std::source_location& loc, fmt::format_string<Args...> f,
      Args&&... args
  ) noexcept
      : std::runtime_error(
            fmt::format(
                "runtime panicked at '{}', {}",
                fmt::format(f, std::forward<Args>(args)...), loc
            )
        )
  {
  }
};

namespace detail
{

  template <class... Args>
  struct runtime_panic_proxy : runtime_panic
  {
    explicit runtime_panic_proxy(
        fmt::format_string<Args...> f, Args&&... args,
        const std::source_location& loc = std::source_location::current()
    ) noexcept
        : runtime_panic(loc, f, std::forward<Args>(args)...)
    {
    }
  };
  template <class... Args>
  runtime_panic_proxy(fmt::format_string<Args...>, Args&&...)
      -> runtime_panic_proxy<Args...>;

} // namespace detail

} // namespace mitama

#define PANIC(...)                            \
  throw ::mitama::detail::runtime_panic_proxy \
  {                                           \
    __VA_ARGS__                               \
  }
