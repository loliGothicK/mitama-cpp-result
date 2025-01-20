#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <mitama/maybe/maybe.hpp>
#include <mitama/maybe/range_to_maybe.hpp>
#include <mitama/result/result.hpp>
#include <mitama/result/result_io.hpp>

#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

using namespace mitama;
using namespace std::string_literals;

TEST_CASE("is_just()", "[maybe][is_just]")
{
  maybe<int> x = just(2);
  REQUIRE(x.is_just());

  maybe<int> y = nothing;
  REQUIRE_FALSE(y.is_just());
}

TEST_CASE("is_nothing()", "[maybe][is_nothing]")
{
  {
    maybe<int> x = just(2);
    REQUIRE_FALSE(x.is_nothing());

    maybe<int> y = nothing;
    REQUIRE(y.is_nothing());
  }
  {
    maybe<bool> x = nothing;
    REQUIRE(x.is_nothing());

    // Issue #412
    maybe<bool> y(x);
    REQUIRE(y.is_nothing());
  }
}

TEST_CASE("unwrap()", "[maybe][unwrap]")
{
  {
    maybe x = just("air"s);
    REQUIRE(x.unwrap() == "air"s);
  }
  try
  {
    maybe<int> x = nothing;
    x.unwrap(); // panics
  }
  catch (const runtime_panic& p)
  {
    REQUIRE(std::string_view{ p.what() }.starts_with(
        "runtime panicked at 'called `maybe::unwrap()` on a "
        "`nothing` value', "
    ));
  }
}

TEST_CASE("expect()", "[maybe][expect]")
{
  {
    maybe x = just("air"s);
    REQUIRE(x.expect("the world is ending") == "air"s);
  }
  try
  {
    maybe<int> x = nothing;
    x.expect("the world is ending"); // panics
  }
  catch (const runtime_panic& p)
  {
    REQUIRE(std::string_view{ p.what() }.starts_with(
        "runtime panicked at 'the world is ending', "
    ));
  }
}

TEST_CASE("cloned()", "[maybe][cloned]")
{
  int x = 12;
  maybe<int&> opt_x = just(x);
  REQUIRE(opt_x == just(12));
  REQUIRE(&(opt_x.unwrap()) == &x);

  auto shallow_copied = opt_x;
  REQUIRE(&(shallow_copied.unwrap()) == &x);

  auto cloned = opt_x.cloned();
  REQUIRE(cloned == just(12));
  REQUIRE(&(cloned.unwrap()) != &x);
}

TEST_CASE("unwrap_or()", "[maybe][unwrap_or]")
{
  REQUIRE(maybe{ just("car"s) }.unwrap_or("bike"s) == "car"s);
  REQUIRE(maybe<std::string>{ nothing }.unwrap_or("bike"s) == "bike"s);
}

TEST_CASE("unwrap_or_else()", "[maybe][unwrap_or_else]")
{
  int k = 10;
  REQUIRE(maybe{ just(4) }.unwrap_or_else([k] { return 2 * k; }) == 4);
  REQUIRE(maybe<int>{}.unwrap_or_else([k] { return 2 * k; }) == 20);
}

TEST_CASE("map()", "[maybe][map]")
{

  maybe maybe_some_string = just("Hello, World!"s);
  // `maybe::map` takes self *by ref*,
  // *not* consuming `maybe_some_string`
  maybe maybe_some_len = maybe_some_string.map(&std::string::size);

  REQUIRE(maybe_some_len == just(13u));

  maybe some_list = just(std::vector<int>{ 1, 2, 3, 4, 5 });
  auto acc = [](auto&& range, auto init) -> double
  { return std::accumulate(range.begin(), range.end(), init); };
  maybe sum = some_list.map(acc, 0.0);

  REQUIRE(sum == just(15));
}

TEST_CASE("map_or()", "[maybe][map_or]")
{

  maybe x = just("foo"s);
  REQUIRE(x.map_or(42, &std::string::size) == 3);

  maybe<std::string> y = nothing;
  REQUIRE(y.map_or(42, &std::string::size) == 42);

  maybe some_num = just(1);

  maybe z = some_num.map_or(0, std::plus{}, 1);

  REQUIRE(z == just(2));
}

