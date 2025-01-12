#pragma once

#include <mitama/mitamagic/format.hpp>

#include <fmt/core.h>
#include <stdexcept>
#include <utility>

namespace mitama
{

class runtime_panic : public std::runtime_error
{
public:
  template <class... Args>
  explicit runtime_panic(
      const char* file, const int line, fmt::format_string<Args...> f,
      Args&&... args
  ) noexcept
      : std::runtime_error(fmt::format(
            "runtime panicked at '{}', {}:{}",
            fmt::format(f, std::forward<Args>(args)...), file, line
        ))
  {
  }
};

} // namespace mitama

#define PANIC(...)                  \
  throw ::mitama::runtime_panic     \
  {                                 \
    __FILE__, __LINE__, __VA_ARGS__ \
  }
