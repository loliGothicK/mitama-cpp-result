#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <mitama/maybe/maybe.hpp>
#include <mitama/result/result.hpp>
#include <mitama/result/result_io.hpp>

#include <cassert>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <utest_utility/is_invalid_expr.hpp>
#include <utility>
#include <vector>

using namespace mitama;
using namespace std::string_literals;
using namespace std::string_view_literals;

using str = std::string;
using str_ref = std::string_view;
using u32 = std::uint32_t;
using i32 = std::int32_t;
template <class T>
using Rc = std::shared_ptr<T>;

std::vector<std::string>
split_impl(std::string&& s, std::regex&& r)
{
  return { std::sregex_token_iterator{ s.begin(), s.end(), r, -1 },
           std::sregex_token_iterator{} };
}

auto
split(std::string s, std::string pattern)
{
  return split_impl(std::move(s), std::regex{ pattern });
}

template <class T>
auto parse = [](str s) -> result<T, str>
{
  try
  {
    if (std::is_integral_v<T>)
    {
      if (s.empty())
        return failure("parse error at empty string"s);

      for (const char c : s)
        if (c < '0' || '9' < c)
          return failure("parse error at string: "s + s);

      if constexpr (std::is_same_v<T, int>)
      {
        return success(std::stoi(s));
      }
      // NOLINTNEXTLINE(runtime/int)
      else if constexpr (std::is_same_v<T, long>)
      {
        return success(std::stol(s));
      }
      // NOLINTNEXTLINE(runtime/int)
      else if constexpr (std::is_same_v<T, unsigned long>)
      {
        return success(std::stoul(s));
      }
      // NOLINTNEXTLINE(runtime/int)
      else if constexpr (std::is_same_v<T, long long>)
      {
        return success(std::stoll(s));
      }
      // NOLINTNEXTLINE(runtime/int)
      else if constexpr (std::is_same_v<T, unsigned long long>)
      {
        return success(std::stoull(s));
      }
    }
    else if constexpr (std::is_floating_point_v<T>)
    {
      if constexpr (std::is_same_v<T, float>)
      {
        return success(std::stof(s));
      }
      else if constexpr (std::is_same_v<T, double>)
      {
        return success(std::stod(s));
      }
      else if constexpr (std::is_same_v<T, long double>)
      {
        return success(std::stold(s));
      }
    }
    else
    {
      static_assert(mitama::always_true_v<T>);
    }
  }
  catch (const std::invalid_argument& e)
  {
    return failure(e.what());
  }
};

TEST_CASE("is_ok() test", "[result][is_ok]")
{
  constexpr result<u32, str_ref> x = success(-3);
  static_assert(x.is_ok() == true);

  constexpr result<u32, str_ref> y = failure("some error message"sv);
  static_assert(y.is_ok() == false);
}

TEST_CASE("is_err() test", "[result][is_err]")
{
  constexpr result<u32, str_ref> x = success(-3);
  static_assert(x.is_err() == false);

  constexpr result<u32, str_ref> y = failure("some error message"sv);
  static_assert(y.is_err() == true);
}

TEST_CASE("ok() test", "[result][ok]")
{
  constexpr result<u32, str_ref> x = success(2);
  static_assert(x.ok() == just(2u));

  constexpr result<int, str_ref> y = failure("Nothing here"sv);
  static_assert(y.ok() == nothing);
}

TEST_CASE("err() test", "[result][err]")
{
  constexpr result<u32, str_ref> x = success(2u);
  static_assert(x.err() == nothing);

  constexpr result<u32, str_ref> y = failure("Nothing here"sv);
  static_assert(y.err() == just("Nothing here"sv));
}

TEST_CASE("map() test", "[result][map]")
{
  std::string line = "1,3,5,7";

  for (auto num : split(line, ","))
  {
    auto res = parse<i32>(num).map([](i32 x) { return x * 2; });
    REQUIRE(res.is_ok() == true);
    REQUIRE(res.ok().unwrap() % 2 == 0);
  }

  result<int, int> some_num = success(1);
  result<int, int> z = some_num.map(std::plus{}, 1);
  REQUIRE(z == success(2));
}

TEST_CASE("map_apply() test", "[result][map_apply]")
{
  result<std::tuple<int, int>, int> res = success(1, 2);
  result<int, int> z = res.map_apply(std::plus<>{});
  REQUIRE(z == success(3));
}

TEST_CASE("map_or_else(F, M) test", "[result][map_or_else]")
{
  auto k = 21;
  {
    result<str, str> x = success("foo"s);
    REQUIRE(
        x.map_or_else(
            [k](auto) { return k * 2; }, [](auto v) { return v.length(); }
        )
        == 3
    );
  }
  {
    result<str, str> x = failure("bar"s);
    REQUIRE(
        x.map_or_else(
            [k](auto) { return k * 2; }, [](auto v) { return v.length(); }
        )
        == 42
    );
  }
}

TEST_CASE("map_anything_else(F) test", "[result][map_anything_else]")
{
  {
    constexpr result<str_ref, str_ref> x = success("foo"sv);
    static_assert(x.map_anything_else([](auto v) { return v.length(); }) == 3);
  }
  {
    constexpr result<str_ref, str_ref> x = failure("bar"sv);
    static_assert(x.map_anything_else([](auto v) { return v.length(); }) == 3);
  }
}

TEST_CASE("map_err() test", "[result][map_err]")
{
  auto stringify = [](u32 x) -> str
  { return "error code: "s + std::to_string(x); };

  result<u32, u32> x = success(2);
  REQUIRE(x.map_err(stringify) == success(2u));

  result<u32, u32> y = failure(13);
  REQUIRE(y.map_err(stringify) == failure("error code: 13"s));

  result<int, int> some_num = failure(1);
  result<int, int> z = some_num.map_err(std::plus{}, 1);
  REQUIRE(z == failure(2));
}

TEST_CASE("map_apply_err() test", "[result][map_apply_err]")
{
  result<int, std::tuple<int, int>> res = failure(1, 1);
  result<int, int> z =
      res.map_apply_err([](auto... a) { return (... + a); }, 1);
  REQUIRE(z == failure(3));
}

