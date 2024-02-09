#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <variant>

std::ostream&
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
