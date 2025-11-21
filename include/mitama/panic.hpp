#pragma once

#include <mitama/mitamagic/format.hpp>

#include <source_location>
#include <stdexcept>
#include <string>
#include <utility>

namespace mitama
{

class runtime_panic : public std::runtime_error
{
public:
  [[nodiscard]] static std::string
  format_location(const std::source_location& loc)
  {
    return std::format(
        "{}:{}:{}: {}", loc.file_name(), loc.line(), loc.column(),
        loc.function_name()
    );
  }

  template <class... Args>
  explicit runtime_panic(
      const std::source_location& loc, std::format_string<Args...> f,
      Args&&... args
  ) noexcept
      : std::runtime_error(
            std::format(
                "runtime panicked at '{}', {}",
                std::format(f, std::forward<Args>(args)...),
                format_location(loc)
            )
        )
  {
  }
};

namespace detail
{

  template <class... Args>
  [[noreturn]] inline void panic_dispatch(
      const std::source_location& loc, std::format_string<Args...> f,
      Args&&... args
  )
  {
    throw runtime_panic(loc, f, std::forward<Args>(args)...);
  }

  template <class... Args>
    requires(
        !std::is_same_v<std::remove_cvref_t<Args>, std::source_location> && ...
    )
  [[noreturn]] inline void
  panic_dispatch(std::format_string<Args...> f, Args&&... args)
  {
    panic_dispatch(
        std::source_location::current(), f, std::forward<Args>(args)...
    );
  }

  template <class... Args>
  [[noreturn]] inline void panic_dispatch(
      std::format_string<Args...> f, Args&&... args,
      const std::source_location& loc
  )
  {
    panic_dispatch(loc, f, std::forward<Args>(args)...);
  }

} // namespace detail

} // namespace mitama

#define PANIC(...) ::mitama::detail::panic_dispatch(__VA_ARGS__)