TEST_CASE("conj test", "[result][conj]")
{
  {
    constexpr result<u32, str_ref> x = success(2);
    constexpr result<str_ref, str_ref> y = failure("late error"sv);
    static_assert(x.conj(y) == failure("late error"sv));
    static_assert((x && y) == failure("late error"sv));
  }

  {
    constexpr result<u32, str_ref> x = failure("early error"sv);
    constexpr result<str_ref, str_ref> y = success("foo"sv);
    static_assert(x.conj(y) == failure("early error"sv));
    static_assert((x && y) == failure("early error"sv));
  }
  {
    constexpr result<u32, str_ref> x = failure("not a 2"sv);
    constexpr result<str_ref, str_ref> y = failure("late error"sv);
    static_assert(x.conj(y) == failure("not a 2"sv));
    static_assert((x && y) == failure("not a 2"sv));
  }

  {
    constexpr result<u32, str_ref> x = success(2);
    constexpr result<str_ref, str_ref> y = success("different result type"sv);
    static_assert(x.conj(y) == success("different result type"sv));
    static_assert((x && y) == success("different result type"sv));
  }
}

TEST_CASE("and_then() test", "[result][and_then]")
{
  constexpr auto sq = [](u32 x) -> result<u32, u32> { return success(x * x); };
  constexpr auto err = [](u32 x) -> result<u32, u32> { return failure(x); };
  constexpr auto eq = [](u32 x, u32 y) -> result<u32, u32>
  { return x == y ? result<u32, u32>(success(x)) : failure(x); };

  static_assert(
      result<u32, u32>{ success(2u) }.and_then(sq).and_then(sq) == success(16u)
  );
  static_assert(
      result<u32, u32>{ success(2u) }.and_then(sq).and_then(err) == failure(4u)
  );
  static_assert(
      result<u32, u32>{ success(2u) }.and_then(err).and_then(sq) == failure(2u)
  );
  static_assert(
      result<u32, u32>{ failure(3u) }.and_then(sq).and_then(sq) == failure(3u)
  );
  static_assert(
      result<u32, u32>{ failure(3u) }.and_then(sq).and_then(sq) == failure(3u)
  );
  static_assert(
      result<u32, u32>{ success(3u) }.and_then(eq, 3u) == success(3u)
  );
  static_assert(
      result<u32, u32>{ failure(3u) }.and_then(eq, 1u) == failure(3u)
  );
}

TEST_CASE("and_then_apply() test", "[result][and_then_apply]")
{
  auto fn = [](u32 x, u32 y) -> result<u32, u32>
  {
    if ((x + y) % 2 == 0)
      return success(x + y);
    else
      return failure(x + y);
  };
  result<std::tuple<u32, u32>, u32> ok1 = success(1u, 2u);
  result<std::tuple<u32, u32>, u32> ok2 = success(1u, 1u);
  result<std::tuple<u32, u32>, u32> err = failure(1u);
  REQUIRE(ok1.and_then_apply(fn) == failure(3u));
  REQUIRE(ok2.and_then_apply(fn) == success(2u));
  REQUIRE(err.and_then_apply(fn) == failure(1u));
}

TEST_CASE("or_else_apply() test", "[result][or_else_apply]")
{
  auto fn = [](u32 x, u32 y) -> result<u32, u32>
  {
    if ((x + y) % 2 == 0)
      return success(x + y);
    else
      return failure(x + y);
  };
  result<u32, std::tuple<u32, u32>> ok = success(1u);
  result<u32, std::tuple<u32, u32>> err1 = failure(1u, 1u);
  result<u32, std::tuple<u32, u32>> err2 = failure(1u, 2u);
  REQUIRE(ok.or_else_apply(fn) == success(1u));
  REQUIRE(err1.or_else_apply(fn) == success(2u));
  REQUIRE(err2.or_else_apply(fn) == failure(3u));
}

TEMPLATE_TEST_CASE("is_convertible_result_with meta test", "[is_convertible_result_with][meta]",
                    int, unsigned, std::string, std::vector<int>)
{
  static_assert(is_convertible_result_with_v<
                mitama::result<int, TestType>, mitama::failure_t<TestType>>);
  static_assert(!is_convertible_result_with_v<
                result<unsigned, std::vector<TestType>>,
                mitama::failure_t<TestType>>);
}

TEST_CASE("disj test", "[result][disj]")
{
  {
    constexpr result<u32, str_ref> x = success(2);
    constexpr result<u32, str_ref> y = failure("late error"sv);
    static_assert(x.disj(y) == success(2u));
    static_assert((x || y) == success(2u));
  }
  {
    constexpr result<u32, str_ref> x = failure("early error"sv);
    constexpr result<u32, str_ref> y = success(2);
    static_assert(x.disj(y) == success(2u));
    static_assert((x || y) == success(2u));
  }
  {
    constexpr result<u32, str_ref> x = failure("not a 2"sv);
    constexpr result<u32, str_ref> y = failure("late error"sv);
    static_assert(x.disj(y) == failure("late error"sv));
    static_assert((x || y) == failure("late error"sv));
  }
  {
    constexpr result<u32, str_ref> x = success(2);
    constexpr result<u32, str_ref> y = success(100);
    static_assert(x.disj(y) == success(2u));
    static_assert((x || y) == success(2u));
  }
}

TEST_CASE("or_else() test", "[result][or_else]")
{
  constexpr auto sq = [](u32 x) -> result<u32, u32> { return success(x * x); };
  constexpr auto err = [](u32 x) -> result<u32, u32> { return failure(x); };
  constexpr auto eq = [](u32 x, u32 y) -> result<u32, u32>
  { return x == y ? result<u32, u32>(success(x)) : failure(x); };

  static_assert(
      result<u32, u32>{ success(2) }.or_else(sq).or_else(sq) == success(2u)
  );
  static_assert(
      result<u32, u32>{ success(2) }.or_else(err).or_else(sq) == success(2u)
  );
  static_assert(
      result<u32, u32>{ failure(3) }.or_else(sq).or_else(err) == success(9u)
  );
  static_assert(
      result<u32, u32>{ failure(3) }.or_else(err).or_else(err) == failure(3u)
  );
  static_assert(result<u32, u32>{ failure(3) }.or_else(eq, 3u) == success(3u));
  static_assert(result<u32, u32>{ success(3) }.or_else(eq, 3u) == success(3u));
  static_assert(result<u32, u32>{ failure(3) }.or_else(eq, 1u) == failure(3u));
}