TEST_CASE("map_or_else()", "[maybe][map_or_else]")
{
  int k = 21;

  maybe x = just("foo"s);
  REQUIRE(x.map_or_else([k] { return 2 * k; }, &std::string::size) == 3);

  maybe<std::string> y = nothing;
  REQUIRE(y.map_or_else([k] { return 2 * k; }, &std::string::size) == 42);
  REQUIRE(
      x.map_or_else([] { return "default"s; }, std::plus{}, "bar"s) == "foobar"s
  );
  REQUIRE(
      y.map_or_else([] { return "default"s; }, std::plus{}, "bar"s)
      == "default"s
  );

  maybe z = just(1);
  REQUIRE(z.map_or_else([k] { return 2 * k; }, std::plus{}, 1) == 2);
}

TEST_CASE("ok_or()", "[maybe][ok_or]")
{

  maybe x = just("foo"s);
  REQUIRE(x.ok_or(0) == success("foo"s));

  maybe<std::string> y = nothing;
  REQUIRE(y.ok_or(0) == failure(0));

  REQUIRE(y.ok_or() == failure<>());
  REQUIRE(y.ok_or<int>() == failure(int{}));
}

TEST_CASE("ok_or_else()", "[maybe][ok_or_else]")
{

  maybe x = just("foo"s);
  REQUIRE(x.ok_or_else([] { return 0; }) == success("foo"s));

  maybe<std::string> y = nothing;
  REQUIRE(y.ok_or_else([] { return 0; }) == failure(0));
}

TEST_CASE("conj()", "[maybe][conj]")
{
  {
    maybe x = just(2);
    maybe<std::string> y = nothing;
    REQUIRE(x.conj(y) == nothing);
    REQUIRE((x && y) == nothing);
  }
  {
    maybe<int> x = nothing;
    maybe y = just("foo"s);
    REQUIRE(x.conj(y) == nothing);
    REQUIRE((x && y) == nothing);
  }
  {
    maybe x = just(2);
    maybe y = just("foo"s);
    REQUIRE(x.conj(y) == just("foo"s));
    REQUIRE((x && y) == just("foo"s));
  }
  {
    maybe<int> x = nothing;
    maybe<std::string> y = nothing;
    REQUIRE(x.conj(y) == nothing);
    REQUIRE((x && y) == nothing);
  }
}

TEST_CASE("and_then()", "[maybe][and_then]")
{

  auto sq = [](int x) -> maybe<int> { return just(x * x); };
  auto nope = [](...) -> maybe<int> { return nothing; };
  auto is_eq = [](int a, int b) { return a == b ? maybe<int>{ a } : nothing; };

  REQUIRE(maybe{ just(2) }.and_then(sq).and_then(sq) == just(16));
  REQUIRE(maybe{ just(2) }.and_then(sq).and_then(nope) == nothing);
  REQUIRE(maybe{ just(2) }.and_then(nope).and_then(sq) == nothing);
  REQUIRE(maybe{ just(2) }.and_then(is_eq, 2) == just(2));
  REQUIRE(nope().and_then(sq).and_then(sq) == nothing);
}

TEST_CASE("filter()", "[maybe][filter]")
{

  auto is_even = [](int n) -> bool { return n % 2 == 0; };

  REQUIRE(maybe<int>{}.filter(is_even) == nothing);
  REQUIRE(maybe{ just(3) }.filter(is_even) == nothing);
  REQUIRE(maybe{ just(4) }.filter(is_even) == just(4));
}

TEST_CASE("disj()", "[maybe][disj]")
{
  {
    maybe x = just(2);
    maybe<int> y = nothing;
    assert(x.disj(y) == just(2));
    assert((x || y) == just(2));
  }
  {
    maybe<int> x = nothing;
    maybe y = just(100);
    assert(x.disj(y) == just(100));
    assert((x || y) == just(100));
  }
  {
    maybe x = just(2);
    maybe y = just(100);
    assert(x.disj(y) == just(2));
    assert((x || y) == just(2));
  }
  {
    maybe<int> x = nothing;
    maybe<int> y = nothing;
    assert(x.disj(y) == nothing);
    assert((x || y) == nothing);
  }
}

