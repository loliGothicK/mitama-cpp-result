#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

inline std::ostream&
operator<<(std::ostream& os, const std::monostate&)
{
  return os << "()";
}
template <class T>
std::ostream&
operator<<(std::ostream& os, const std::unique_ptr<T>& ptr)
{
  if (ptr)
  {
    return os << *ptr;
  }
  else
  {
    return os << "null";
  }
}
template <class T>
std::ostream&
operator<<(std::ostream& os, const std::shared_ptr<T>& ptr)
{
  if (ptr)
  {
    return os << *ptr;
  }
  else
  {
    return os << "null";
  }
}

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

template <>
struct fmt::formatter<std::monostate> : ostream_formatter
{
};
template <class T>
struct fmt::formatter<std::unique_ptr<T>> : ostream_formatter
{
};
template <class T>
struct fmt::formatter<std::shared_ptr<T>> : ostream_formatter
{
};

template <class T>
  requires std::is_convertible_v<std::remove_cvref_t<T>, std::string_view>
std::string
quote_str(T&& val)
{
  return fmt::format("\"{}\"", std::forward<T>(val));
}
template <class T>
constexpr T&&
quote_str(T&& val) noexcept
{
  return std::forward<T>(val);
}