TEST_CASE("unwrap_or() test", "[result][unwrap_or]")
{
  constexpr result<u32, u32> err = failure(2);
  constexpr result<u32, u32> ok = success(2);

  static_assert(ok.unwrap_or(1u) == 2u);
  static_assert(err.unwrap_or(1u) == 1u);
}

TEST_CASE("unwrap_or_else() test", "[result][unwrap_or_else]")
{
  constexpr auto count = [](str_ref x) -> size_t { return x.size(); };

  static_assert(result<u32, str_ref>{ success(2) }.unwrap_or_else(count) == 2);
  static_assert(
      result<u32, str_ref>{ failure("foo"sv) }.unwrap_or_else(count) == 3ull
  );
  static_assert(
      result<u32, str_ref>{ failure("foo"sv) }.unwrap_or_else([]
                                                              { return 3ull; })
      == 3ull
  );
}

TEST_CASE("unwrap() test", "[result][unwrap]")
{
  {
    constexpr result<u32, str_ref> x = success(2);
    static_assert(x.unwrap() == 2u);
  }
  try
  {
    result<u32, str> x = failure("emergency failure"s);
    x.unwrap(); // panics with `emergency failure`
  }
  catch (const runtime_panic& p)
  {
    const std::string_view expected =
        "runtime panicked at 'called `basic_result::unwrap()` on a "
        "value: `failure(\"emergency failure\")`', ";

    const std::string_view what = p.what();
    REQUIRE(what.starts_with(expected));

    std::size_t i = expected.size();
    for (; i < what.size() && what[i] != ':'; ++i)
      ;
    REQUIRE(i++ != what.size());
    REQUIRE((what[i] >= '0' && what[i] <= '9'));
  }
}

TEST_CASE("unwrap_err() test", "[result][unwrap_err]")
{
  try
  {
    result<u32, str> x = success(2);
    x.unwrap_err(); // panics with `2`
  }
  catch (const runtime_panic& p)
  {
    const std::string_view expected =
        R"(runtime panicked at 'called `basic_result::unwrap_err()` on a value: `success(2)`', )";

    const std::string_view what = p.what();
    REQUIRE(what.starts_with(expected));

    std::size_t i = expected.size();
    for (; i < what.size() && what[i] != ':'; ++i)
      ;
    REQUIRE(i++ != what.size());
    REQUIRE((what[i] >= '0' && what[i] <= '9'));
  }

  {
    constexpr result<u32, str_ref> x = failure("emergency failure"sv);
    static_assert(x.unwrap_err() == "emergency failure"sv);
  }
}

TEST_CASE("unwrap_or_default() test", "[result][unwrap_or_default]")
{
  auto good_year_from_input = "1909"s;
  auto bad_year_from_input = "190blarg"s;
  auto good_year = parse<int>(good_year_from_input).unwrap_or_default();
  auto bad_year = parse<int>(bad_year_from_input).unwrap_or_default();

  REQUIRE(1909 == good_year);
  REQUIRE(0 == bad_year);
}

TEST_CASE("transpose() test", "[result][transpose]")
{
  constexpr result<maybe<i32>, void> x = success(just(5));
  constexpr maybe<result<i32, void>> y = just(success(5));

  static_assert(x.transpose() == y);
}

TEST_CASE("and_finally() test", "[result][and_finally]")
{
  int hook = 0;
  result<int, std::string> x = failure("error"s);
  x.and_finally([&hook](int v) { hook = v; });
  REQUIRE(hook == 0);

  result<int, std::string> y = success(1);
  y.and_finally([&hook](int v) { hook = v; });
  REQUIRE(hook == 1);
}

TEST_CASE("or_finally() test", "[result][or_finally]")
{
  std::string hook = "default";
  result<int, std::string> x = success(42);
  x.or_finally([&hook](std::string v) { hook = v; });
  REQUIRE(hook == "default"s);

  result<int, std::string> y = failure("error"s);
  y.or_finally([&hook](std::string v) { hook = v; });
  REQUIRE(hook == "error"s);
}

TEST_CASE("and_peek() test", "[result][and_peek]")
{
  mut_result<int, std::string> x = success(42);
  REQUIRE(x.and_peek([](int& v) { v = 57; }) == success(57));
}

TEST_CASE("or_peek() test", "[result][and_peek]")
{
  mut_result<int, std::string> x = failure("foo"s);
  REQUIRE(x.or_peek([](std::string& v) { v = "bar"; }) == failure("bar"s));
}

TEST_CASE("basics test", "[result][basics]")
{
  auto even = [](u32 u) -> result<u32, str>
  {
    if (u % 2 == 0)
      return success(u);
    else
      return failure("odd"s);
  };
  auto func = [](auto u) -> result<u32, str>
  {
    if (u % 3 == 0)
      return success(1u);
    else
      return failure("error"s);
  };
  REQUIRE(even(2).and_then(func) == failure("error"s));
  REQUIRE(even(2) == success(2u));
}

TEST_CASE(
    "constructors and assignments test", "[result][constructors][assignments]"
)
{
  {
    auto res = result<int, int>{ success(2) };
    res = result<int, int>{ failure(2) };
    (void)result<std::string, double>{ in_place_ok, "hoge" };
    (void)result<double, std::string>{ in_place_err, "hoge" };
    (void)result<std::vector<int>, double>{ in_place_ok, { 1, 2, 3, 4 } };
    (void)result<double, std::vector<double>>{ in_place_err,
                                               { 1., 2., 3., 4. } };
  }
  {
    auto res = mut_result<int, int>{ success(2) };
    res = success(1);
    res = failure(2);
  }
}