TEST_CASE("or_else()", "[maybe][or_else]")
{

  auto nobody = []() -> maybe<std::string> { return nothing; };
  auto vikings = []() -> maybe<std::string> { return just("vikings"s); };
  auto to_maybe = [](auto&& e) -> maybe<std::decay_t<decltype(e)>>
  { return just(std::forward<decltype(e)>(e)); };

  REQUIRE(maybe{ just("barbarians"s) }.or_else(vikings) == just("barbarians"s));
  REQUIRE(maybe<std::string>{}.or_else(vikings) == just("vikings"s));
  REQUIRE(
      maybe<std::string>{}.or_else(to_maybe, "vikings"s) == just("vikings"s)
  );
  REQUIRE(maybe<std::string>{}.or_else(nobody) == nothing);
}

TEST_CASE("xdisj()", "[maybe][xdisj]")
{
  {
    maybe x = just(2);
    maybe<int> y = nothing;
    assert(x.xdisj(y) == just(2));
    assert((x ^ y) == just(2));
  }
  {
    maybe<int> x = nothing;
    maybe y = just(100);
    assert(x.xdisj(y) == just(100));
    assert((x ^ y) == just(100));
  }
  {
    maybe x = just(2);
    maybe y = just(100);
    assert(x.xdisj(y) == nothing);
    assert((x ^ y) == nothing);
  }
  {
    maybe<int> x = nothing;
    maybe<int> y = nothing;
    assert(x.xdisj(y) == nothing);
    assert((x ^ y) == nothing);
  }
}

struct noncopyable
{
  noncopyable() = default;
  noncopyable(const noncopyable&) = delete;
  noncopyable& operator=(const noncopyable&) = delete;
  noncopyable(noncopyable&&) = default;
  noncopyable& operator=(noncopyable&&) = default;

  bool operator==(noncopyable&&) &
  {
    return true;
  }
  bool operator==(noncopyable&&) const&
  {
    return true;
  }
  bool operator==(const noncopyable&) &
  {
    return true;
  }
  bool operator==(const noncopyable&) const&
  {
    return true;
  }

  friend std::ostream& operator<<(std::ostream& os, noncopyable&)
  {
    return os << "noncopyable&";
  }
  friend std::ostream& operator<<(std::ostream& os, const noncopyable&)
  {
    return os << "noncopyable const&";
  }
  friend std::ostream& operator<<(std::ostream& os, noncopyable&&)
  {
    return os << "noncopyable&&";
  }
};

TEST_CASE("get_or_emplace()", "[maybe][get_or_emplace]")
{
  maybe<noncopyable> x = nothing;
  auto& y = x.get_or_emplace(noncopyable{});
  REQUIRE(y == noncopyable{});
}

TEST_CASE("get_or_emplace_with()", "[maybe][get_or_emplace_with]")
{
  GIVEN("a nothing type of maybe<int>.")
  {
    maybe<int> x = nothing;
    WHEN("call get_or_emplace_with and bind to `y`.")
    {
      std::ignore = x.get_or_emplace_with([] { return 5; });
      std::ignore = x.get_or_emplace_with([](auto x) { return x; }, 5);
      auto& y = x.get_or_emplace_with(&std::string::size, "12345"s);
      REQUIRE(y == 5);
      WHEN("assign 7 to `y`")
      {
        y = 7;
        THEN("changed x to just(7)")
        {
          REQUIRE(x == just(7));
        }
      }
    }
  }
}

TEST_CASE("as_ref()", "[maybe][as_ref]")
{
  {
    maybe x = just(5);
    auto ref = x.as_ref();
    REQUIRE(x == just(5));
    ref.unwrap() = 2;
    REQUIRE(x == just(2));
  }
}

TEST_CASE("replace()", "[maybe][replace]")
{
  {
    maybe x = just(2);
    auto old = x.replace(5);
    REQUIRE(x == just(5));
    REQUIRE(old == just(2));
  }
  {
    maybe<int> x = nothing;
    auto old = x.replace(3);
    REQUIRE(x == just(3));
    REQUIRE(old == nothing);
  }
}

TEST_CASE("transpose()", "[maybe][transpose]")
{

  result<maybe<int>, std::string> x = success(just(5));
  maybe<result<int, std::string>> y = just(success(5));
  REQUIRE(x == y.transpose());
}

TEST_CASE("unwrap_or_default()", "[maybe][unwrap_or_default]")
{

  maybe<std::string> x = nothing;
  REQUIRE(x.unwrap_or_default() == ""s);
}

