#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <mitama/result/result.hpp>
#include <mitama/result/result_io.hpp>
#include <mitama/anyhow/anyhow.hpp>
#include <mitama/thiserror/thiserror.hpp>

namespace anyhow = mitama::anyhow;
using namespace std::literals;

TEST_CASE("anyhow::result from success", "[anyhow][101]") {
  anyhow::result<int> res = mitama::success(1);
  REQUIRE(res == mitama::success(1));
}

TEST_CASE("anyhow::result from failure", "[anyhow][101]") {
  anyhow::result<int> res = mitama::failure(anyhow::anyhow("error"s));
  REQUIRE(res.is_err());
}

TEST_CASE("with context", "[anyhow][context]") {
  anyhow::result<int> res = mitama::failure(anyhow::anyhow("unknown error occurred"s));
  auto ctx = res.with_context([] { return anyhow::anyhow("internal error"); });
  REQUIRE(ctx.is_err());
}

TEST_CASE("try with context", "[anyhow][context]") {
  auto res = []() -> anyhow::result<int> {
    anyhow::result<int> res = mitama::failure(anyhow::anyhow("unknown error occurred"s));
    auto ok = MITAMA_TRY(res.with_context([]{ return anyhow::anyhow("internal error"s); }));
    // early return
    REQUIRE(false);
    return mitama::success(ok);
  }();
  REQUIRE(res.is_err());
  std::cout << res << std::endl;
}

class data_store_error {
  template <class S, class ...T>
  using error = mitama::thiserror::error<S, T...>;

public:
  using disconnect
    = error<MITAMA_ERROR("data store disconnected")>;
  using redaction
    = error<MITAMA_ERROR("for key `{0}` isn't available"), std::string>;
  using invalid_header
    = error<MITAMA_ERROR("(expected {0}, found {1})"), std::string, std::string>;
  using unknown
    = error<MITAMA_ERROR("unknown data store error")>;

  template <class Source>
  explicit constexpr data_store_error(Source const& src): storage(src) {}

  auto anyhow() const {
    return std::visit([&](auto const& inner) -> std::shared_ptr<anyhow::error> {
      return std::make_shared<std::decay_t<decltype(inner)>>(inner);
    }, storage);
  }

  friend std::ostream& operator<<(std::ostream& os, data_store_error const& self) {
    return os << std::visit(boost::hana::overload(
            [&](disconnect const& err) { return fmt::format("data_store_error(disconnect({})", err); },
            [&](redaction const& err) { return fmt::format("data_store_error(redaction({})", err); },
            [&](invalid_header const& err) { return fmt::format("data_store_error(invalid_header({})", err); },
            [&](unknown const& err) { return fmt::format("data_store_error(unknown({})", err); }
      ), self.storage);
  }
private:
  std::variant<disconnect, redaction, invalid_header, unknown> storage;
};

TEST_CASE("thiserror", "[anyhow][thiserror]") {
  mitama::result<int, data_store_error> data = mitama::failure<data_store_error>(
          data_store_error::invalid_header{"field-name"s, "invalid"s});
  auto res = data
          .map_err(&data_store_error::anyhow)
          .with_context([] { return anyhow::anyhow("data store failed."s); });
  REQUIRE(data.is_err());
  std::cout << res << std::endl;
}