TEST_CASE("format test", "[result][format]")
{
  SECTION("success")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << success(1);
    REQUIRE(ss.str() == "success(1)"s);
  }
  SECTION("failure")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << failure(1);
    REQUIRE(ss.str() == "failure(1)"s);
  }
  SECTION("result ok")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << result<int, std::string>{ success(1) };
    REQUIRE(ss.str() == "success(1)"s);
  }
  SECTION("result err")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << result<int, std::string>{ failure("hoge"s) };
    REQUIRE(ss.str() == "failure(\"hoge\")"s);
  }
  SECTION("result of void ok")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << result<>{ success() };
    REQUIRE(ss.str() == "success(())"s);
  }
  SECTION("result of void err")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << result<>{ failure() };
    REQUIRE(ss.str() == "failure(())"s);
  }
  SECTION("result of range ok")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << success(std::vector<std::string>{ "foo"s, "bar"s });
    REQUIRE(ss.str() == "success([\"foo\", \"bar\"])"s);
  }
  SECTION("result of range err")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << failure(std::vector<std::string>{ "foo"s, "bar"s });
    REQUIRE(ss.str() == "failure([\"foo\", \"bar\"])"s);
  }
  SECTION("success of tuple")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << success(std::tuple{ "foo"s, 1 });
    REQUIRE(ss.str() == "success((\"foo\", 1))"s);
  }
  SECTION("failure of tuple")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << failure(std::tuple{ "foo"s, 1 });
    REQUIRE(ss.str() == "failure((\"foo\", 1))"s);
  }
  SECTION("result of tuple")
  {
    using namespace std::literals;
    {
      std::stringstream ss;
      ss << result<std::tuple<str, int>, int>(success(std::tuple{ "foo"s, 1 }));
      REQUIRE(ss.str() == "success((\"foo\", 1))"s);
    }
    {
      std::stringstream ss;
      ss << result<int, std::tuple<str, int>>(failure(std::tuple{ "foo"s, 1 }));
      REQUIRE(ss.str() == "failure((\"foo\", 1))"s);
    }
  }
  SECTION("result of dictionary")
  {
    using namespace std::literals;
    {
      std::stringstream ss;
      ss << result<std::map<str, int>, int>(success(std::map<str, int>{
          { "foo"s, 1 } }));
      REQUIRE(ss.str() == "success({\"foo\": 1})"s);
    }
  }
  SECTION("result of tuple of tuple")
  {
    using namespace std::literals;
    {
      std::stringstream ss;
      ss << result<std::tuple<std::tuple<int, int>, int>, int>(
          success(std::tuple{ std::tuple{ 1, 1 }, 1 })
      );
      REQUIRE(ss.str() == "success(((1, 1), 1))"s);
    }
  }
  SECTION("result of range of tuple")
  {
    using namespace std::literals;
    {
      std::stringstream ss;
      ss << result<std::vector<std::tuple<int, int>>, int>(success(std::vector{
          std::tuple{ 1, 1 }, std::tuple{ 1, 1 } }));
      REQUIRE(ss.str() == "success([(1, 1), (1, 1)])"s);
    }
  }
  SECTION("failure")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << failure("foo"s);
    REQUIRE(ss.str() == "failure(\"foo\")"s);
  }
  SECTION("failure of string_view")
  {
    using namespace std::literals;
    std::stringstream ss;
    constexpr std::string_view foo = "foo";
    ss << failure(foo);
    REQUIRE(ss.str() == "failure(\"foo\")"s);
  }
  SECTION("failure of char*")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << failure("foo");
    REQUIRE(ss.str() == "failure(\"foo\")"s);
  }
  SECTION("replace")
  {
    using namespace std::literals;
    auto res = mut_result<int, std::vector<int>>{ in_place_err, { 1, 2, 3 } };
    REQUIRE(fmt::format("{}", res) == "failure([1, 2, 3])"s);
    res = success(1);
    REQUIRE(fmt::format("{}", res) == "success(1)"s);
  }
}

TEST_CASE("monostate success test", "[result][monostate]")
{
  constexpr auto func = []() -> result<void, str_ref>
  {
    if (false)
      return failure<str_ref>("hoge"sv);
    return success<>();
  };

  static_assert(func().is_ok());
}

TEST_CASE("monostate failure test", "[result][monostate]")
{
  constexpr auto func = []() -> result<void, void>
  {
    if (false)
      return success<>();
    return failure<>();
  };
  static_assert(func().is_err());
}

TEST_CASE("contextually convertible to bool", "[result]")
{
  auto err_func = []() -> result</*defaulted monostate*/>
  {
    if (false)
      return failure<>();
    return failure<>();
  };
  auto ok_func = []() -> result<void, std::string>
  {
    if (false)
      return failure<std::string>("hoge"s);
    return success<>();
  };
  REQUIRE(!err_func());
  REQUIRE(ok_func());
}

SCENARIO("test for reference type", "[result][ref]")
{
  using namespace std::literals;
  GIVEN("A result that refer to some string")
  {
    str hoge = "foo";
    mut_result<str&, str&> res(success<str&>(hoge));

    REQUIRE(hoge == "foo"s);
    REQUIRE(res.unwrap() == "foo"s);

    WHEN("the result is overwritten")
    {
      res.unwrap() = "bar";

      THEN("the referred string change")
      {
        REQUIRE(hoge == "bar"s);
        REQUIRE(res.unwrap() == "bar"s);
      }
    }
  }
}

SCENARIO("test for as_ref", "[result][as_ref]")
{
  using namespace std::literals;
  GIVEN("A new result, containing a reference into the original")
  {
    mut_result<str, str> res(success<str>("foo"s));
    auto ref /* mut_result<str&, str&> */ = res.as_ref();

    REQUIRE(res == ref);
    REQUIRE(res == success("foo"s));
    REQUIRE(ref == success("foo"s));
  }
}

