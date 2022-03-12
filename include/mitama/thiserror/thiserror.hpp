#ifndef MITAMA_THISERROR_THISERROR_HPP
#define MITAMA_THISERROR_THISERROR_HPP
#include <mitama/anyhow/error.hpp>
#include <mitama/mitamagic/format.hpp>

#include <cstddef>
#include <string_view>
#include <tuple>
#include <optional>
#include <iostream>
#include <utility>

#if __cplusplus >= 202002L
#ifndef MITAMA_THISERROR_ENABLE_V1
// Disable v1 by default on C++20
#define MITAMA_THISERROR_ENABLE_V1 false
#endif
#else
// Enforce enabling v1 before C++20
#define MITAMA_THISERROR_ENABLE_V1 true
#endif

#ifdef __cpp_consteval
#define MITAMA_CONSTEVAL_FALLBACK consteval
#else
#define MITAMA_CONSTEVAL_FALLBACK constexpr
#endif

#if MITAMA_THISERROR_ENABLE_V1
#include <boost/metaparse/string.hpp>
#define MITAMA_ERROR(MSG) BOOST_METAPARSE_STRING(MSG)
namespace mitama::thiserror
{
#if __cplusplus >= 202002L
  namespace v1
  {
#else
  inline namespace v1
  {
#endif
    template <class String, class... Sources>
    struct error;

    template <char... String, class... Sources>
    struct error<boost::metaparse::string<String...>, Sources...> final
        : anyhow::error,
          std::enable_shared_from_this<error<boost::metaparse::string<String...>, Sources...>>
    {
    private:
      using Self = error<boost::metaparse::string<String...>, Sources...>;

    public:
      static constexpr const char fmt[] = {String...};
      std::tuple<Sources...> sources;

      explicit error(Sources const &...sources) : sources{sources...} {}

      ~error() override = default;

      // impl Display for thiserror::error
      friend std::ostream &operator<<(std::ostream &os, error const &err)
      {
        return os << std::apply([&](auto &&...src)
                                { return fmt::format(std::string{String...}, std::forward<decltype(src)>(src)...); },
                                err.sources);
      }

      std::shared_ptr<mitama::anyhow::error> context(std::shared_ptr<mitama::anyhow::error> ctx) override
      {
        return std::make_shared<mitama::anyhow::errors>(std::enable_shared_from_this<Self>::shared_from_this(), std::move(ctx));
      }

      std::string what() const override
      {
        std::stringstream ss;
        ss << *this;
        return ss.str();
      }
    };
    struct derive_error
    {
      template <class Msg, class... Sources>
      using error = error<Msg, Sources...>;
    };
  }
}
#endif

#if __cplusplus >= 202002L
namespace mitama::thiserror::inline v2
{
  template <unsigned N>
  struct fixed_string
  {
    char buf[N + 1]{}; // null-terminated string

    // explicitly deleted
    fixed_string() = delete;

    MITAMA_CONSTEVAL_FALLBACK fixed_string(char const *s)
    {
      for (std::size_t i = 0; i != N; ++i)
        buf[i] = s[i];
    }

    // implicit conversion operator
    constexpr operator char const *() const { return buf; }
  };

  // string literal -> fixed_string
  template <std::size_t N>
  fixed_string(char const (&)[N]) -> fixed_string<N - 1>;

  // any string literal in non-template argument
  template <fixed_string Fmt, class... Sources>
  struct error final
      : anyhow::error,
        std::enable_shared_from_this<error<Fmt, Sources...>>
  {
  private:
    using Self = error<Fmt, Sources...>;

  public:
    static constexpr char const *fmt = Fmt;
    std::tuple<Sources...> sources;

    explicit error(Sources const &...sources) : sources{sources...} {}

    ~error() override = default;

    std::shared_ptr<anyhow::error> context(std::shared_ptr<anyhow::error> ctx) override
    {
      return std::make_shared<anyhow::errors>(
          std::enable_shared_from_this<Self>::shared_from_this(),
          std::move(ctx));
    }

    std::string what() const override
    {
      std::stringstream ss;
      ss << *this;
      return ss.str();
    }

    // impl Display for thiserror::error
    friend std::ostream &operator<<(std::ostream &os, error const &err)
    {
      return os << std::apply([&](auto &&...src)
                              { return fmt::format(fmt, std::forward<decltype(src)>(src)...); },
                              err.sources);
    }
  };

  struct derive_error
  {
    template <fixed_string Msg, class... Sources>
    using error = error<Msg, Sources...>;
  };
}
#endif

#if __cplusplus >= 202002L
template <mitama::thiserror::fixed_string Fmt, class... Sources>
#else
template <class Fmt, class... Sources>
#endif
struct fmt::formatter<mitama::thiserror::error<Fmt, Sources...>>
{
  using type = mitama::thiserror::error<Fmt, Sources...>;

  constexpr auto parse(format_parse_context &ctx)
  {
    auto it = ctx.begin(), end = ctx.end();
    // Check if reached the end of the range:
    if (it != end && *it != '}')
    {
      throw format_error(
          fmt::format(
              "invalid format for thiserror: (expected {{}}, found {{:{})",
              std::string_view{it, static_cast<std::size_t>(std::distance(it, end))}));
    }

    // Return an iterator past the end of the parsed range:
    return it;
  }

  template <typename FormatContext>
  auto format(const mitama::thiserror::error<Fmt, Sources...> &err, FormatContext &ctx)
  {
    return std::apply([&ctx](auto const &...sources)
                      { return format_to(ctx.out(), type::fmt, sources...); },
                      err.sources);
  }
};

#undef MITAMA_CONSTEVAL_FALLBACK
#endif
