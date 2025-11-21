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
using namespace std::string_view_literals;

TEST_CASE("is_just()", "[maybe][is_just]")
{
  constexpr maybe<int> x = just(2);
  static_assert(x.is_just());

  constexpr maybe<int> y = nothing;
  static_assert(!y.is_just());
}

TEST_CASE("is_nothing()", "[maybe][is_nothing]")
{
  {
    constexpr maybe<int> x = just(2);
    static_assert(!x.is_nothing());

    constexpr maybe<int> y = nothing;
    static_assert(y.is_nothing());
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
    constexpr maybe x = just("air"sv);
    static_assert(x.unwrap() == "air"sv);
  }
  try
  {
    maybe<int> x = nothing;
    x.unwrap(); // panics
  }
  catch (const runtime_panic& p)
  {
    REQUIRE(
        std::string_view{ p.what() }.starts_with(
            "runtime panicked at 'called `maybe::unwrap()` on a "
            "`nothing` value', "
        )
    );
  }
}

TEST_CASE("expect()", "[maybe][expect]")
{
  {
    constexpr maybe x = just("air"sv);
    static_assert(x.expect("the world is ending") == "air"sv);
  }
  try
  {
    maybe<int> x = nothing;
    x.expect("the world is ending"); // panics
  }
  catch (const runtime_panic& p)
  {
    REQUIRE(
        std::string_view{ p.what() }.starts_with(
            "runtime panicked at 'the world is ending', "
        )
    );
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
  static_assert(maybe{ just("car"sv) }.unwrap_or("bike"sv) == "car"sv);
  static_assert(
      maybe<std::string_view>{ nothing }.unwrap_or("bike"sv) == "bike"sv
  );
}

TEST_CASE("unwrap_or_else()", "[maybe][unwrap_or_else]")
{
  constexpr int k = 10;
  static_assert(maybe{ just(4) }.unwrap_or_else([] { return 2 * k; }) == 4);
  static_assert(maybe<int>{}.unwrap_or_else([] { return 2 * k; }) == 20);
}

TEST_CASE("map()", "[maybe][map]")
{

  constexpr maybe maybe_some_string = just("Hello, World!"sv);
  // `maybe::map` takes self *by ref*,
  // *not* consuming `maybe_some_string`
  constexpr maybe maybe_some_len =
      maybe_some_string.map(&std::string_view::size);

  static_assert(maybe_some_len == just(13u));

  constexpr maybe some_list = just(std::array{ 1, 2, 3, 4, 5 });
  constexpr auto acc = [](auto&& range, auto init) -> double
  { return std::accumulate(range.begin(), range.end(), init); };
  constexpr maybe sum = some_list.map(acc, 0.0);

  static_assert(sum == just(15));
}

TEST_CASE("map_or()", "[maybe][map_or]")
{

  constexpr maybe x = just("foo"sv);
  static_assert(x.map_or(42, &std::string_view::size) == 3);

  constexpr maybe<std::string_view> y = nothing;
  static_assert(y.map_or(42, &std::string_view::size) == 42);

  constexpr maybe some_num = just(1);

  constexpr maybe z = some_num.map_or(0, std::plus{}, 1);

  static_assert(z == just(2));
}

TEST_CASE("map_or_else()", "[maybe][map_or_else]")
{
  constexpr int k = 21;

  maybe x = just("foo"s);
  REQUIRE(x.map_or_else([] { return 2 * k; }, &std::string::size) == 3);

  maybe<std::string> y = nothing;
  REQUIRE(y.map_or_else([] { return 2 * k; }, &std::string::size) == 42);
  REQUIRE(
      x.map_or_else([] { return "default"s; }, std::plus{}, "bar"s) == "foobar"s
  );
  REQUIRE(
      y.map_or_else([] { return "default"s; }, std::plus{}, "bar"s)
      == "default"s
  );

  constexpr maybe z = just(1);
  static_assert(z.map_or_else([] { return 2 * k; }, std::plus{}, 1) == 2);
}

TEST_CASE("ok_or()", "[maybe][ok_or]")
{
  constexpr maybe x = just("foo"sv);
  static_assert(x.ok_or(0) == success("foo"sv));

  constexpr maybe<std::string_view> y = nothing;
  static_assert(y.ok_or(0) == failure(0));

  static_assert(y.ok_or() == failure<>());
  static_assert(y.ok_or<int>() == failure(int{}));
}

TEST_CASE("ok_or_else()", "[maybe][ok_or_else]")
{
  constexpr maybe x = just("foo"sv);
  static_assert(x.ok_or_else([] { return 0; }) == success("foo"sv));

  constexpr maybe<std::string_view> y = nothing;
  static_assert(y.ok_or_else([] { return 0; }) == failure(0));
}

TEST_CASE("conj()", "[maybe][conj]")
{
  {
    constexpr maybe x = just(2);
    constexpr maybe<std::string_view> y = nothing;
    static_assert(x.conj(y) == nothing);
    static_assert((x && y) == nothing);
  }
  {
    constexpr maybe<int> x = nothing;
    constexpr maybe y = just("foo"sv);
    static_assert(x.conj(y) == nothing);
    static_assert((x && y) == nothing);
  }
  {
    constexpr maybe x = just(2);
    constexpr maybe y = just("foo"sv);
    static_assert(x.conj(y) == just("foo"sv));
    static_assert((x && y) == just("foo"sv));
  }
  {
    constexpr maybe<int> x = nothing;
    constexpr maybe<std::string_view> y = nothing;
    static_assert(x.conj(y) == nothing);
    static_assert((x && y) == nothing);
  }
}

TEST_CASE("and_then()", "[maybe][and_then]")
{
  constexpr auto sq = [](int x) -> maybe<int> { return just(x * x); };
  constexpr auto nope = [](...) -> maybe<int> { return nothing; };
  constexpr auto is_eq = [](int a, int b)
  { return a == b ? maybe<int>{ a } : nothing; };

  static_assert(maybe{ just(2) }.and_then(sq).and_then(sq) == just(16));
  static_assert(maybe{ just(2) }.and_then(sq).and_then(nope) == nothing);
  static_assert(maybe{ just(2) }.and_then(nope).and_then(sq) == nothing);
  static_assert(maybe{ just(2) }.and_then(is_eq, 2) == just(2));
  static_assert(nope().and_then(sq).and_then(sq) == nothing);
}

TEST_CASE("filter()", "[maybe][filter]")
{
  constexpr auto is_even = [](int n) -> bool { return n % 2 == 0; };

  static_assert(maybe<int>{}.filter(is_even) == nothing);
  static_assert(maybe{ just(3) }.filter(is_even) == nothing);
  static_assert(maybe{ just(4) }.filter(is_even) == just(4));
}

TEST_CASE("disj()", "[maybe][disj]")
{
  {
    constexpr maybe x = just(2);
    constexpr maybe<int> y = nothing;
    static_assert(x.disj(y) == just(2));
    static_assert((x || y) == just(2));
  }
  {
    constexpr maybe<int> x = nothing;
    constexpr maybe y = just(100);
    static_assert(x.disj(y) == just(100));
    static_assert((x || y) == just(100));
  }
  {
    constexpr maybe x = just(2);
    constexpr maybe y = just(100);
    static_assert(x.disj(y) == just(2));
    static_assert((x || y) == just(2));
  }
  {
    constexpr maybe<int> x = nothing;
    constexpr maybe<int> y = nothing;
    static_assert(x.disj(y) == nothing);
    static_assert((x || y) == nothing);
  }
}

TEST_CASE("or_else()", "[maybe][or_else]")
{

  constexpr auto nobody = []() -> maybe<std::string_view> { return nothing; };
  constexpr auto vikings = []() -> maybe<std::string_view>
  { return just("vikings"sv); };
  constexpr auto to_maybe = [](auto&& e) -> maybe<std::decay_t<decltype(e)>>
  { return just(std::forward<decltype(e)>(e)); };

  static_assert(
      maybe{ just("barbarians"sv) }.or_else(vikings) == just("barbarians"sv)
  );
  static_assert(
      maybe<std::string_view>{}.or_else(vikings) == just("vikings"sv)
  );
  static_assert(
      maybe<std::string_view>{}.or_else(to_maybe, "vikings"sv)
      == just("vikings"sv)
  );
  static_assert(maybe<std::string_view>{}.or_else(nobody) == nothing);
}

TEST_CASE("xdisj()", "[maybe][xdisj]")
{
  {
    constexpr maybe x = just(2);
    constexpr maybe<int> y = nothing;
    static_assert(x.xdisj(y) == just(2));
    static_assert((x ^ y) == just(2));
  }
  {
    constexpr maybe<int> x = nothing;
    constexpr maybe y = just(100);
    static_assert(x.xdisj(y) == just(100));
    static_assert((x ^ y) == just(100));
  }
  {
    constexpr maybe x = just(2);
    constexpr maybe y = just(100);
    static_assert(x.xdisj(y) == nothing);
    static_assert((x ^ y) == nothing);
  }
  {
    constexpr maybe<int> x = nothing;
    constexpr maybe<int> y = nothing;
    static_assert(x.xdisj(y) == nothing);
    static_assert((x ^ y) == nothing);
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
  constexpr result<maybe<int>, std::string_view> x = success(just(5));
  constexpr maybe<result<int, std::string_view>> y = just(success(5));
  static_assert(x == y.transpose());
}

TEST_CASE("unwrap_or_default()", "[maybe][unwrap_or_default]")
{
  constexpr maybe<std::string_view> x = nothing;
  static_assert(x.unwrap_or_default() == ""sv);
}

TEST_CASE("flatten()", "[maybe][flatten]")
{
  constexpr maybe<maybe<int>> x = just(just(6));
  static_assert(just(6) == x.flatten());

  constexpr maybe<maybe<int>> y = just(nothing);
  static_assert(nothing == y.flatten());

  constexpr maybe<maybe<int>> z = nothing;
  static_assert(nothing == z.flatten());

  // Flattening once only removes one level of nesting:
  constexpr maybe<maybe<maybe<int>>> nest = just(just(just(6)));
  static_assert(just(6) == nest.flatten().flatten());
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
  constexpr maybe<int> just1 = just(1);
  constexpr maybe<int> just2 = just(2);
  constexpr maybe<int> none = nothing;

  static_assert(!(just1 == just2));
  static_assert(!(just2 == just1));
  static_assert(just1 == just1);
  static_assert(just2 == just2);

  static_assert(!(just1 == 2));
  static_assert(!(just2 == 1));
  static_assert(just1 == 1);
  static_assert(just2 == 2);

  static_assert(!(1 == just2));
  static_assert(!(2 == just1));
  static_assert(1 == just1);
  static_assert(2 == just2);

  static_assert(!(just(1) == just2));
  static_assert(!(just(2) == just1));
  static_assert(just(1) == just1);
  static_assert(just(2) == just2);

  static_assert(!(just1 == just(2)));
  static_assert(!(just2 == just(1)));
  static_assert(just1 == just(1));
  static_assert(just2 == just(2));

  static_assert(!(just(1) == just(2)));
  static_assert(!(just(2) == just(1)));
  static_assert(just(1) == just(1));
  static_assert(just(2) == just(2));

  static_assert(none == none);
  static_assert(nothing == none);
  static_assert(none == nothing);
  static_assert(nothing == nothing);

  static_assert(!(none == just1));
  static_assert(!(none == just2));
  static_assert(!(none == 1));
  static_assert(!(none == 2));

  static_assert(!(1 == none));
  static_assert(!(2 == none));

  static_assert(!(just1 == none));
  static_assert(!(just2 == none));

  static_assert(!(nothing == just1));
  static_assert(!(nothing == just2));

  static_assert(!(none == just(1)));
  static_assert(!(none == just(2)));

  static_assert(!(nothing == just(1)));
  static_assert(!(nothing == just(2)));

  static_assert(!(just1 == none));
  static_assert(!(just2 == none));

  static_assert(!(just(1) == none));
  static_assert(!(just(2) == none));

  static_assert(!(just1 == nothing));
  static_assert(!(just2 == nothing));

  static_assert(!(just(1) == nothing));
  static_assert(!(just(2) == nothing));
}

TEST_CASE("not_equal", "[maybe][not_equal]")
{
  constexpr maybe<int> just1 = just(1);
  constexpr maybe<int> just2 = just(2);
  constexpr maybe<int> none = nothing;

  static_assert(just1 != just2);
  static_assert(just2 != just1);
  static_assert(!(just1 != just1));
  static_assert(!(just2 != just2));

  static_assert(just1 != 2);
  static_assert(just2 != 1);
  static_assert(!(just1 != 1));
  static_assert(!(just2 != 2));

  static_assert(1 != just2);
  static_assert(2 != just1);
  static_assert(!(1 != just1));
  static_assert(!(2 != just2));

  static_assert(just(1) != just2);
  static_assert(just(2) != just1);
  static_assert(!(just(1) != just1));
  static_assert(!(just(2) != just2));

  static_assert(just1 != just(2));
  static_assert(just2 != just(1));
  static_assert(!(just1 != just(1)));
  static_assert(!(just2 != just(2)));

  static_assert(just(1) != just(2));
  static_assert(just(2) != just(1));
  static_assert(!(just(1) != just(1)));
  static_assert(!(just(2) != just(2)));

  static_assert(!(none != none));
  static_assert(!(nothing != none));
  static_assert(!(none != nothing));
  static_assert(!(nothing != nothing));

  static_assert(none != just1);
  static_assert(none != just2);
  static_assert(none != 1);
  static_assert(none != 2);

  static_assert(1 != none);
  static_assert(2 != none);

  static_assert(just1 != none);
  static_assert(just2 != none);

  static_assert(nothing != just1);
  static_assert(nothing != just2);

  static_assert(none != just(1));
  static_assert(none != just(2));

  static_assert(nothing != just(1));
  static_assert(nothing != just(2));

  static_assert(just1 != none);
  static_assert(just2 != none);

  static_assert(just(1) != none);
  static_assert(just(2) != none);

  static_assert(just1 != nothing);
  static_assert(just2 != nothing);

  static_assert(just(1) != nothing);
  static_assert(just(2) != nothing);
}

TEST_CASE("less compare", "[maybe][less]")
{
  constexpr maybe<int> just1 = just(1);
  constexpr maybe<int> just2 = just(2);
  constexpr maybe<int> none = nothing;

  static_assert(just1 < just2);
  static_assert(!(just2 < just1));
  static_assert(!(just1 < just1));
  static_assert(!(just2 < just2));

  static_assert(just1 < 2);
  static_assert(!(just2 < 1));
  static_assert(!(just1 < 1));
  static_assert(!(just2 < 2));

  static_assert(1 < just2);
  static_assert(!(2 < just1));
  static_assert(!(1 < just1));
  static_assert(!(2 < just2));

  static_assert(just(1) < just2);
  static_assert(!(just(2) < just1));
  static_assert(!(just(1) < just1));
  static_assert(!(just(2) < just2));

  static_assert(just1 < just(2));
  static_assert(!(just2 < just(1)));
  static_assert(!(just1 < just(1)));
  static_assert(!(just2 < just(2)));

  static_assert(just(1) < just(2));
  static_assert(!(just(2) < just(1)));
  static_assert(!(just(1) < just(1)));
  static_assert(!(just(2) < just(2)));

  static_assert(!(none < none));
  static_assert(!(nothing < none));
  static_assert(!(none < nothing));
  static_assert(!(nothing < nothing));

  static_assert(none < just1);
  static_assert(none < just2);
  static_assert(none < 1);
  static_assert(none < 2);

  static_assert(!(1 < none));
  static_assert(!(2 < none));

  static_assert(!(just1 < none));
  static_assert(!(just2 < none));

  static_assert(nothing < just1);
  static_assert(nothing < just2);

  static_assert(none < just(1));
  static_assert(none < just(2));

  static_assert(nothing < just(1));
  static_assert(nothing < just(2));

  static_assert(!(just1 < none));
  static_assert(!(just2 < none));

  static_assert(!(just(1) < none));
  static_assert(!(just(2) < none));

  static_assert(!(just1 < nothing));
  static_assert(!(just2 < nothing));

  static_assert(!(just(1) < nothing));
  static_assert(!(just(2) < nothing));
}

TEST_CASE("less_or_equal compare", "[maybe][less_or_equal]")
{
  constexpr maybe<int> just1 = just(1);
  constexpr maybe<int> just2 = just(2);
  constexpr maybe<int> none = nothing;

  static_assert(just1 <= just2);
  static_assert(!(just2 <= just1));
  static_assert(just1 <= just1);
  static_assert(just2 <= just2);

  static_assert(just1 <= 2);
  static_assert(!(just2 <= 1));
  static_assert(just1 <= 1);
  static_assert(just2 <= 2);

  static_assert(1 <= just2);
  static_assert(!(2 <= just1));
  static_assert(1 <= just1);
  static_assert(2 <= just2);

  static_assert(just(1) <= just2);
  static_assert(!(just(2) <= just1));
  static_assert(just(1) <= just1);
  static_assert(just(2) <= just2);

  static_assert(just1 <= just(2));
  static_assert(!(just2 <= just(1)));
  static_assert(just1 <= just(1));
  static_assert(just2 <= just(2));

  static_assert(just(1) <= just(2));
  static_assert(!(just(2) <= just(1)));
  static_assert(just(1) <= just(1));
  static_assert(just(2) <= just(2));

  static_assert(none <= none);
  static_assert(nothing <= none);
  static_assert(none <= nothing);
  static_assert(nothing <= nothing);

  static_assert(none <= just1);
  static_assert(none <= just2);

  static_assert(none <= 1);
  static_assert(none <= 2);

  static_assert(nothing <= just1);
  static_assert(nothing <= just2);

  static_assert(none <= just(1));
  static_assert(none <= just(2));

  static_assert(nothing <= just(1));
  static_assert(nothing <= just(2));

  static_assert(!(just1 <= none));
  static_assert(!(just2 <= none));

  static_assert(!(1 <= none));
  static_assert(!(2 <= none));

  static_assert(!(just(1) <= none));
  static_assert(!(just(2) <= none));

  static_assert(!(just1 <= nothing));
  static_assert(!(just2 <= nothing));

  static_assert(!(just(1) <= nothing));
  static_assert(!(just(2) <= nothing));
}

TEST_CASE("greater compare", "[maybe][greater]")
{
  constexpr maybe<int> just1 = just(1);
  constexpr maybe<int> just2 = just(2);
  constexpr maybe<int> none = nothing;

  static_assert(!(just1 > just2));
  static_assert(just2 > just1);
  static_assert(!(just1 > just1));
  static_assert(!(just2 > just2));

  static_assert(!(just1 > 2));
  static_assert(just2 > 1);
  static_assert(!(just1 > 1));
  static_assert(!(just2 > 2));

  static_assert(!(1 > just2));
  static_assert(2 > just1);
  static_assert(!(1 > just1));
  static_assert(!(2 > just2));

  static_assert(!(just(1) > just2));
  static_assert(just(2) > just1);
  static_assert(!(just(1) > just1));
  static_assert(!(just(2) > just2));

  static_assert(!(just1 > just(2)));
  static_assert(just2 > just(1));
  static_assert(!(just1 > just(1)));
  static_assert(!(just2 > just(2)));

  static_assert(!(just(1) > just(2)));
  static_assert(just(2) > just(1));
  static_assert(!(just(1) > just(1)));
  static_assert(!(just(2) > just(2)));

  static_assert(!(none > none));
  static_assert(!(nothing > none));
  static_assert(!(none > nothing));
  static_assert(!(nothing > nothing));

  static_assert(!(none > just1));
  static_assert(!(none > just2));

  static_assert(!(none > 1));
  static_assert(!(none > 2));

  static_assert(!(nothing > just1));
  static_assert(!(nothing > just2));

  static_assert(!(none > just(1)));
  static_assert(!(none > just(2)));

  static_assert(!(nothing > just(1)));
  static_assert(!(nothing > just(2)));

  static_assert(just1 > none);
  static_assert(just2 > none);

  static_assert(1 > none);
  static_assert(2 > none);

  static_assert(just(1) > none);
  static_assert(just(2) > none);

  static_assert(just1 > nothing);
  static_assert(just2 > nothing);

  static_assert(just(1) > nothing);
  static_assert(just(2) > nothing);
}

TEST_CASE("greater_or_equal compare", "[maybe][greater_or_equal]")
{
  constexpr maybe<int> just1 = just(1);
  constexpr maybe<int> just2 = just(2);
  constexpr maybe<int> none = nothing;

  static_assert(!(just1 >= just2));
  static_assert(just2 >= just1);
  static_assert(just1 >= just1);
  static_assert(just2 >= just2);

  static_assert(!(just1 >= 2));
  static_assert(just2 >= 1);
  static_assert(just1 >= 1);
  static_assert(just2 >= 2);

  static_assert(!(1 >= just2));
  static_assert(2 >= just1);
  static_assert(1 >= just1);
  static_assert(2 >= just2);

  static_assert(!(just(1) >= just2));
  static_assert(just(2) >= just1);
  static_assert(just(1) >= just1);
  static_assert(just(2) >= just2);

  static_assert(!(just1 >= just(2)));
  static_assert(just2 >= just(1));
  static_assert(just1 >= just(1));
  static_assert(just2 >= just(2));

  static_assert(!(just(1) >= just(2)));
  static_assert(just(2) >= just(1));
  static_assert(just(1) >= just(1));
  static_assert(just(2) >= just(2));

  static_assert(none >= none);
  static_assert(nothing >= none);
  static_assert(none >= nothing);
  static_assert(nothing >= nothing);

  static_assert(!(nothing >= just1));
  static_assert(!(nothing >= just2));

  static_assert(!(none >= just1));
  static_assert(!(none >= just2));

  static_assert(!(none >= 1));
  static_assert(!(none >= 2));

  static_assert(!(none >= just(1)));
  static_assert(!(none >= just(2)));

  static_assert(!(nothing >= just(1)));
  static_assert(!(nothing >= just(2)));

  static_assert(just1 >= none);
  static_assert(just2 >= none);

  static_assert(1 >= none);
  static_assert(2 >= none);

  static_assert(just(1) >= none);
  static_assert(just(2) >= none);

  static_assert(just1 >= nothing);
  static_assert(just2 >= nothing);

  static_assert(just(1) >= nothing);
  static_assert(just(2) >= nothing);
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
      ss << maybe<std::map<str, int>>(
          just(std::map<str, int>{ { "foo"s, 1 } })
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
  constexpr std::array v{ 1, 2, 3 };
  constexpr maybe x = range_to_maybe(v);
  static_assert(x == just(1));

  constexpr maybe y = range_to_maybe(std::array<int, 0>{});
  static_assert(y == nothing);

  constexpr int a[] = { 1 };
  constexpr maybe z = range_to_maybe(a);
  static_assert(z == 1);
}

#include <mitama/boolinators.hpp>

TEST_CASE("as_maybe test", "[maybe][as_maybe][boolinators]")
{
  constexpr maybe x = as_maybe(true);
  static_assert(x == just(std::monostate{}));
  constexpr maybe y = as_maybe(false);
  static_assert(y == nothing);
}

TEST_CASE("as_just test", "[maybe][as_just][boolinators]")
{
  constexpr maybe x = as_just(true, 1);
  static_assert(x == just(1));
  constexpr maybe y = as_just(false, 1);
  static_assert(y == nothing);
}

TEST_CASE("as_just_from test", "[maybe][as_just_from][boolinators]")
{
  constexpr maybe x = as_just_from(true, [] { return 1; });
  static_assert(x == just(1));
  constexpr maybe y = as_just_from(false, [] { return 1; });
  static_assert(y == nothing);
}