SCENARIO("test for as_mut", "[result][as_mut]")
{
  using namespace std::literals;
  GIVEN("A new result, containing a reference into the original")
  {
    auto ptr = std::make_shared<str>("foo"s);
    mut_result<Rc<str>, Rc<str>> res(success<Rc<str>>(ptr));
    auto ref /* result<str const&, str const&> */ = res.as_mut();

    REQUIRE(res == success(ptr));
    REQUIRE(ref == success(ptr));

    WHEN("The new result is overwritten")
    {
      *ref.unwrap() = "bar"s;

      THEN("the original result change")
      {
        REQUIRE(res == ref);
        REQUIRE(*res.unwrap() == "bar"s);
        REQUIRE(*ref.unwrap() == "bar"s);
      }
    }
  }
}

SCENARIO("test for indirect", "[result][indirect]")
{
  using namespace std::literals;
  using vec_iter = typename std::vector<int>::iterator;

  GIVEN("A new result, containing a indirect reference into the original")
  {
    std::vector<int> vec{ 1, 2, 3 };
    mut_result<vec_iter, vec_iter> res(success<vec_iter>(vec.begin()));
    auto indirect = res.indirect();

    REQUIRE(*res.unwrap() == indirect.unwrap());
    REQUIRE(indirect == success(1));

    WHEN("The new result is overwritten")
    {
      auto& ref = indirect.unwrap();
      ref = 42;

      THEN("the original result change")
      {
        REQUIRE(vec[0] == 42);
        REQUIRE(indirect == success(42));
      }
    }
  }
}

SCENARIO("test for dangling indirect", "[result][indirect][dangling]")
{
  using namespace std::literals;
  using vec_iter = typename std::vector<int>::iterator;
  GIVEN(
      "A new result which is containing a dangling reference into the "
      "discarded unique_ptr"
  )
  {
    auto indirect = mut_result<std::unique_ptr<int>, std::unique_ptr<int>>(
                        success(std::make_unique<int>(1))
    )
                        .as_ref()
                        .indirect();
    REQUIRE(std::is_same_v<
            decltype(indirect.unwrap()),
            dangling<std::reference_wrapper<int>>&>);
    // indirect.unwrap().transmute()
    // ^~~~~~~~~~~~~~~~~~~~~~~~~~ Undefined Behavior!
  }
  GIVEN("A new result which is containing a reference into the living vector")
  {
    std::vector<int> vec{ 1, 3 };
    auto indirect = result<vec_iter, vec_iter>(success(vec.begin())).indirect();

    REQUIRE(indirect.unwrap().transmute() == 1);
    //       ^~~~~~~~~~~~~~~~~~~~~~~~~~ OK!
  }
}

struct incomplete_type;
incomplete_type& get_incomplete_type();
template <class T, class = void>
struct is_complete_type : std::false_type
{
};
template <class T>
struct is_complete_type<T, std::void_t<decltype(sizeof(T))>> : std::true_type
{
};

TEST_CASE("incomplete type reference", "[result]")
{
  static_assert(!is_complete_type<incomplete_type>::value);
  [[maybe_unused]] result<incomplete_type&> res =
      success<incomplete_type&>(get_incomplete_type()
      ); // use incomplete_type& for result
}

struct incomplete_type
{
};

incomplete_type&
get_incomplete_type()
{
  static incomplete_type obj = incomplete_type{};
  return obj;
}

TEST_CASE("less compare", "[result][less]")
{
  constexpr result<int, int> ok1 = success(1);
  constexpr result<int, int> ok2 = success(2);
  constexpr result<int, int> err1 = failure(1);
  constexpr result<int, int> err2 = failure(2);

  static_assert(ok1 < ok2);
  static_assert(!(ok2 < ok1));
  static_assert(!(ok1 < ok1));
  static_assert(!(ok2 < ok2));

  static_assert(success(1) < ok2);
  static_assert(!(success(2) < ok1));
  static_assert(!(success(1) < ok1));
  static_assert(!(success(2) < ok2));

  static_assert(ok1 < success(2));
  static_assert(!(ok2 < success(1)));
  static_assert(!(ok1 < success(1)));
  static_assert(!(ok2 < success(2)));

  static_assert(success(1) < success(2));
  static_assert(!(success(2) < success(1)));
  static_assert(!(success(1) < success(1)));
  static_assert(!(success(2) < success(2)));

  static_assert(ok1 < 2);
  static_assert(!(ok2 < 1));
  static_assert(!(ok1 < 1));
  static_assert(!(ok2 < 2));

  static_assert(1 < ok2);
  static_assert(!(2 < ok1));
  static_assert(!(1 < ok1));
  static_assert(!(2 < ok2));

  static_assert(err1 < err2);
  static_assert(!(err2 < err1));
  static_assert(!(err1 < err1));
  static_assert(!(err2 < err2));

  static_assert(failure(1) < err2);
  static_assert(!(failure(2) < err1));
  static_assert(!(failure(1) < err1));
  static_assert(!(failure(2) < err2));

  static_assert(err1 < failure(2));
  static_assert(!(err2 < failure(1)));
  static_assert(!(err1 < failure(1)));
  static_assert(!(err2 < failure(2)));

  static_assert(failure(1) < failure(2));
  static_assert(!(failure(2) < failure(1)));
  static_assert(!(failure(1) < failure(1)));
  static_assert(!(failure(2) < failure(2)));

  static_assert(err1 < ok1);
  static_assert(err1 < ok2);
  static_assert(err2 < ok1);
  static_assert(err2 < ok2);

  static_assert(failure(1) < ok1);
  static_assert(failure(1) < ok2);
  static_assert(failure(2) < ok1);
  static_assert(failure(2) < ok2);

  static_assert(err1 < success(1));
  static_assert(err1 < success(2));
  static_assert(err2 < success(1));
  static_assert(err2 < success(2));

  static_assert(failure(1) < success(1));
  static_assert(failure(1) < success(2));
  static_assert(failure(2) < success(1));
  static_assert(failure(2) < success(2));

  static_assert(!(ok1 < err1));
  static_assert(!(ok1 < err2));
  static_assert(!(ok2 < err1));
  static_assert(!(ok2 < err2));

  static_assert(!(success(1) < err1));
  static_assert(!(success(1) < err2));
  static_assert(!(success(2) < err1));
  static_assert(!(success(2) < err2));

  static_assert(!(ok1 < failure(1)));
  static_assert(!(ok1 < failure(2)));
  static_assert(!(ok2 < failure(1)));
  static_assert(!(ok2 < failure(2)));

  static_assert(!(success(1) < failure(1)));
  static_assert(!(success(1) < failure(2)));
  static_assert(!(success(2) < failure(1)));
  static_assert(!(success(2) < failure(2)));

  static_assert(!(1 < err1));
  static_assert(!(1 < err2));
  static_assert(!(2 < err1));
  static_assert(!(2 < err2));
}