TEST_CASE("flatten()", "[maybe][flatten]")
{

  maybe<maybe<int>> x = just(just(6));
  REQUIRE(just(6) == x.flatten());

  maybe<maybe<int>> y = just(nothing);
  REQUIRE(nothing == y.flatten());

  maybe<maybe<int>> z = nothing;
  REQUIRE(nothing == z.flatten());

  // Flattening once only removes one level of nesting:
  maybe<maybe<maybe<int>>> nest = just(just(just(6)));
  REQUIRE(just(6) == nest.flatten().flatten());
}

TEST_CASE("and_finally()", "[maybe][and_finally]")
{

  std::string hook = "default";
  maybe<std::string> x = nothing;
  x.and_finally([&hook](std::string v) { hook = v; });
  REQUIRE(hook == "default"s);

  maybe y = just("error"s);
  y.and_finally([&hook](std::string v) { hook = v; });
  REQUIRE(hook == "error"s);
}

TEST_CASE("or_finally()", "[maybe][or_finally]")
{

  std::string hook = "default";
  maybe<int> x = nothing;
  x.or_finally([&hook] { hook = "error"; });
  REQUIRE(hook == "error"s);
}

TEST_CASE("and_peek() test", "[maybe][and_peek]")
{
  maybe x = just(42);
  REQUIRE(x.and_peek([](int& v) { v = 57; }) == just(57));
  int y = 0;
  REQUIRE(
      (std::as_const(x).and_peek([&y](auto&& v) { y = v; }), y) == x.unwrap()
  );
}

TEST_CASE("or_peek() test", "[maybe][and_peek]")
{
  maybe<int> x = nothing;
  int hook = 0;
  REQUIRE(x.or_peek([&hook] { hook = 57; }) == nothing);
  REQUIRE(hook == 57);
}

struct base
{
  virtual ~base() = default;
  virtual std::string test()
  {
    return "base";
  }
};

struct derived final : base
{
  std::string test() override
  {
    return "derived";
  }
};

TEST_CASE("reference of abstract", "[maybe][abstract]")
{
  derived _derived = {};
  maybe<base&> x = just(_derived);
  REQUIRE(x->test() == "derived"s);
  base _base = {};
  x = just(_base);
  REQUIRE(x->test() == "base"s);
}

TEST_CASE("equal", "[maybe][equal]")
{
  maybe<int> just1 = just(1);
  maybe<int> just2 = just(2);
  maybe<int> none = nothing;

  REQUIRE_FALSE(just1 == just2);
  REQUIRE_FALSE(just2 == just1);
  REQUIRE(just1 == just1);
  REQUIRE(just2 == just2);

  REQUIRE_FALSE(just1 == 2);
  REQUIRE_FALSE(just2 == 1);
  REQUIRE(just1 == 1);
  REQUIRE(just2 == 2);

  REQUIRE_FALSE(1 == just2);
  REQUIRE_FALSE(2 == just1);
  REQUIRE(1 == just1);
  REQUIRE(2 == just2);

  REQUIRE_FALSE(just(1) == just2);
  REQUIRE_FALSE(just(2) == just1);
  REQUIRE(just(1) == just1);
  REQUIRE(just(2) == just2);

  REQUIRE_FALSE(just1 == just(2));
  REQUIRE_FALSE(just2 == just(1));
  REQUIRE(just1 == just(1));
  REQUIRE(just2 == just(2));

  REQUIRE_FALSE(just(1) == just(2));
  REQUIRE_FALSE(just(2) == just(1));
  REQUIRE(just(1) == just(1));
  REQUIRE(just(2) == just(2));

  REQUIRE(none == none);
  REQUIRE(nothing == none);
  REQUIRE(none == nothing);
  REQUIRE(nothing == nothing);

  REQUIRE_FALSE(none == just1);
  REQUIRE_FALSE(none == just2);
  REQUIRE_FALSE(none == 1);
  REQUIRE_FALSE(none == 2);

  REQUIRE_FALSE(1 == none);
  REQUIRE_FALSE(2 == none);

  REQUIRE_FALSE(just1 == none);
  REQUIRE_FALSE(just2 == none);

  REQUIRE_FALSE(nothing == just1);
  REQUIRE_FALSE(nothing == just2);

  REQUIRE_FALSE(none == just(1));
  REQUIRE_FALSE(none == just(2));

  REQUIRE_FALSE(nothing == just(1));
  REQUIRE_FALSE(nothing == just(2));

  REQUIRE_FALSE(just1 == none);
  REQUIRE_FALSE(just2 == none);

  REQUIRE_FALSE(just(1) == none);
  REQUIRE_FALSE(just(2) == none);

  REQUIRE_FALSE(just1 == nothing);
  REQUIRE_FALSE(just2 == nothing);

  REQUIRE_FALSE(just(1) == nothing);
  REQUIRE_FALSE(just(2) == nothing);
}

