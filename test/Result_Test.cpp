#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <result/result.hpp>
#include <utest_utility/is_invalid_expr.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/type_index.hpp>
#include <functional>
#include <cstdint>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <cassert>
#include <memory>
#include <array>

using namespace boost::xpressive;
using namespace mitama;
using namespace std::string_literals;
using namespace std::string_view_literals;
namespace hana = boost::hana;
using boost::lambda::_1;
using boost::lambda::_2;

using str = std::string;
using u32 = std::uint32_t;
using i32 = std::int32_t;

std::vector<std::string> split_impl(std::string &&s, std::regex &&r)
{
  return {std::sregex_token_iterator{s.begin(), s.end(), r, -1},
          std::sregex_token_iterator{}};
}

auto split(std::string s, std::string pattern)
{
  return split_impl(std::move(s), std::regex{pattern});
}

template <class T>
auto parse = [](str s) -> result<T, str> {
  try
  {
    if (std::is_integral_v<T>){
      using namespace boost::xpressive;
      sregex re1 = +range('0', '9');
      smatch what;
      if (!regex_match(s, what, re1))
        return Err("parse error at string: "s + s);
      if constexpr (std::is_unsigned_v<T>) {
        sregex re2 = as_xpr("-") >> +range('0', '9');
        if (regex_match(s, what, re1))
          return Err("negative value: "s + s);
      };
      if constexpr (std::is_same_v<T, int>)
      {
        return Ok{std::stoi(s)};
      }
      else if constexpr (std::is_same_v<T, long>)
      {
        return Ok{std::stol(s)};
      }
      else if constexpr (std::is_same_v<T, unsigned long>)
      {
        return Ok{std::stoul(s)};
      }
      else if constexpr (std::is_same_v<T, long long>)
      {
        return Ok{std::stoll(s)};
      }
      else if constexpr (std::is_same_v<T, unsigned long long>)
      {
        return Ok{std::stoull(s)};
      }
    }
    else if constexpr (std::is_floating_point_v<T>){
      if constexpr (std::is_same_v<T, float>)
      {
        return Ok{std::stof(s)};
      }
      else if constexpr (std::is_same_v<T, double>)
      {
        return Ok{std::stod(s)};
      }
      else if constexpr (std::is_same_v<T, long double>)
      {
        return Ok{std::stold(s)};
      }
    }
    else
    {
      static_assert(mitama::always_true_v<T>);
    }
  }
  catch (std::invalid_argument const &e)
  {
    return Err{e.what()};
  }
};

TEST_CASE("is_ok() test", "[result][is_ok]"){
  result<u32, str> x = Ok(-3);
  REQUIRE(x.is_ok() == true);

  result<u32, str> y = Err("Some error message"s);
  REQUIRE(y.is_ok() == false);
}

TEST_CASE("is_err() test", "[result][is_err]"){
  result<u32, str> x = Ok(-3);
  REQUIRE(x.is_err() == false);

  result<u32, str> y = Err("Some error message"s);
  REQUIRE(y.is_err() == true);
}

TEST_CASE("ok() test", "[result][ok]"){
  result<u32, str> x = Ok(2);
  REQUIRE(x.err() == None);

  result<int, str> y = Err("Nothing here"s);
  REQUIRE(y.err() == Some("Nothing here"s));
}

TEST_CASE("err() test", "[result][err]"){
  result<u32, str> x = Ok(2);
  REQUIRE(x.err() == None);

  result<u32, str> y = Err("Nothing here"s);
  REQUIRE(y.err() == Some("Nothing here"s));
}

TEST_CASE("map() test", "[result][map]"){
  std::string line = "1,3,5,7";

  for (auto num : split(line, ","))
  {
    if (auto res = parse<i32>(num).map(_1 * 2); res.is_ok())
    {
      REQUIRE(res.ok().value() % 2 == 0);
    }
  }
}

TEST_CASE("map_or_else(F, M) test", "[result][map_or_else]"){
  auto k = 21;
  {
    result<str, str> x = Ok("foo"s);
    REQUIRE(x.map_or_else([k](auto){ return k * 2; }, [](auto v) { return v.length(); }) == 3);
  }
  {
    result<str, str> x = Err("bar"s);
    REQUIRE(x.map_or_else([k](auto){ return k * 2; }, [](auto v) { return v.length(); }) == 42);
  }
}


TEST_CASE("map_err() test", "[result][map_err]"){
  auto stringify = [](u32 x) -> str {
    return "error code: "s + std::to_string(x);
  };

  result<u32, u32> x = Ok(2);
  REQUIRE(x.map_err(stringify) == Ok(2u));

  result<u32, u32> y = Err(13);
  REQUIRE(y.map_err(stringify) == Err("error code: 13"s));
}