TEST_CASE("less_or_equal compare", "[result][less_or_equal]")
{
  constexpr result<int, int> ok1 = success(1);
  constexpr result<int, int> ok2 = success(2);
  constexpr result<int, int> err1 = failure(1);
  constexpr result<int, int> err2 = failure(2);

  static_assert(ok1 <= ok2);
  static_assert(!(ok2 <= ok1));
  static_assert(ok1 <= ok1);
  static_assert(ok2 <= ok2);

  static_assert(success(1) <= ok2);
  static_assert(!(success(2) <= ok1));
  static_assert(success(1) <= ok1);
  static_assert(success(2) <= ok2);

  static_assert(ok1 <= success(2));
  static_assert(!(ok2 <= success(1)));
  static_assert(ok1 <= success(1));
  static_assert(ok2 <= success(2));

  static_assert(success(1) <= success(2));
  static_assert(!(success(2) <= success(1)));
  static_assert(success(1) <= success(1));
  static_assert(success(2) <= success(2));

  static_assert(1 <= ok2);
  static_assert(!(2 <= ok1));
  static_assert(1 <= ok1);
  static_assert(2 <= ok2);

  static_assert(ok1 <= 2);
  static_assert(!(ok2 <= 1));
  static_assert(ok1 <= 1);
  static_assert(ok2 <= 2);

  static_assert(err1 <= err2);
  static_assert(!(err2 <= err1));
  static_assert(err1 <= err1);
  static_assert(err2 <= err2);

  static_assert(failure(1) <= err2);
  static_assert(!(failure(2) <= err1));
  static_assert(failure(1) <= err1);
  static_assert(failure(2) <= err2);

  static_assert(err1 <= failure(2));
  static_assert(!(err2 <= failure(1)));
  static_assert(err1 <= failure(1));
  static_assert(err2 <= failure(2));

  static_assert(failure(1) <= failure(2));
  static_assert(!(failure(2) <= failure(1)));
  static_assert(failure(1) <= failure(1));
  static_assert(failure(2) <= failure(2));

  static_assert(err1 <= ok1);
  static_assert(err1 <= ok2);
  static_assert(err2 <= ok1);
  static_assert(err2 <= ok2);

  static_assert(failure(1) <= ok1);
  static_assert(failure(1) <= ok2);
  static_assert(failure(2) <= ok1);
  static_assert(failure(2) <= ok2);

  static_assert(err1 <= success(1));
  static_assert(err1 <= success(2));
  static_assert(err2 <= success(1));
  static_assert(err2 <= success(2));

  static_assert(failure(1) <= success(1));
  static_assert(failure(1) <= success(2));
  static_assert(failure(2) <= success(1));
  static_assert(failure(2) <= success(2));

  static_assert(err1 <= 1);
  static_assert(err1 <= 2);
  static_assert(err2 <= 1);
  static_assert(err2 <= 2);

  static_assert(!(ok1 <= err1));
  static_assert(!(ok1 <= err2));
  static_assert(!(ok2 <= err1));
  static_assert(!(ok2 <= err2));

  static_assert(!(success(1) <= err1));
  static_assert(!(success(1) <= err2));
  static_assert(!(success(2) <= err1));
  static_assert(!(success(2) <= err2));

  static_assert(!(ok1 <= failure(1)));
  static_assert(!(ok1 <= failure(2)));
  static_assert(!(ok2 <= failure(1)));
  static_assert(!(ok2 <= failure(2)));

  static_assert(!(success(1) <= failure(1)));
  static_assert(!(success(1) <= failure(2)));
  static_assert(!(success(2) <= failure(1)));
  static_assert(!(success(2) <= failure(2)));

  static_assert(!(1 <= err1));
  static_assert(!(1 <= err2));
  static_assert(!(2 <= err1));
  static_assert(!(2 <= err2));
}

