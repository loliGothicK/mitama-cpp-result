#ifndef MITAMA_PANIC_HPP
#define MITAMA_PANIC_HPP

#include <stdexcept>
#include <boost/format.hpp>
#include <variant>
#include <utility>
#include <string>
#include <string_view>
#if defined(MITAMA_PANIC_WITH_STACKTRACE) 
  #ifdef _WIN32
  #else
    #define BOOST_STACKTRACE_USE_ADDR2LINE
  #endif
  #include <boost/stacktrace.hpp>
#endif

namespace mitama {

class macro_use_tag_t{};
inline static constexpr macro_use_tag_t macro_use{};

class stacktarce_use_tag_t{};
inline static constexpr stacktarce_use_tag_t stacktarce_use{};

class runtime_panic : public std::runtime_error
{
public:
  template <class... Args>
  runtime_panic(boost::format fmt, Args &&... args) noexcept
      : std::runtime_error((fmt % ... % args).str()) {}

  template <class... Args>
  explicit runtime_panic(macro_use_tag_t, const char *func, int line, std::string fmt, Args &&... args) noexcept
      : std::runtime_error(
            std::string{"runtime panicked at '"} + (boost::format(fmt) % ... % [](auto&& arg) -> decltype(auto) {
              using namespace std::string_view_literals;
              if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::monostate>) {
                return "()"sv;
              }
              else {
                return std::forward<decltype(arg)>(arg);
              }
            }(std::forward<Args>(args))).str() +
            (boost::format("', %1%:%2%") % std::string{func} % line).str()) {}

#if defined(MITAMA_PANIC_WITH_STACKTRACE)
  template <class StackTrace, class... Args>
  explicit runtime_panic(stacktarce_use_tag_t, const char *func, int line, StackTrace&& st, std::string fmt, Args &&... args) noexcept
      : std::runtime_error(
            std::string{"runtime panicked at '"} + (boost::format(fmt) % ... % [](auto&& arg [[maybe_unused]]) -> decltype(auto) {
              using namespace std::string_view_literals;
              if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::monostate>) {
                return "()"sv;
              }
              else {
                return std::forward<decltype(arg)>(arg);
              }
            }(std::forward<Args>(args))).str() +
            (boost::format("', %1%:%2%\n\nstacktrace:\n%3%") % std::string{func} % line % std::forward<StackTrace>(st)).str()) {}
#endif
};
}

#if !defined(MITAMA_PANIC_WITH_STACKTRACE) 
#define PANIC(...) \
  throw ::mitama::runtime_panic { ::mitama::macro_use, __FILE__, __LINE__, __VA_ARGS__ }
#else
#define PANIC(...) \
  throw ::mitama::runtime_panic { ::mitama::stacktarce_use, __FILE__, __LINE__, boost::stacktrace::stacktrace(), __VA_ARGS__ }
#endif

#endif
