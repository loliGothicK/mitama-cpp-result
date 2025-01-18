#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <mitama/anyhow/anyhow.hpp>
#include <mitama/result/result.hpp>
#include <mitama/result/result_io.hpp>
#include <mitama/thiserror/thiserror.hpp>

#include <iostream>
#include <string>

namespace anyhow = mitama::anyhow;
using namespace std::literals;

TEST_CASE("anyhow::result from success", "[anyhow][101]")
{
  anyhow::result<int> res = mitama::success(1);
  REQUIRE(res == mitama::success(1));
}

TEST_CASE("anyhow::result from failure", "[anyhow][101]")
{
  {
    const auto res = [](int x) -> anyhow::result<int>
    {
      if (x % 2 != 0)
        return mitama::failure(anyhow::anyhow("invalid input"s));
      return mitama::success(0);
    };
    REQUIRE(res(13).unwrap_err()->what() == "invalid input"s);
  }
  {
    const auto res = [](int x) -> anyhow::result<int>
    {
      if (x % 2 != 0)
        return mitama::failure(anyhow::anyhow("invalid input: {}", x));
      return mitama::success(0);
    };
    REQUIRE(res(13).unwrap_err()->what() == "invalid input: 13"s);
  }
}

TEST_CASE("with context", "[anyhow][context]")
{
  anyhow::result<int> res =
      mitama::failure(anyhow::anyhow("unknown error occurred"s));
  auto ctx = res.with_context([] { return anyhow::anyhow("internal error"); });
  REQUIRE(ctx.is_err());
}

TEST_CASE("try with context", "[anyhow][context]")
{
  auto res = []() -> anyhow::result<int>
  {
    anyhow::result<int> res =
        mitama::failure(anyhow::anyhow("unknown error occurred"s));
    auto ok = MITAMA_TRY(
        res.with_context([] { return anyhow::anyhow("internal error"s); })
    );
    // early return
    REQUIRE(false);
    return mitama::success(ok);
  }();
  REQUIRE(res.is_err());
  std::cout << res << std::endl;
}

struct data_store_error : mitama::thiserror::derive_error
{
  using disconnect = error<"data store disconnected">;
  using redaction = error<"for key `{0}` isn't available", std::string>;
  using invalid_header =
      error<"(expected {0}, found {1})", std::string, std::string>;
  using unknown = error<"unknown data store error">;
};

TEST_CASE("data_store_error::disconnect", "[anyhow][thiserror]")
{
  anyhow::result<int> data = anyhow::failure<data_store_error::disconnect>();
  auto res =
      data.with_context([] { return anyhow::anyhow("data store failed."s); });
  REQUIRE(data.is_err());
  std::cout << res << std::endl;
}

TEST_CASE("data_store_error::redaction", "[anyhow][thiserror]")
{
  anyhow::result<int> data =
      anyhow::failure<data_store_error::redaction>("invalid key");
  auto res =
      data.with_context([] { return anyhow::anyhow("data store failed."s); });
  REQUIRE(data.is_err());
  std::cout << res << std::endl;
}