TEST_CASE("greater compare", "[result][greater]")
{
  constexpr result<int, int> ok1 = success(1);
  constexpr result<int, int> ok2 = success(2);
  constexpr result<int, int> err1 = failure(1);
  constexpr result<int, int> err2 = failure(2);

  static_assert(!(ok1 > ok2));
  static_assert(ok2 > ok1);
  static_assert(!(ok1 > ok1));
  static_assert(!(ok2 > ok2));

  static_assert(!(success(1) > ok2));
  static_assert(success(2) > ok1);
  static_assert(!(success(1) > ok1));
  static_assert(!(success(2) > ok2));

  static_assert(!(ok1 > success(2)));
  static_assert(ok2 > success(1));
  static_assert(!(ok1 > success(1)));
  static_assert(!(ok2 > success(2)));

  static_assert(!(success(1) > success(2)));
  static_assert(success(2) > success(1));
  static_assert(!(success(1) > success(1)));
  static_assert(!(success(2) > success(2)));

  static_assert(!(1 > ok2));
  static_assert(2 > ok1);
  static_assert(!(1 > ok1));
  static_assert(!(2 > ok2));

  static_assert(!(ok1 > 2));
  static_assert(ok2 > 1);
  static_assert(!(ok1 > 1));
  static_assert(!(ok2 > 2));

  static_assert(!(err1 > err2));
  static_assert(err2 > err1);
  static_assert(!(err1 > err1));
  static_assert(!(err2 > err2));

  static_assert(!(failure(1) > err2));
  static_assert(failure(2) > err1);
  static_assert(!(failure(1) > err1));
  static_assert(!(failure(2) > err2));

  static_assert(!(err1 > failure(2)));
  static_assert(err2 > failure(1));
  static_assert(!(err1 > failure(1)));
  static_assert(!(err2 > failure(2)));

  static_assert(!(failure(1) > failure(2)));
  static_assert(failure(2) > failure(1));
  static_assert(!(failure(1) > failure(1)));
  static_assert(!(failure(2) > failure(2)));

  static_assert(!(err1 > ok1));
  static_assert(!(err1 > ok2));
  static_assert(!(err2 > ok1));
  static_assert(!(err2 > ok2));

  static_assert(!(failure(1) > ok1));
  static_assert(!(failure(1) > ok2));
  static_assert(!(failure(2) > ok1));
  static_assert(!(failure(2) > ok2));

  static_assert(!(err1 > success(1)));
  static_assert(!(err1 > success(2)));
  static_assert(!(err2 > success(1)));
  static_assert(!(err2 > success(2)));

  static_assert(!(failure(1) > success(1)));
  static_assert(!(failure(1) > success(2)));
  static_assert(!(failure(2) > success(1)));
  static_assert(!(failure(2) > success(2)));

  static_assert(!(err1 > 1));
  static_assert(!(err1 > 2));
  static_assert(!(err2 > 1));
  static_assert(!(err2 > 2));

  static_assert(ok1 > err1);
  static_assert(ok1 > err2);
  static_assert(ok2 > err1);
  static_assert(ok2 > err2);

  static_assert(success(1) > err1);
  static_assert(success(1) > err2);
  static_assert(success(2) > err1);
  static_assert(success(2) > err2);

  static_assert(ok1 > failure(1));
  static_assert(ok1 > failure(2));
  static_assert(ok2 > failure(1));
  static_assert(ok2 > failure(2));

  static_assert(success(1) > failure(1));
  static_assert(success(1) > failure(2));
  static_assert(success(2) > failure(1));
  static_assert(success(2) > failure(2));

  static_assert(1 > err1);
  static_assert(1 > err2);
  static_assert(2 > err1);
  static_assert(2 > err2);
}

TEST_CASE("greater_or_equal compare", "[result][greater_or_equal]")
{
  constexpr result<int, int> ok1 = success(1);
  constexpr result<int, int> ok2 = success(2);
  constexpr result<int, int> err1 = failure(1);
  constexpr result<int, int> err2 = failure(2);

  static_assert(!(ok1 >= ok2));
  static_assert(ok2 >= ok1);
  static_assert(ok1 >= ok1);
  static_assert(ok2 >= ok2);

  static_assert(!(success(1) >= ok2));
  static_assert(success(2) >= ok1);
  static_assert(success(1) >= ok1);
  static_assert(success(2) >= ok2);

  static_assert(!(ok1 >= success(2)));
  static_assert(ok2 >= success(1));
  static_assert(ok1 >= success(1));
  static_assert(ok2 >= success(2));

  static_assert(!(success(1) >= success(2)));
  static_assert(success(2) >= success(1));
  static_assert(success(1) >= success(1));
  static_assert(success(2) >= success(2));

  static_assert(!(1 >= ok2));
  static_assert(2 >= ok1);
  static_assert(1 >= ok1);
  static_assert(2 >= ok2);

  static_assert(!(ok1 >= 2));
  static_assert(ok2 >= 1);
  static_assert(ok1 >= 1);
  static_assert(ok2 >= 2);

  static_assert(!(err1 >= err2));
  static_assert(err2 >= err1);
  static_assert(err1 >= err1);
  static_assert(err2 >= err2);

  static_assert(!(failure(1) >= err2));
  static_assert(failure(2) >= err1);
  static_assert(failure(1) >= err1);
  static_assert(failure(2) >= err2);

  static_assert(!(err1 >= failure(2)));
  static_assert(err2 >= failure(1));
  static_assert(err1 >= failure(1));
  static_assert(err2 >= failure(2));

  static_assert(!(failure(1) >= failure(2)));
  static_assert(failure(2) >= failure(1));
  static_assert(failure(1) >= failure(1));
  static_assert(failure(2) >= failure(2));

  static_assert(!(err1 >= ok1));
  static_assert(!(err1 >= ok2));
  static_assert(!(err2 >= ok1));
  static_assert(!(err2 >= ok2));

  static_assert(!(failure(1) >= ok1));
  static_assert(!(failure(1) >= ok2));
  static_assert(!(failure(2) >= ok1));
  static_assert(!(failure(2) >= ok2));

  static_assert(!(err1 >= success(1)));
  static_assert(!(err1 >= success(2)));
  static_assert(!(err2 >= success(1)));
  static_assert(!(err2 >= success(2)));

  static_assert(!(failure(1) >= success(1)));
  static_assert(!(failure(1) >= success(2)));
  static_assert(!(failure(2) >= success(1)));
  static_assert(!(failure(2) >= success(2)));

  static_assert(!(err1 >= 1));
  static_assert(!(err1 >= 2));
  static_assert(!(err2 >= 1));
  static_assert(!(err2 >= 2));

  static_assert(ok1 >= err1);
  static_assert(ok1 >= err2);
  static_assert(ok2 >= err1);
  static_assert(ok2 >= err2);

  static_assert(success(1) >= err1);
  static_assert(success(1) >= err2);
  static_assert(success(2) >= err1);
  static_assert(success(2) >= err2);

  static_assert(ok1 >= failure(1));
  static_assert(ok1 >= failure(2));
  static_assert(ok2 >= failure(1));
  static_assert(ok2 >= failure(2));

  static_assert(success(1) >= failure(1));
  static_assert(success(1) >= failure(2));
  static_assert(success(2) >= failure(1));
  static_assert(success(2) >= failure(2));

  static_assert(1 >= err1);
  static_assert(1 >= err2);
  static_assert(2 >= err1);
  static_assert(2 >= err2);
}

#include <mitama/boolinators.hpp>

TEST_CASE("as_ok test", "[result][as_ok][boolinators]")
{
  constexpr basic_result x = as_ok(true, 1);
  static_assert(x == success(1));
  constexpr basic_result y = as_ok(false, 1);
  static_assert(y == failure());
}