TEST_CASE("not_equal", "[maybe][not_equal]")
{
  maybe<int> just1 = just(1);
  maybe<int> just2 = just(2);
  maybe<int> none = nothing;

  REQUIRE(just1 != just2);
  REQUIRE(just2 != just1);
  REQUIRE_FALSE(just1 != just1);
  REQUIRE_FALSE(just2 != just2);

  REQUIRE(just1 != 2);
  REQUIRE(just2 != 1);
  REQUIRE_FALSE(just1 != 1);
  REQUIRE_FALSE(just2 != 2);

  REQUIRE(1 != just2);
  REQUIRE(2 != just1);
  REQUIRE_FALSE(1 != just1);
  REQUIRE_FALSE(2 != just2);

  REQUIRE(just(1) != just2);
  REQUIRE(just(2) != just1);
  REQUIRE_FALSE(just(1) != just1);
  REQUIRE_FALSE(just(2) != just2);

  REQUIRE(just1 != just(2));
  REQUIRE(just2 != just(1));
  REQUIRE_FALSE(just1 != just(1));
  REQUIRE_FALSE(just2 != just(2));

  REQUIRE(just(1) != just(2));
  REQUIRE(just(2) != just(1));
  REQUIRE_FALSE(just(1) != just(1));
  REQUIRE_FALSE(just(2) != just(2));

  REQUIRE_FALSE(none != none);
  REQUIRE_FALSE(nothing != none);
  REQUIRE_FALSE(none != nothing);
  REQUIRE_FALSE(nothing != nothing);

  REQUIRE(none != just1);
  REQUIRE(none != just2);
  REQUIRE(none != 1);
  REQUIRE(none != 2);

  REQUIRE(1 != none);
  REQUIRE(2 != none);

  REQUIRE(just1 != none);
  REQUIRE(just2 != none);

  REQUIRE(nothing != just1);
  REQUIRE(nothing != just2);

  REQUIRE(none != just(1));
  REQUIRE(none != just(2));

  REQUIRE(nothing != just(1));
  REQUIRE(nothing != just(2));

  REQUIRE(just1 != none);
  REQUIRE(just2 != none);

  REQUIRE(just(1) != none);
  REQUIRE(just(2) != none);

  REQUIRE(just1 != nothing);
  REQUIRE(just2 != nothing);

  REQUIRE(just(1) != nothing);
  REQUIRE(just(2) != nothing);
}

