#ifndef MITAMA_THISERROR_THISERROR_HPP
#define MITAMA_THISERROR_THISERROR_HPP
#include <mitama/control/version.hpp>
#include <mitama/anyhow/error.hpp>
#include <mitama/mitamagic/format.hpp>

#include <cstddef>
#include <string_view>
#include <tuple>
#include <optional>
#include <iostream>
#include <utility>

#if CPP20_THISERROR
namespace mitama::thiserror {
  template<unsigned N>
  struct fixed_string {
    char buf[N + 1]{}; // null-terminated string

    // explicitly deleted
    fixed_string() = delete;

    // constexpr context only
    consteval fixed_string(char const* s) {
      for (std::size_t i = 0; i != N; ++i) buf[i] = s[i];
    }

    // implicit conversion operator
    constexpr explicit operator char const* () const { return buf; }
    constexpr explicit operator std::string_view() const { return { buf, N + 1 }; }
  };

  // string literal -> fixed_string 
  template<std::size_t N> fixed_string(char const (&)[N])->fixed_string<N - 1>;

  // any string literal in non-template argument
  template<fixed_string Fmt, class ...Sources>
  class error
    : public anyhow::error
    , public std::enable_shared_from_this<error<Fmt, Sources...>>
  {
    using Self = error<Fmt, Sources...>;
  public:
    inline static constinit auto fmt = static_cast<std::string_view>(Fmt);
    std::tuple<Sources...> sources;

    explicit error(Sources const&... sources) : sources{ sources... } {}

    ~error() override = default;

    // impl Display for thiserror::error
    friend std::ostream& operator<<(std::ostream& os, error const& err) {
      return os << err.what();
    }

    std::shared_ptr<mitama::anyhow::error> context(std::shared_ptr<mitama::anyhow::error> ctx) override {
      return std::make_shared<mitama::anyhow::errors>(std::enable_shared_from_this<Self>::shared_from_this(), std::move(ctx));
    }

    std::string what() const override {
        return std::apply([](auto const& ... srcs) { return std::format(fmt, srcs...); }, sources);
    }
  };

  template <fixed_string Fmt, class... Sources>
  struct std::formatter<mitama::thiserror::error<Fmt, Sources...>> : std::formatter<std::string> {
    using type = mitama::thiserror::error<Fmt, Sources...>;

    auto format(type const& err, format_context& ctx) {
      return std::apply([&ctx](auto const& ...sources) {
        return formatter<std::string>::format(std::format(type::fmt, sources...), ctx);
      }, err.sources);
    }
  };
}
#else
#include <boost/metaparse.hpp>
#define MITAMA_ERROR(MSG) BOOST_METAPARSE_STRING(MSG)
namespace mitama::thiserror {
  template <class String, class ...Sources>
  struct error;

  template <char... String, class ...Sources>
  struct error<boost::metaparse::string<String...>, Sources...> final
    : anyhow::error
    , std::enable_shared_from_this<error<boost::metaparse::string<String...>, Sources...>>
  {
  private:
    using Self = error<boost::metaparse::string<String...>, Sources...>;
  public:
    static constexpr const char fmt[] = { String... };
    std::tuple<Sources...> sources;

    explicit error(Sources const&... sources) : sources{ sources... } {}

    ~error() override = default;

    // impl Display for thiserror::error
    friend std::ostream& operator<<(std::ostream& os, error const& err) {
      return os << std::apply([&](auto&&... src) {
        return fmt::format(std::string{ String... }, std::forward<decltype(src)>(src)...);
        }, err.sources);
    }

    std::shared_ptr<mitama::anyhow::error> context(std::shared_ptr<mitama::anyhow::error> ctx) override {
      return std::make_shared<mitama::anyhow::errors>(std::enable_shared_from_this<Self>::shared_from_this(), std::move(ctx));
    }

    std::string what() const override {
      std::stringstream ss;
      ss << *this;
      return ss.str();
    }
  };
}
template <class Fmt, class... Sources>
struct fmt::formatter<mitama::thiserror::error<Fmt, Sources...>> {
  using type = mitama::thiserror::error<Fmt, Sources...>;

  constexpr auto parse(format_parse_context& ctx) {
    auto it = ctx.begin(), end = ctx.end();
    // Check if reached the end of the range:
    if (it != end && *it != '}')
      throw format_error(
        fmt::format("invalid format for thiserror: (expected {{}}, found {{:{})", fmt::join(it, end, "")));

    // Return an iterator past the end of the parsed range:
    return it;
  }
  template <typename FormatContext>
  auto format(const mitama::thiserror::error<Fmt, Sources...>& err, FormatContext& ctx) {
    return std::apply([&ctx](auto const& ...sources) {
      return format_to(ctx.out(), type::fmt, sources...);
      }, err.sources);
  }
};
#endif
#endif
