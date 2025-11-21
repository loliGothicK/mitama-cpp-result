#pragma once

#include <mitama/anyhow/error.hpp>
#include <mitama/mitamagic/format.hpp>

#include <cstddef>
#include <iostream>
#include <memory>
#include <sstream>
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
                 return std::format(fmt, std::forward<decltype(src)>(src)...);
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
struct std::formatter<mitama::thiserror::error<Fmt, Sources...>, char>
    : std::formatter<std::string, char>
{
  using type = mitama::thiserror::error<Fmt, Sources...>;

  template <typename FormatContext>
  auto format(
      const mitama::thiserror::error<Fmt, Sources...>& err, FormatContext& ctx
  )
  {
    auto rendered = std::apply(
        [](const auto&... sources)
        { return std::format(type::fmt, sources...); }, err.sources
    );
    return std::formatter<std::string, char>::format(rendered, ctx);
  }
};

#undef MITAMA_CONSTEVAL_FALLBACK
