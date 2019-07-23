#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <mitama/result/result.hpp>
#include <mitama/maybe/maybe.hpp>

#include <boost/xpressive/xpressive.hpp>

#include <string>

using namespace mitama;
using namespace std::string_literals;

TEST_CASE("is_just()", "[maybe][is_just]"){
  maybe<int> x = just(2);
  REQUIRE( x.is_just() );

  maybe<int> y = nothing;
  REQUIRE_FALSE( y.is_just() );
}

TEST_CASE("is_nothing()", "[maybe][is_nothing]"){
  maybe<int> x = just(2);
  REQUIRE_FALSE( x.is_nothing() );

  maybe<int> y = nothing;
  REQUIRE( y.is_nothing() );
}

TEST_CASE("unwrap()", "[maybe][unwrap]"){
  {
    maybe x = just("air"s);
    REQUIRE(x.unwrap() == "air"s);
  }
  try {
    maybe<int> x = nothing;
    x.unwrap(); // panics
  }
  catch (runtime_panic const &p)
  {
    using namespace boost::xpressive;
    sregex re =
        as_xpr(
            "runtime panicked at 'called `maybe::unwrap()` on a `nothing` value', ") >>
        *_ >> as_xpr(":") >> +range('0', '9');
    smatch what;
    REQUIRE(regex_match(std::string{p.what()}, what, re));
  }
}

TEST_CASE("expect()", "[maybe][expect]"){
  {
    maybe x = just("air"s);
    REQUIRE(x.expect("the world is ending") == "air"s);
  }
  try {
    maybe<int> x = nothing;
    x.expect("the world is ending"); // panics
  }
  catch (runtime_panic const &p)
  {
    using namespace boost::xpressive;
    sregex re =
        as_xpr(
            "runtime panicked at 'the world is ending', ") >>
        *_ >> as_xpr(":") >> +range('0', '9');
    smatch what;
    REQUIRE(regex_match(std::string{p.what()}, what, re));
  }
}

TEST_CASE("unwrap_or()", "[maybe][unwrap_or]"){
  REQUIRE(maybe{just("car"s)}.unwrap_or("bike"s) == "car"s);
  REQUIRE(maybe<std::string>{nothing}.unwrap_or("bike"s) == "bike"s);
}

TEST_CASE("unwrap_or_else()", "[maybe][unwrap_or_else]"){
  int k = 10;
  REQUIRE(maybe{just(4)}.unwrap_or_else([k]{ return 2 * k; }) == 4);
  REQUIRE(maybe<int>{}.unwrap_or_else([k]{ return 2 * k; }) == 20);
}

TEST_CASE("map()", "[maybe][map]"){

  maybe maybe_some_string = just("Hello, World!"s);
  // `maybe::map` takes self *by ref*,
  // *not* consuming `maybe_some_string`
  maybe maybe_some_len = maybe_some_string.map(&std::string::size);

  REQUIRE(maybe_some_len == just(13u));

}

TEST_CASE("map_or()", "[maybe][map_or]"){

  maybe x = just("foo"s);
  REQUIRE(x.map_or(42, &std::string::size) == 3);

  maybe<std::string> y = nothing;
  REQUIRE(y.map_or(42, &std::string::size) == 42);

}

TEST_CASE("map_or_else()", "[maybe][map_or_else]"){

  int k = 21;

  maybe x = just("foo"s);
  REQUIRE(x.map_or_else([k]{ return 2 * k; }, &std::string::size) == 3);

  maybe<std::string> y = nothing;
  REQUIRE(y.map_or_else([k]{ return 2 * k; }, &std::string::size) == 42);

}

TEST_CASE("ok_or()", "[maybe][ok_or]"){

  maybe x = just("foo"s);
  REQUIRE(x.ok_or(0) == success("foo"s));

  maybe<std::string> y = nothing;
  REQUIRE(y.ok_or(0) == failure(0));

  REQUIRE(y.ok_or() == failure<>());

}