TEST_CASE("operator&& test", "[result][and]"){
  {
    result<u32, str> x = Ok(2);
    result<str, str> y = Err("late error"s);
    REQUIRE((x && y) == Err("late error"s));
  }

  {
    result<u32, str> x = Err("early error"s);
    result<str, str> y = Ok("foo"s);
    REQUIRE((x && y) == Err("early error"s));
  }
  {
    result<u32, str> x = Err("not a 2"s);
    result<str, str> y = Err("late error"s);
    REQUIRE((x && y) == Err("not a 2"s));
  }

  {
    result<u32, str> x = Ok(2);
    result<str, str> y = Ok("different result type"s);
    REQUIRE((x && y) == Ok("different result type"s));
  }
}

TEST_CASE("and_then() test", "[result][and_then]"){
  auto sq = [](u32 x) -> result<u32, u32> { return Ok(x * x); };
  auto err = [](u32 x) -> result<u32, u32> { return Err(x); };

  REQUIRE(Ok(2u).and_then(sq).and_then(sq) == Ok(16u));
  REQUIRE(Ok(2u).and_then(sq).and_then(err) == Err(4u));
  REQUIRE(Ok(2u).and_then(err).and_then(sq) == Err(2u));
  REQUIRE(Err(3u).and_then(sq).and_then(sq) == Err(3u));
  REQUIRE(Err(3u).and_then(sq).and_then(sq) == Err(3u));
}

TEMPLATE_TEST_CASE("is_result_with_v meta test", "[is_result_with_v][and_then][meta]",
                    int, unsigned, std::string, std::vector<int>)
{
  REQUIRE(is_result_with_v<mitama::result<int, TestType>, mitama::Err<TestType>>);
  REQUIRE(!is_result_with_v<result<unsigned, std::vector<TestType>>, mitama::Err<TestType>>);
}

// TEMPLATE_TEST_CASE("and_then() meta test", "[result][and_then][meta]",
//                     int, unsigned, std::string, std::vector<int>)
// {
//   REQUIRE(!IS_INVALID_EXPR(DECLVAL(0).and_then(DECLVAL(1)))(result<double, TestType>, std::function<result<float, TestType>(unsigned)>));
//   REQUIRE(IS_INVALID_EXPR(std::declval<result<double, TestType>>().and_then(DECLVAL(0)))(std::function<result<TestType, float>(unsigned)>));
// }

TEST_CASE("operator|| test", "[result][or]"){
  {
    result<u32, str> x = Ok(2);
    result<u32, str> y = Err("late error"s);
    REQUIRE((x || y) ==  Ok(2u));
  }
  {
    result<u32, str> x = Err("early error"s);
    result<u32, str> y = Ok(2);
    REQUIRE((x || y) ==  Ok(2u));
  }
  {
    result<u32, str> x = Err("not a 2"s);
    result<u32, str> y = Err("late error"s);
    REQUIRE((x || y) ==  Err("late error"s));
  }
  {
    result<u32, str> x = Ok(2);
    result<u32, str> y = Ok(100);
    REQUIRE((x || y) ==  Ok(2u));
  }
}

TEST_CASE("or_else() test", "[result][or_else]"){
  auto sq = [](u32 x) -> result<u32, u32> { return Ok(x * x); };
  auto err = [](u32 x) -> result<u32, u32> { return Err(x); };

  REQUIRE(Ok(2).or_else(sq).or_else(sq) ==  Ok(2));
  REQUIRE(Ok(2).or_else(err).or_else(sq) ==  Ok(2));
  REQUIRE(Err(3).or_else(sq).or_else(err) ==  Ok(9u));
  REQUIRE(Err(3).or_else(err).or_else(err) ==  Err(3u));
}

TEST_CASE("unwrap_or() test", "[result][unwrap_or]"){
  result<u32, u32> err = Err(2);
  result<u32, u32> ok = Ok(2);

  REQUIRE(ok.unwrap_or(1u) ==  2u);
  REQUIRE(err.unwrap_or(1u) ==  1u);
}

TEST_CASE("unwrap_or_else() test", "[result][unwrap_or_else]"){
  auto count = [](str x) -> size_t { return x.size(); };

  REQUIRE(Ok(2).unwrap_or_else(count) ==  2);
  REQUIRE(Err("foo"s).unwrap_or_else(count) ==  3ull);
  REQUIRE(Err("foo"s).unwrap_or_else([]{ return 3ull; }) ==  3ull);
}

