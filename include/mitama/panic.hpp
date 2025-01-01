#pragma once

#include <mitama/mitamagic/format.hpp>

#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <variant>

namespace mitama
{

class runtime_panic : public std::runtime_error
{
public:
  template <class... Args>
  explicit runtime_panic(
      const char* file, const int line, std::string_view fmt, Args&&... args
  ) noexcept
      : std::runtime_error(
            std::string{ "runtime panicked at '" }
            + fmt::format(fmt::runtime(fmt), std::forward<Args>(args)...)
            + fmt::format("', {}:{}", file, line)
        )
  {
  }
};

} // namespace mitama

#define PANIC(...)                  \
  throw ::mitama::runtime_panic     \
  {                                 \
    __FILE__, __LINE__, __VA_ARGS__ \
  }