TEST_CASE("ok_or_else()", "[maybe][ok_or_else]"){

  maybe x = just("foo"s);
  REQUIRE(x.ok_or_else([]{ return 0; }) == success("foo"s));

  maybe<std::string> y = nothing;
  REQUIRE(y.ok_or_else([]{ return 0; }) == failure(0));

}

TEST_CASE("and_then()", "[maybe][and_then]"){

  auto sq = [](int x) -> maybe<int> { return just(x * x); };
  auto nope = [](...) -> maybe<int> { return nothing; };

  REQUIRE(maybe{just(2)}.and_then(sq).and_then(sq) == just(16));
  REQUIRE(maybe{just(2)}.and_then(sq).and_then(nope) == nothing);
  REQUIRE(maybe{just(2)}.and_then(nope).and_then(sq) == nothing);
  REQUIRE(nope().and_then(sq).and_then(sq) == nothing);
}

TEST_CASE("filter()", "[maybe][filter]"){

  auto is_even = [](int n) -> bool {
    return n % 2 == 0;
  };

  REQUIRE(maybe<int>{}.filter(is_even) == nothing);
  REQUIRE(maybe{just(3)}.filter(is_even) == nothing);
  REQUIRE(maybe{just(4)}.filter(is_even) == just(4));

}

TEST_CASE("or_else()", "[maybe][or_else]"){

  auto nobody = []() -> maybe<std::string> { return nothing; };
  auto vikings = []() -> maybe<std::string> { return just("vikings"s); };

  REQUIRE(maybe{just("barbarians"s)}.or_else(vikings) == just("barbarians"s));
  REQUIRE(maybe<std::string>{}.or_else(vikings) == just("vikings"s));
  REQUIRE(maybe<std::string>{}.or_else(nobody) == nothing);

}

TEST_CASE("get_or_insert()", "[maybe][get_or_insert]"){
  GIVEN("a nothing type of maybe<int>.") {
    maybe<int> x = nothing;
    WHEN("call get_or_insert with value `5` and bind to `y`."){
      auto& y = x.get_or_insert(5);
      REQUIRE(y == 5);
      WHEN("assign 7 to `y`") {
        y = 7;
        THEN("changed x to just(7)"){
          REQUIRE(x == just(7));
        }
      }
    }
  }
}

TEST_CASE("get_or_insert_with()", "[maybe][get_or_insert_with]"){
  GIVEN("a nothing type of maybe<int>.") {
    maybe<int> x = nothing;
    WHEN("call get_or_insert_with and bind to `y`."){
      auto& y = x.get_or_insert_with([]{ return 5; });
      REQUIRE(y == 5);
      WHEN("assign 7 to `y`") {
        y = 7;
        THEN("changed x to just(7)"){
          REQUIRE(x == just(7));
        }
      }
    }
  }
}

