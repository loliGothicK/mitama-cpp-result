#ifndef MITAMA_PANIC_HPP
#define MITAMA_PANIC_HPP

#include <stdexcept>
#include <boost/format.hpp>
#include <variant>
#include <utility>
#include <string>
#include <string_view>

namespace mitama {

class macro_use_tag_t{};
inline static constexpr macro_use_tag_t macro_use{};

class runtime_panic : public std::runtime_error
{
public:
  template <class... Args>
  runtime_panic(boost::format fmt, Args &&... args) noexcept
      : std::runtime_error((fmt % ... % args).str()) {}

  template <class... Args>
  explicit runtime_panic(macro_use_tag_t, const char *func, int line, std::string fmt, Args &&... args) noexcept
      : std::runtime_error(
            std::string{"runtime panicked at '"} + (boost::format(fmt) % ... % [](auto&& arg [[maybe_unused]]){
              using namespace std::string_view_literals;
              if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::monostate>) {
                return "()"sv;
              }
              else {
                return std::forward<decltype(arg)>(arg);
              }
            }(args)).str() +
            (boost::format("', %1%:%2%") % std::string{func} % line).str()) {}
};

}

#define PANIC(...) \
  throw ::mitama::runtime_panic { ::mitama::macro_use, __FILE__, __LINE__, __VA_ARGS__ }

#endif
