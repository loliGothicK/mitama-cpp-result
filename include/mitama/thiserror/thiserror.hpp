#pragma once

#include <mitama/anyhow/error.hpp>
#include <mitama/mitamagic/format.hpp>

#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

#ifdef __cpp_consteval
#  define MITAMA_CONSTEVAL_FALLBACK consteval
#else
#  define MITAMA_CONSTEVAL_FALLBACK constexpr
#endif

namespace mitama::thiserror
{
template <unsigned N>
struct fixed_string
{
  char buf[N + 1]{}; // null-terminated string

  // explicitly deleted
  fixed_string() = delete;

  MITAMA_CONSTEVAL_FALLBACK fixed_string(const char* s)
  {
    for (std::size_t i = 0; i != N; ++i)
      buf[i] = s[i];
  }

  // implicit conversion operator
  constexpr operator const char*() const
  {
    return buf;
  }
};

// string literal -> fixed_string
template <std::size_t N>
fixed_string(const char (&)[N]) -> fixed_string<N - 1>;

// any string literal in non-template argument
template <fixed_string Fmt, class... Sources>
struct error final : anyhow::error,
                     std::enable_shared_from_this<error<Fmt, Sources...>>
{
private:
  using Self = error<Fmt, Sources...>;

public:
  static constexpr const char* fmt = Fmt;
  std::tuple<Sources...> sources;

  explicit error(const Sources&... sources) : sources{ sources... } {}

  ~error() override = default;

  std::shared_ptr<anyhow::error>
  context(std::shared_ptr<anyhow::error> ctx) override
  {
    return std::make_shared<anyhow::errors>(
        std::enable_shared_from_this<Self>::shared_from_this(), std::move(ctx)
    );
  }

  std::string what() const override
  {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

  // impl Display for thiserror::error
  friend std::ostream& operator<<(std::ostream& os, const error& err)
  {
    return os << std::apply(
               [&](auto&&... src)
               {
                 return fmt::format(fmt, std::forward<decltype(src)>(src)...);
               },
               err.sources
           );
  }
};

class derive_error
{
protected:
  template <fixed_string Msg, class... Sources>
  using error = error<Msg, Sources...>;
};
} // namespace mitama::thiserror

template <mitama::thiserror::fixed_string Fmt, class... Sources>
struct fmt::formatter<mitama::thiserror::error<Fmt, Sources...>>
{
  using type = mitama::thiserror::error<Fmt, Sources...>;

  constexpr auto parse(format_parse_context& ctx)
  {
    auto it = ctx.begin(), end = ctx.end();
    // Check if reached the end of the range:
    if (it != end && *it != '}')
    {
      throw format_error(
          fmt::format(
              "invalid format for thiserror: (expected {{}}, found {{:{})",
              std::string_view{
                  it, static_cast<std::size_t>(std::distance(it, end)) }
          )
      );
    }

    // Return an iterator past the end of the parsed range:
    return it;
  }

  template <typename FormatContext>
  auto format(
      const mitama::thiserror::error<Fmt, Sources...>& err, FormatContext& ctx
  )
  {
    return std::apply(
        [&ctx](const auto&... sources)
        { return format_to(ctx.out(), type::fmt, sources...); }, err.sources
    );
  }
};

#undef MITAMA_CONSTEVAL_FALLBACK
