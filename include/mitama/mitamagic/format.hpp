#pragma once

#include <format>
#include <memory>
#include <source_location>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

template <class T>
  requires std::is_convertible_v<std::remove_cvref_t<T>, std::string_view>
std::string
quote_str(T&& val)
{
  return std::format("\"{}\"", std::string_view{ std::forward<T>(val) });
}
template <class T>
constexpr T&&
quote_str(T&& val) noexcept
{
  return std::forward<T>(val);
}

namespace std
{
template <>
struct formatter<std::monostate, char>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    return ctx.begin();
  }

  template <class FormatContext>
  auto format(const std::monostate&, FormatContext& ctx) const
  {
    return formatter<string_view, char>{}.format("()", ctx);
  }
};

template <class T>
struct formatter<std::unique_ptr<T>, char>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    return ctx.begin();
  }

  template <class FormatContext>
  auto format(const std::unique_ptr<T>& ptr, FormatContext& ctx) const
  {
    if (ptr)
    {
      return formatter<std::remove_cvref_t<T>, char>{}.format(*ptr, ctx);
    }
    return formatter<string_view, char>{}.format("null", ctx);
  }
};

template <class T>
struct formatter<std::shared_ptr<T>, char>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    return ctx.begin();
  }

  template <class FormatContext>
  auto format(const std::shared_ptr<T>& ptr, FormatContext& ctx) const
  {
    if (ptr)
    {
      return formatter<std::remove_cvref_t<T>, char>{}.format(*ptr, ctx);
    }
    return formatter<string_view, char>{}.format("null", ctx);
  }
};

template <std::size_t N>
struct formatter<const char[N], char>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    return ctx.begin();
  }

  template <class FormatContext>
  auto format(const char (&value)[N], FormatContext& ctx) const
  {
    const auto size = N ? N - 1 : 0;
    return formatter<string_view, char>{}.format(
        std::string_view{ value, size }, ctx
    );
  }
};

template <std::size_t N>
struct formatter<char[N], char> : formatter<const char[N], char>
{
};

template <>
struct formatter<std::source_location, char>
{
  constexpr auto parse(format_parse_context& ctx)
  {
    return ctx.begin();
  }

  template <class FormatContext>
  auto format(const std::source_location& loc, FormatContext& ctx) const
  {
    return formatter<string, char>{}.format(
        std::format(
            "{}:{}:{}: {}", loc.file_name(), loc.line(), loc.column(),
            loc.function_name()
        ),
        ctx
    );
  }
};
} // namespace std