TEST_CASE("less compare", "[maybe][less]")
{
  maybe<int> just1 = just(1);
  maybe<int> just2 = just(2);
  maybe<int> none = nothing;

  REQUIRE(just1 < just2);
  REQUIRE_FALSE(just2 < just1);
  REQUIRE_FALSE(just1 < just1);
  REQUIRE_FALSE(just2 < just2);

  REQUIRE(just1 < 2);
  REQUIRE_FALSE(just2 < 1);
  REQUIRE_FALSE(just1 < 1);
  REQUIRE_FALSE(just2 < 2);

  REQUIRE(1 < just2);
  REQUIRE_FALSE(2 < just1);
  REQUIRE_FALSE(1 < just1);
  REQUIRE_FALSE(2 < just2);

  REQUIRE(just(1) < just2);
  REQUIRE_FALSE(just(2) < just1);
  REQUIRE_FALSE(just(1) < just1);
  REQUIRE_FALSE(just(2) < just2);

  REQUIRE(just1 < just(2));
  REQUIRE_FALSE(just2 < just(1));
  REQUIRE_FALSE(just1 < just(1));
  REQUIRE_FALSE(just2 < just(2));

  REQUIRE(just(1) < just(2));
  REQUIRE_FALSE(just(2) < just(1));
  REQUIRE_FALSE(just(1) < just(1));
  REQUIRE_FALSE(just(2) < just(2));

  REQUIRE_FALSE(none < none);
  REQUIRE_FALSE(nothing < none);
  REQUIRE_FALSE(none < nothing);
  REQUIRE_FALSE(nothing < nothing);

  REQUIRE(none < just1);
  REQUIRE(none < just2);
  REQUIRE(none < 1);
  REQUIRE(none < 2);

  REQUIRE_FALSE(1 < none);
  REQUIRE_FALSE(2 < none);

  REQUIRE_FALSE(just1 < none);
  REQUIRE_FALSE(just2 < none);

  REQUIRE(nothing < just1);
  REQUIRE(nothing < just2);

  REQUIRE(none < just(1));
  REQUIRE(none < just(2));

  REQUIRE(nothing < just(1));
  REQUIRE(nothing < just(2));

  REQUIRE_FALSE(just1 < none);
  REQUIRE_FALSE(just2 < none);

  REQUIRE_FALSE(just(1) < none);
  REQUIRE_FALSE(just(2) < none);

  REQUIRE_FALSE(just1 < nothing);
  REQUIRE_FALSE(just2 < nothing);

  REQUIRE_FALSE(just(1) < nothing);
  REQUIRE_FALSE(just(2) < nothing);
}

TEST_CASE("less_or_equal compare", "[maybe][less_or_equal]")
{
  maybe<int> just1 = just(1);
  maybe<int> just2 = just(2);
  maybe<int> none = nothing;

  REQUIRE(just1 <= just2);
  REQUIRE_FALSE(just2 <= just1);
  REQUIRE(just1 <= just1);
  REQUIRE(just2 <= just2);

  REQUIRE(just1 <= 2);
  REQUIRE_FALSE(just2 <= 1);
  REQUIRE(just1 <= 1);
  REQUIRE(just2 <= 2);

  REQUIRE(1 <= just2);
  REQUIRE_FALSE(2 <= just1);
  REQUIRE(1 <= just1);
  REQUIRE(2 <= just2);

  REQUIRE(just(1) <= just2);
  REQUIRE_FALSE(just(2) <= just1);
  REQUIRE(just(1) <= just1);
  REQUIRE(just(2) <= just2);

  REQUIRE(just1 <= just(2));
  REQUIRE_FALSE(just2 <= just(1));
  REQUIRE(just1 <= just(1));
  REQUIRE(just2 <= just(2));

  REQUIRE(just(1) <= just(2));
  REQUIRE_FALSE(just(2) <= just(1));
  REQUIRE(just(1) <= just(1));
  REQUIRE(just(2) <= just(2));

  REQUIRE(none <= none);
  REQUIRE(nothing <= none);
  REQUIRE(none <= nothing);
  REQUIRE(nothing <= nothing);

  REQUIRE(none <= just1);
  REQUIRE(none <= just2);

  REQUIRE(none <= 1);
  REQUIRE(none <= 2);

  REQUIRE(nothing <= just1);
  REQUIRE(nothing <= just2);

  REQUIRE(none <= just(1));
  REQUIRE(none <= just(2));

  REQUIRE(nothing <= just(1));
  REQUIRE(nothing <= just(2));

  REQUIRE_FALSE(just1 <= none);
  REQUIRE_FALSE(just2 <= none);

  REQUIRE_FALSE(1 <= none);
  REQUIRE_FALSE(2 <= none);

  REQUIRE_FALSE(just(1) <= none);
  REQUIRE_FALSE(just(2) <= none);

  REQUIRE_FALSE(just1 <= nothing);
  REQUIRE_FALSE(just2 <= nothing);

  REQUIRE_FALSE(just(1) <= nothing);
  REQUIRE_FALSE(just(2) <= nothing);
}