TEST_CASE("unwrap() test", "[result][unwrap]"){
  {
    result<u32, str> x = Ok(2);
    REQUIRE(x.unwrap() ==  2u);
  }
  try {
    result<u32, str> x = Err("emergency failure"s);
    x.unwrap(); // panics with `emergency failure`
  }
  catch (runtime_panic const &p)
  {
    using namespace boost::xpressive;
    sregex re =
        as_xpr(
            R"(runtime panicked at 'called `basic_result::unwrap() on an `Err` value: emergency failure', )") >>
        *_ >> as_xpr(":") >> +range('0', '9');
    smatch what;
    std::cout << p.what() << std::endl;
    REQUIRE(regex_match(std::string{p.what()}, what, re));
  }
}

TEST_CASE("unwrap_err() test", "[result][unwrap_err]"){
  try {
    result<u32, str> x = Ok(2);
    x.unwrap_err(); // panics with `2`
  }
  catch (runtime_panic const &p)
  {
    using namespace boost::xpressive;
    sregex re =
        as_xpr(
            R"(runtime panicked at 'called `basic_result::unwrap_err() on an `Ok` value: 2', )") >>
        *_ >> as_xpr(":") >> +range('0', '9');
    smatch what;
    std::cout << p.what() << std::endl;
    REQUIRE(regex_match(std::string{p.what()}, what, re));
  }

  {
    result<u32, str> x = Err("emergency failure"s);
    REQUIRE(x.unwrap_err() ==  "emergency failure"s);
  }
}

TEST_CASE("unwrap_or_default() test", "[result][unwrap_or_default]"){
  auto good_year_from_input = "1909"s;
  auto bad_year_from_input = "190blarg"s;
  auto good_year = parse<int>(good_year_from_input).unwrap_or_default();
  auto bad_year = parse<int>(bad_year_from_input).unwrap_or_default();

  REQUIRE(1909 ==  good_year);
  REQUIRE(0 ==  bad_year);
}

TEST_CASE("transpose() test", "[result][transpose]"){
  result<boost::optional<i32>, std::monostate> x = Ok(Some(5));
  boost::optional<result<i32, std::monostate>> y = Some(result<i32, std::monostate>(Ok(5)));
  
  REQUIRE(x.transpose() == y);
}

TEST_CASE("basics test", "[result][basics]"){
  auto even = [](u32 u) -> result<u32, str> {
    if (u % 2 == 0)
      return Ok(u);
    else
      return Err("odd"s);
  };
  auto func = [](auto u) -> result<u32, str> { if(u%3==0) return Ok(1u); else return Err("error"s); };
  REQUIRE(even(2).and_then(func) ==  Err("error"s));
  REQUIRE(even(2) ==  Ok(2u));
}

TEST_CASE("constructors and assignments test", "[result][constructors][assignments]"){
  auto res = result<int,int>{Ok(2)};
  res = result<int, int>{Err(2)};
  (void)result<std::string, double>{in_place_ok, "hoge"};
  (void)result<double, std::string>{in_place_err, "hoge"};
  (void)result<std::vector<int>, double>{in_place_ok, {1, 2, 3, 4}};
  (void)result<double, std::vector<double>>{in_place_err, {1., 2., 3., 4.}};

  res = Ok(1);
  res = Err(2);
}

TEST_CASE("format test", "[result][format]"){
  SECTION("Ok"){
    using namespace std::literals;
    std::stringstream ss;
    ss << Ok(1);
    REQUIRE(ss.str() ==  "Ok(1)"s);
  }
  SECTION("Err"){
    using namespace std::literals;
    std::stringstream ss;
    ss << Err(1);
    REQUIRE(ss.str() ==  "Err(1)"s);
  }
  SECTION("result ok"){
    using namespace std::literals;
    std::stringstream ss;
    ss << result<int, std::string>{Ok(1)};
    REQUIRE(ss.str() ==  "Ok(1)"s);
  }
  SECTION("result err"){
    using namespace std::literals;
    std::stringstream ss;
    ss << result<int, std::string>{Err("hoge"s)};
    REQUIRE(ss.str() ==  "Err(hoge)"s);
  }
  SECTION("result of range ok"){
    using namespace std::literals;
    std::stringstream ss;
    ss << Ok(std::vector<std::string>{"foo"s, "bar"s});
    REQUIRE(ss.str() == "Ok([foo,bar])"s);
  }
  SECTION("result of range err"){
    using namespace std::literals;
    std::stringstream ss;
    ss << Err(std::vector<std::string>{"foo"s, "bar"s});
    REQUIRE(ss.str() == "Err([foo,bar])"s);
  }
  SECTION("Err"){
    using namespace std::literals;
    std::stringstream ss;
    ss << Err("foo"s);
    REQUIRE(ss.str() ==  "Err(foo)"s);
  }
  SECTION("replace"){
    using namespace std::literals;
    auto res = result<int, std::vector<int>>{in_place_err, {1,2,3}};
    REQUIRE((boost::format("%1%") % res).str() == "Err([1,2,3])"s);
    res = Ok(1);
    REQUIRE((boost::format("%1%") % res).str() ==  "Ok(1)"s);
  }
}

