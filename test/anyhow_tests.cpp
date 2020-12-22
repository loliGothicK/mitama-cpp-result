#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <mitama/result/result.hpp>
#include <mitama/result/result_io.hpp>
#include <mitama/anyhow/anyhow.hpp>

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
