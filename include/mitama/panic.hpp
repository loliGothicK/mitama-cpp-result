#pragma once

#include <boost/format.hpp>

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
  explicit runtime_panic(boost::format fmt, Args&&... args) noexcept
      : std::runtime_error((fmt % ... % std::forward<Args>(args)).str())
  {
  }

  template <class... Args>
  explicit runtime_panic(
      const char* func, int line, std::string fmt, Args&&... args
  ) noexcept
      : std::runtime_error(
          std::string{ "runtime panicked at '" }
          + (boost::format(fmt) % ... % [](auto&& arg) -> decltype(auto)
             {
               using std::string_view_literals::operator""sv;
               if constexpr (std::is_same_v<
                                 std::decay_t<decltype(arg)>, std::monostate>)
               {
                 return "()"sv;
               }
               else
               {
                 return std::forward<decltype(arg)>(arg);
               }
             }(std::forward<Args>(args)))
                .str()
          + (boost::format("', %1%:%2%") % std::string{ func } % line).str()
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