TEST_CASE("as_result test", "[result][as_result][boolinators]")
{
  constexpr basic_result x = as_result(true, 1, "err"sv);
  static_assert(x == success(1));
  constexpr basic_result y = as_result(false, 1, "err"sv);
  static_assert(y == failure("err"));
}

TEST_CASE("as_result_from test", "[result][as_result_from][boolinators]")
{
  constexpr basic_result x =
      as_result_from(true, [] { return 1; }, [] { return "err"sv; });
  static_assert(x == success(1));
  constexpr basic_result y =
      as_result_from(false, [] { return 1; }, [] { return "err"sv; });
  static_assert(y == failure("err"));
}

TEST_CASE("ok_or test", "[result][ok_or][boolinators]")
{
  constexpr basic_result x = ok_or(true, "err"sv);
  static_assert(x == success(std::monostate{}));
  constexpr basic_result y = ok_or(false, "err"sv);
  static_assert(y == failure("err"sv));
}

TEST_CASE("ok_or_else test", "[result][ok_or_else][boolinators]")
{
  constexpr basic_result x = ok_or_else(true, [] { return "err"sv; });
  static_assert(x == success(std::monostate{}));
  constexpr basic_result y = ok_or_else(false, [] { return "err"sv; });
  static_assert(y == failure("err"sv));
}

TEST_CASE("result with void", "[result][void]")
{
  constexpr result<void, str_ref> ok1 = success();
  static_assert(ok1.is_ok() == true);

  constexpr result<void, void> ok2 = success(std::monostate{});
  static_assert(ok2.is_ok() == true);

  constexpr result<u32, void> err1 = failure(std::monostate{});
  static_assert(err1.is_ok() == false);

  constexpr result<void, void> err2 = failure();
  static_assert(err2.is_ok() == false);
}

TEST_CASE("map(F(u32) -> void)", "[result][map][u32][void]")
{
  u32 val = 3;
  auto lambda = [&](u32 y) { val += y; };

  result<u32, str> x = success(1);
  result<void, str> y = x.map(lambda);

  REQUIRE(y.is_ok() == true);
  REQUIRE(x.map(lambda).is_ok() == true);
  REQUIRE(val == 5);
}

TEST_CASE("map(F(void) -> u32)", "[result][map][void][u32]")
{
  u32 val = 3;
  auto lambda = [&]() -> u32 { return val + 1; };

  result<void, str> x = success();
  result<u32, str> y = x.map(lambda);

  REQUIRE(y.is_ok() == true);
  REQUIRE(x.map(lambda).is_ok() == true);
  REQUIRE(y.unwrap() == 4);
}

TEST_CASE("map(F(void) -> void)", "[result][map][void][void]")
{
  u32 val = 3;
  auto lambda = [&] { val += 1; };

  result<void, str> x = success();
  result<void, str> y = x.map(lambda);

  REQUIRE(y.is_ok() == true);
  REQUIRE(x.map(lambda).is_ok() == true);
  REQUIRE(val == 5);
}

TEST_CASE("map_err(F(u32) -> void)", "[result][map_err][u32][void]")
{
  u32 val = 3;
  auto lambda = [&](u32 y) { val += y; };

  result<str, u32> x = failure(1);
  result<str, void> y = x.map_err(lambda);

  REQUIRE(y.is_ok() == false);
  REQUIRE(x.map_err(lambda).is_ok() == false);
  REQUIRE(val == 5);
}

TEST_CASE("map_err(F(void) -> u32)", "[result][map_err][void][u32]")
{
  u32 val = 3;
  auto lambda = [&]() -> u32 { return val += 1; };

  result<str, void> x = failure();
  result<str, u32> y = x.map_err(lambda);

  REQUIRE(y.is_ok() == false);
  REQUIRE(x.map_err(lambda).is_ok() == false);
  REQUIRE(y.unwrap_err() == 4);
}

TEST_CASE("map_err(F(void) -> void)", "[result][map_err][void][void]")
{
  u32 val = 3;
  auto lambda = [&] { val += 1; };

  result<str, void> x = failure();
  result<str, void> y = x.map_err(lambda);

  REQUIRE(y.is_ok() == false);
  REQUIRE(x.map_err(lambda).is_ok() == false);
  REQUIRE(val == 5);
}

TEST_CASE("map & map_err with void", "[result][map][map_err][void]")
{
  u32 val = 3;
  auto add_one_val = [&] { val += 1; };
  auto add_one_arg = [&]([[maybe_unused]] u32 e) { e += 1; };

  result<void, u32> x = success();
  result<void, void> y = x.map(add_one_val) // 4
                             .map(add_one_val) // 5
                             .map(add_one_val) // 6
                             .map_err(add_one_arg) // does not affect
                             //          .map_err(add_one_arg)
                             //          ^~~~~~~~~~~~~~~~~~~~~ Compile Error!
                             .map_err(add_one_val); // does not affect

  REQUIRE(y.is_ok() == true);
  REQUIRE(val == 6);
}

TEST_CASE("MITAMA_TRY", "[result][mitama_try]")
{
  auto func = []() -> result<u32, str_ref>
  {
    result<u32, str_ref> a = success(1);
    u32 b = 2, c = 3;
    u32 d = MITAMA_TRY(
        [&a, &b, &c]() -> ::result<u32, str_ref>
        { return a.map([&b, &c](u32 x) -> u32 { return x + b + c; }); }()
    );
    return success(d);
  };

  constexpr result<u32, str_ref> x = func();
  static_assert(MITAMA_CPP_RESULT_TRY_MAY_NOT_PANIC == true);
  static_assert(x.is_ok() == true);
  static_assert(x.unwrap() == 6);
}

TEST_CASE("MITAMA_TRY2", "[result][mitama_try]")
{
  (void)[]
  {
    auto res =
        mut_result<std::unique_ptr<u32>>{ mitama::in_place_ok, new auto(42u) };

    auto&& ret = MITAMA_TRY(std::move(res));
    return mitama::failure();
  }
  ();
}