TEST_CASE("greater compare", "[maybe][greater]")
{
  maybe<int> just1 = just(1);
  maybe<int> just2 = just(2);
  maybe<int> none = nothing;

  REQUIRE_FALSE(just1 > just2);
  REQUIRE(just2 > just1);
  REQUIRE_FALSE(just1 > just1);
  REQUIRE_FALSE(just2 > just2);

  REQUIRE_FALSE(just1 > 2);
  REQUIRE(just2 > 1);
  REQUIRE_FALSE(just1 > 1);
  REQUIRE_FALSE(just2 > 2);

  REQUIRE_FALSE(1 > just2);
  REQUIRE(2 > just1);
  REQUIRE_FALSE(1 > just1);
  REQUIRE_FALSE(2 > just2);

  REQUIRE_FALSE(just(1) > just2);
  REQUIRE(just(2) > just1);
  REQUIRE_FALSE(just(1) > just1);
  REQUIRE_FALSE(just(2) > just2);

  REQUIRE_FALSE(just1 > just(2));
  REQUIRE(just2 > just(1));
  REQUIRE_FALSE(just1 > just(1));
  REQUIRE_FALSE(just2 > just(2));

  REQUIRE_FALSE(just(1) > just(2));
  REQUIRE(just(2) > just(1));
  REQUIRE_FALSE(just(1) > just(1));
  REQUIRE_FALSE(just(2) > just(2));

  REQUIRE_FALSE(none > none);
  REQUIRE_FALSE(nothing > none);
  REQUIRE_FALSE(none > nothing);
  REQUIRE_FALSE(nothing > nothing);

  REQUIRE_FALSE(none > just1);
  REQUIRE_FALSE(none > just2);

  REQUIRE_FALSE(none > 1);
  REQUIRE_FALSE(none > 2);

  REQUIRE_FALSE(nothing > just1);
  REQUIRE_FALSE(nothing > just2);

  REQUIRE_FALSE(none > just(1));
  REQUIRE_FALSE(none > just(2));

  REQUIRE_FALSE(nothing > just(1));
  REQUIRE_FALSE(nothing > just(2));

  REQUIRE(just1 > none);
  REQUIRE(just2 > none);

  REQUIRE(1 > none);
  REQUIRE(2 > none);

  REQUIRE(just(1) > none);
  REQUIRE(just(2) > none);

  REQUIRE(just1 > nothing);
  REQUIRE(just2 > nothing);

  REQUIRE(just(1) > nothing);
  REQUIRE(just(2) > nothing);
}

TEST_CASE("greater_or_equal compare", "[maybe][greater_or_equal]")
{
  maybe<int> just1 = just(1);
  maybe<int> just2 = just(2);
  maybe<int> none = nothing;

  REQUIRE_FALSE(just1 >= just2);
  REQUIRE(just2 >= just1);
  REQUIRE(just1 >= just1);
  REQUIRE(just2 >= just2);

  REQUIRE_FALSE(just1 >= 2);
  REQUIRE(just2 >= 1);
  REQUIRE(just1 >= 1);
  REQUIRE(just2 >= 2);

  REQUIRE_FALSE(1 >= just2);
  REQUIRE(2 >= just1);
  REQUIRE(1 >= just1);
  REQUIRE(2 >= just2);

  REQUIRE_FALSE(just(1) >= just2);
  REQUIRE(just(2) >= just1);
  REQUIRE(just(1) >= just1);
  REQUIRE(just(2) >= just2);

  REQUIRE_FALSE(just1 >= just(2));
  REQUIRE(just2 >= just(1));
  REQUIRE(just1 >= just(1));
  REQUIRE(just2 >= just(2));

  REQUIRE_FALSE(just(1) >= just(2));
  REQUIRE(just(2) >= just(1));
  REQUIRE(just(1) >= just(1));
  REQUIRE(just(2) >= just(2));

  REQUIRE(none >= none);
  REQUIRE(nothing >= none);
  REQUIRE(none >= nothing);
  REQUIRE(nothing >= nothing);

  REQUIRE_FALSE(nothing >= just1);
  REQUIRE_FALSE(nothing >= just2);

  REQUIRE_FALSE(none >= just1);
  REQUIRE_FALSE(none >= just2);

  REQUIRE_FALSE(none >= 1);
  REQUIRE_FALSE(none >= 2);

  REQUIRE_FALSE(none >= just(1));
  REQUIRE_FALSE(none >= just(2));

  REQUIRE_FALSE(nothing >= just(1));
  REQUIRE_FALSE(nothing >= just(2));

  REQUIRE(just1 >= none);
  REQUIRE(just2 >= none);

  REQUIRE(1 >= none);
  REQUIRE(2 >= none);

  REQUIRE(just(1) >= none);
  REQUIRE(just(2) >= none);

  REQUIRE(just1 >= nothing);
  REQUIRE(just2 >= nothing);

  REQUIRE(just(1) >= nothing);
  REQUIRE(just(2) >= nothing);
}