TEST_CASE("monostate Ok test", "[result][monostate]"){
  auto func = []() -> result<std::monostate, std::string> {
    if (false) return Err<std::string>("hoge"s);
    return Ok<>{};
  };

  REQUIRE(func().is_ok());
}

TEST_CASE("monostate Err test", "[result][monostate]"){
  auto func = []() -> result</*defaulted monostate*/> {
    if (false) return Ok<>{};
    return Err<>();
  };
  REQUIRE(func().is_err());
}

TEST_CASE("contextually convertible to bool", "[result]"){
  auto err_func = []() -> result</*defaulted monostate*/> {
    if (false) return Err<>{};
    return Err<>();
  };
  auto ok_func = []() -> result<std::monostate, std::string> {
    if (false) return Err<std::string>("hoge"s);
    return Ok<>{};
  };
  REQUIRE(!err_func());
  REQUIRE(ok_func());
}

SCENARIO("test for reference type", "[result][ref]"){
  using namespace std::literals;
  GIVEN( "A result that refer to some string" ) {
    str hoge = "foo";
    result<str&, str&> res(Ok{hoge});

    REQUIRE( hoge == "foo"s );
    REQUIRE( res.unwrap() == "foo"s );
    REQUIRE( &(res.unwrap()) == &hoge );

    WHEN( "the result is overwritten" ) {
      res.unwrap() = "bar";

      THEN( "the referred string change" ) {
        REQUIRE( hoge == "bar"s );
        REQUIRE( res.unwrap() == "bar"s );
      }
    }
  }
}

SCENARIO("test for as_ref", "[result][as_ref]"){
  using namespace std::literals;
  GIVEN( "A new result, containing a reference into the original" ) {
    result<str, str> res(Ok{"foo"s});
    auto ref /* result<str&, str&> */ = res.as_ref();

    REQUIRE( res == ref );
    REQUIRE( res == Ok("foo"s) );
    REQUIRE( ref == Ok("foo"s) );

    WHEN( "The new result is overwritten" ) {
      ref.unwrap() = "bar"s;

      THEN( "the original result change" ) {
        REQUIRE( res == ref );
        REQUIRE( res == Ok("bar"s) );
        REQUIRE( ref == Ok("bar"s) );
      }
    }
  }
}

SCENARIO("test for deref", "[result][deref]"){
  using namespace std::literals;
  GIVEN( "A new result, containing a indirect reference into the original" ) {
    auto ptr = std::make_shared<int>(42);
    result<std::shared_ptr<int>, std::shared_ptr<int>> res(Ok{ptr});
    auto deref = res.deref();

    REQUIRE( *res.unwrap() == deref.unwrap() );
    REQUIRE( deref == Ok(42) );

    WHEN( "The new result is overwritten" ) {
      deref.unwrap() = 57;

      THEN( "the original result change" ) {
        REQUIRE( *ptr == 57 );
        REQUIRE( deref == Ok(57) );
      }
    }
  }
}

SCENARIO("test for dangling deref", "[result][deref][dangling]"){
  using namespace std::literals;
  using vec_iter = typename std::vector<int>::iterator;
  GIVEN( "A new result which is containing a dangling reference into the discarded vector" ) {
    auto deref = result<vec_iter, vec_iter>(Ok{std::vector<int>{1,3}.begin()}).deref();

    REQUIRE( std::is_same_v<decltype(deref.unwrap()), dangling<int&>> );
    // deref.unwrap().transmute()
    // ^~~~~~~~~~~~~~~~~~~~~~~~~~ Undefined Behavior!
  }
  GIVEN( "A new result which is containing a reference into the living vector" ) {
    std::vector<int> vec{1,3};
    auto deref = result<vec_iter, vec_iter>(Ok{vec.begin()}).deref();

    REQUIRE( deref.unwrap().transmute() == 1 );
    //       ^~~~~~~~~~~~~~~~~~~~~~~~~~ OK! 
  }
}
