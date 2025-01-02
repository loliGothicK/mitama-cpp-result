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
      const char* file, const int line, std::string_view f, Args&&... args
  ) noexcept
      : std::runtime_error(fmt::format(
            "runtime panicked at '{}', {}:{}",
            fmt::format(fmt::runtime(f), std::forward<Args>(args)...), file,
            line
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