TEST_CASE("format test", "[maybe][format]")
{
  using str = std::string;
  SECTION("just")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << just(1);
    REQUIRE(ss.str() == "just(1)"s);
  }
  SECTION("maybe of just")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << maybe<int>{ just(1) };
    REQUIRE(ss.str() == "just(1)"s);
  }
  SECTION("just of range")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << just(std::vector<std::string>{ "foo"s, "bar"s });
    REQUIRE(ss.str() == "just([\"foo\", \"bar\"])"s);
  }
  SECTION("maybe of range err")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << maybe(just(std::vector<std::string>{ "foo"s, "bar"s }));
    REQUIRE(ss.str() == "just([\"foo\", \"bar\"])"s);
  }
  SECTION("just of tuple")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << just(std::tuple{ "foo"s, 1 });
    REQUIRE(ss.str() == "just((\"foo\", 1))"s);
  }
  SECTION("maybe of just tuple")
  {
    using namespace std::literals;
    std::stringstream ss;
    ss << maybe(just(std::tuple{ "foo"s, 1 }));
    REQUIRE(ss.str() == "just((\"foo\", 1))"s);
  }
  SECTION("just of dictionary")
  {
    using namespace std::literals;
    {
      std::stringstream ss;
      ss << just(std::map<str, int>{ { "foo"s, 1 } });
      REQUIRE(ss.str() == "just({\"foo\": 1})"s);
    }
  }
  SECTION("maybe of just dictionary")
  {
    using namespace std::literals;
    {
      std::stringstream ss;
      ss << maybe<std::map<str, int>>(just(std::map<str, int>{ { "foo"s, 1 } })
      );
      REQUIRE(ss.str() == "just({\"foo\": 1})"s);
    }
  }
  SECTION("just of tuple of tuple")
  {
    using namespace std::literals;
    {
      std::stringstream ss;
      ss << just(std::tuple{ std::tuple{ 1, 1 }, 1 });
      REQUIRE(ss.str() == "just(((1, 1), 1))"s);
    }
  }
  SECTION("maybe of just tuple of tuple")
  {
    using namespace std::literals;
    {
      std::stringstream ss;
      ss << maybe(just(std::tuple{ std::tuple{ 1, 1 }, 1 }));
      REQUIRE(ss.str() == "just(((1, 1), 1))"s);
    }
  }
}

TEST_CASE("forward mode test", "[maybe][forward]")
{
  maybe<std::string> aaaaaaaaaa = just(10, 'a');
  REQUIRE(aaaaaaaaaa == just("aaaaaaaaaa"));

  maybe<std::vector<int>> vec = just({ 1, 2, 3 });
  REQUIRE(vec == just(std::vector{ 1, 2, 3 }));
}

TEST_CASE("range_to_maybe test", "[maybe][range_to_maybe]")
{
  std::vector v{ 1, 2, 3 };
  maybe x = range_to_maybe(v);
  REQUIRE(x == just(1));

  maybe y = range_to_maybe(std::vector<int>{});
  REQUIRE(y == nothing);

  int a[] = { 1 };
  maybe z = range_to_maybe(a);
  REQUIRE(z == 1);
}

#include <mitama/boolinators.hpp>

TEST_CASE("as_maybe test", "[maybe][as_maybe][boolinators]")
{
  maybe x = as_maybe(true);
  REQUIRE(x == just(std::monostate{}));
  maybe y = as_maybe(false);
  REQUIRE(y == nothing);
}

TEST_CASE("as_just test", "[maybe][as_just][boolinators]")
{
  maybe x = as_just(true, 1);
  REQUIRE(x == just(1));
  maybe y = as_just(false, 1);
  REQUIRE(y == nothing);
}

TEST_CASE("as_just_from test", "[maybe][as_just_from][boolinators]")
{
  maybe x = as_just_from(true, [] { return 1; });
  REQUIRE(x == just(1));
  maybe y = as_just_from(false, [] { return 1; });
  REQUIRE(y == nothing);
}