TEST_CASE("replace()", "[maybe][replace]"){
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

TEST_CASE("transpose()", "[maybe][transpose]"){

  result<maybe<int>, std::string> x = success(just(5));
  maybe<result<int, std::string>> y = just(success(5));
  REQUIRE(x == y.transpose());

}

TEST_CASE("unwrap_or_default()", "[maybe][unwrap_or_default]"){

  maybe<std::string> x = nothing;
  REQUIRE(x.unwrap_or_default() == ""s);

}

TEST_CASE("flatten()", "[maybe][flatten]"){

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

TEST_CASE("and_finally()", "[maybe][and_finally]"){

  std::string hook = "default";
  maybe<std::string> x = nothing;
  x.and_finally([&hook](std::string v){
    hook = v;
  });
  REQUIRE(hook == "default"s);

  maybe y = just("error"s);
  y.and_finally([&hook](std::string v){
    hook = v;
  });
  REQUIRE(hook == "error"s);

}

TEST_CASE("less compare", "[maybe][less]"){
  maybe<int> just1 = just(1);
  maybe<int> just2 = just(2);
  maybe<int> none1 = nothing;
  maybe<int> none2 = nothing;

  REQUIRE(just1 < just2);
  REQUIRE_FALSE(just2 < just1);
  REQUIRE_FALSE(just1 < just1);
  REQUIRE_FALSE(just2 < just2);

  REQUIRE_FALSE(none1 < none2);
  REQUIRE_FALSE(none2 < none1);
  REQUIRE_FALSE(none1 < none1);
  REQUIRE_FALSE(none2 < none2);

  REQUIRE(none1 < just1);
  REQUIRE(none1 < just2);
  REQUIRE(none2 < just1);
  REQUIRE(none2 < just2);

  REQUIRE_FALSE(just1 < none1);
  REQUIRE_FALSE(just1 < none2);
  REQUIRE_FALSE(just2 < none1);
  REQUIRE_FALSE(just2 < none2);

}

TEST_CASE("less_or_equal compare", "[maybe][less_or_equal]"){
  maybe<int> just1 = just(1);
  maybe<int> just2 = just(2);
  maybe<int> none1 = nothing;
  maybe<int> none2 = nothing;

  REQUIRE(just1 <= just2);
  REQUIRE_FALSE(just2 <= just1);
  REQUIRE(just1 <= just1);
  REQUIRE(just2 <= just2);

  REQUIRE(none1 <= none2);
  REQUIRE(none2 <= none1);
  REQUIRE(none1 <= none1);
  REQUIRE(none2 <= none2);

  REQUIRE(none1 <= just1);
  REQUIRE(none1 <= just2);
  REQUIRE(none2 <= just1);
  REQUIRE(none2 <= just2);

  REQUIRE_FALSE(just1 <= none1);
  REQUIRE_FALSE(just1 <= none2);
  REQUIRE_FALSE(just2 <= none1);
  REQUIRE_FALSE(just2 <= none2);

}

TEST_CASE("greater compare", "[maybe][greater]"){
  maybe<int> just1 = just(1);
  maybe<int> just2 = just(2);
  maybe<int> none1 = nothing;
  maybe<int> none2 = nothing;

  REQUIRE_FALSE(just1 > just2);
  REQUIRE(just2 > just1);
  REQUIRE_FALSE(just1 > just1);
  REQUIRE_FALSE(just2 > just2);

  REQUIRE_FALSE(none1 > none2);
  REQUIRE_FALSE(none2 > none1);
  REQUIRE_FALSE(none1 > none1);
  REQUIRE_FALSE(none2 > none2);

  REQUIRE_FALSE(none1 > just1);
  REQUIRE_FALSE(none1 > just2);
  REQUIRE_FALSE(none2 > just1);
  REQUIRE_FALSE(none2 > just2);

  REQUIRE(just1 > none1);
  REQUIRE(just1 > none2);
  REQUIRE(just2 > none1);
  REQUIRE(just2 > none2);

}

TEST_CASE("greater_or_equal compare", "[maybe][greater_or_equal]"){
  maybe<int> just1 = just(1);
  maybe<int> just2 = just(2);
  maybe<int> none1 = nothing;
  maybe<int> none2 = nothing;

  REQUIRE_FALSE(just1 >= just2);
  REQUIRE(just2 >= just1);
  REQUIRE(just1 >= just1);
  REQUIRE(just2 >= just2);

  REQUIRE(none1 >= none2);
  REQUIRE(none2 >= none1);
  REQUIRE(none1 >= none1);
  REQUIRE(none2 >= none2);

  REQUIRE_FALSE(none1 >= just1);
  REQUIRE_FALSE(none1 >= just2);
  REQUIRE_FALSE(none2 >= just1);
  REQUIRE_FALSE(none2 >= just2);

  REQUIRE(just1 >= none1);
  REQUIRE(just1 >= none2);
  REQUIRE(just2 >= none1);
  REQUIRE(just2 >= none2);

}
