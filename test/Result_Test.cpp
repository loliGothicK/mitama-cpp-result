#include "../include/Result.hpp"
#include <boost/hana/assert.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <cstdint>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <cassert>
#include <array>

#define ASSERT_SHOULD_BE_INVALID_EXPR(...) ASSERT_SHOULD_BE_INVALID_EXPR_IMPL1(__VA_ARGS__)ASSERT_SHOULD_BE_INVALID_EXPR_IMPL2

#define ASSERT_SHOULD_BE_INVALID_EXPR_IMPL1(...) static_assert(false ? ::mitama::make_overload([](auto x)->decltype(__VA_ARGS__, std::false_type{}){return{};},[](...)->std::true_type{return{};})
#define ASSERT_SHOULD_BE_INVALID_EXPR_IMPL2(...) (::mitama::type_transfer<__VA_ARGS__>{}): ::mitama::protean_bool{})
#define DECLVAL std::declval<typename decltype(x)::type>()

using namespace boost::xpressive;
using namespace mitama;
using namespace std::string_literals;
using namespace std::string_view_literals;
namespace hana = boost::hana;
using boost::lambda::_1;
using boost::lambda::_2;

#define assert_eq(A,B) assert([](auto&& x, auto&& y){ return x == y; }(A, B))
#define assert_true(A) assert(A)
#define assert_false(A) assert(!(A))


template <class T, class=void>
struct is_complete_type: std::false_type {};

template <class T>
struct is_complete_type<T,std::void_t<decltype(sizeof(T))>>
    : std::true_type {};

template < class T >
inline constexpr bool is_complete_type_v = is_complete_type<T>::value;

/*
 * compile time type traits checking
 */

// memory exhausted
// #include "special_member_tests.hpp"

struct Complete{};
struct Incomplete;

static_assert(is_complete_type_v<Result<Complete, Complete>>);
static_assert(!is_complete_type_v<Result<Incomplete, Complete>>);
static_assert(!is_complete_type_v<Result<Complete, Incomplete>>);
static_assert(!is_complete_type_v<Result<Incomplete, Incomplete>>);

struct DestructibleType{};
struct NonDestructibleType{ ~NonDestructibleType() = delete; };

static_assert(is_complete_type_v<Result<DestructibleType, DestructibleType>>);
static_assert(!is_complete_type_v<Result<NonDestructibleType, DestructibleType>>);
static_assert(!is_complete_type_v<Result<DestructibleType, NonDestructibleType>>);
static_assert(!is_complete_type_v<Result<NonDestructibleType, NonDestructibleType>>);


struct DefaultConstructibleType
{
};
struct NonDefaultConstructibleType{
  NonDefaultConstructibleType() = delete;
  NonDefaultConstructibleType(const NonDefaultConstructibleType &) = default;
  NonDefaultConstructibleType(NonDefaultConstructibleType &&) = default;
  NonDefaultConstructibleType &operator=(const NonDefaultConstructibleType &) = default;
  NonDefaultConstructibleType &operator=(NonDefaultConstructibleType &&) = default;
  ~NonDefaultConstructibleType() = default;
};

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
auto parse = [](str s) -> Result<T, str> {
  try
  {
    if (std::is_integral_v<T>){
      using namespace boost::xpressive;
      sregex re1 = +range('0', '9');
      smatch what;
      if (!regex_match(s, what, re1))
        return Err("parse error at string: " + s);
      if constexpr (std::is_unsigned_v<T>) {
        sregex re2 = as_xpr("-") >> +range('0', '9');
        if (regex_match(s, what, re1))
          return Err("negative value: " + s);
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

int main(){

{
  Result<u32, str> x = Ok(-3);
  assert_eq(x.is_ok(), true);

  Result<u32, str> y = Err("Some error message");
  assert_eq(y.is_ok(), false);
  std::cout << "is_ok test passed !\n";
}
{
  Result<u32, str> x = Ok(-3);
  assert_eq(x.is_err(), false);

  Result<u32, str> y = Err("Some error message");
  assert_eq(y.is_err(), true);
  std::cout << "is_err test passed !\n";
}
{
  Result<u32, str> x = Ok(2);
  assert_eq(x.err(), None);

  Result<u32, str> y = Err("Nothing here");
  assert_eq(y.err(), Some("Nothing here"));
  std::cout << "ok test passed !\n";
}
{
  Result<u32, str> x = Ok(2);
  assert_eq(x.err(), None);

  Result<u32, str> y = Err("Nothing here");
  assert_eq(y.err(), Some("Nothing here"));
  std::cout << "err test passed !\n";
}
{
  std::string line = "1,3,5,7";

  for (auto num : split(line, ","))
  {
    if (auto res = parse<i32>(num).map(_1 * 2); res.is_ok())
    {
      assert_true(res.ok().value() % 2 == 0);
    }
  }
  std::cout << "map test passed !\n";
}
{
  auto stringify = [](u32 x) -> str {
    return "error code: "s + std::to_string(x);
  };

  Result<u32, u32> x = Ok(2);
  assert_eq(x.map_err(stringify), Ok(2u));

  Result<u32, u32> y = Err(13);
  assert_eq(y.map_err(stringify), Err("error code: 13"s));
  std::cout << "map_err test passed !\n";
}
{
  {
    Result<u32, str> x = Ok(2);
    Result<str, str> y = Err("late error"s);
    assert_eq(x && y, Err("late error"s));
  }

  {
    Result<u32, str> x = Err("early error"s);
    Result<str, str> y = Ok("foo"s);
    assert_eq(x && y, Err("early error"s));
  }
  {
    Result<u32, str> x = Err("not a 2"s);
    Result<str, str> y = Err("late error"s);
    assert_eq(x && y, Err("not a 2"s));
  }

  {
    Result<u32, str> x = Ok(2);
    Result<str, str> y = Ok("different result type"s);
    assert_eq(x && y, Ok("different result type"s));
  }
  std::cout << "operator&& test passed !\n";
}
{
  auto sq = [](u32 x) -> Result<u32, u32> { return Ok(x * x); };
  auto err = [](u32 x) -> Result<u32, u32> { return Err(x); };

  assert_eq(Ok(2u).and_then(sq).and_then(sq), Ok(16u));
  assert_eq(Ok(2u).and_then(sq).and_then(err), Err(4u));
  assert_eq(Ok(2u).and_then(err).and_then(sq), Err(2u));
  assert_eq(Err(3u).and_then(sq).and_then(sq), Err(3u));
  std::cout << "and_then test passed !\n";
}
{
  {
    Result<u32, str> x = Ok(2);
    Result<u32, str> y = Err("late error"s);
    assert_eq(x || y, Ok(2u));
  }
  {
    Result<u32, str> x = Err("early error"s);
    Result<u32, str> y = Ok(2);
    assert_eq(x || y, Ok(2u));
  }
  {
    Result<u32, str> x = Err("not a 2"s);
    Result<u32, str> y = Err("late error"s);
    assert_eq(x || y, Err("late error"s));
  }
  {
    Result<u32, str> x = Ok(2);
    Result<u32, str> y = Ok(100);
    assert_eq(x || y, Ok(2u));
  }
  std::cout << "operator|| test passed !\n";
}
{
  auto sq = [](u32 x) -> Result<u32, u32> { return Ok(x * x); };
  auto err = [](u32 x) -> Result<u32, u32> { return Err(x); };

  assert_eq(Ok(2).or_else(sq).or_else(sq), Ok(2));
  assert_eq(Ok(2).or_else(err).or_else(sq), Ok(2));
  assert_eq(Err(3).or_else(sq).or_else(err), Ok(9u));
  assert_eq(Err(3).or_else(err).or_else(err), Err(3u));
  std::cout << "or_else test passed !\n";
}
{
  Result<u32, u32> err = Err(2);
  Result<u32, u32> ok = Ok(2);

  assert_eq(ok.unwrap_or(1u), 2u);
  assert_eq(err.unwrap_or(1u), 1u);
  std::cout << "unwrap_or test passed !\n";
}
{
  auto count = [](str x) -> size_t { return x.size(); };

  assert_eq(Ok(2).unwrap_or_else(count), 2);
  assert_eq(Err("foo"s).unwrap_or_else(count), 3ull);
  std::cout << "unwrap_or_else test passed !\n";
}
{
  {
    Result<u32, str> x = Ok(2);
    assert_eq(x.unwrap(), 2u);
  }
  try {
    Result<u32, str> x = Err("emergency failure"s);
    x.unwrap(); // panics with `emergency failure`
  }
  catch (runtime_panic const &p)
  {
    using namespace boost::xpressive;
    sregex re =
        as_xpr(
            R"(runtime panicked at 'called `Result::unwrap() on an `Err` value: emergency failure', )") >>
        *_ >> as_xpr(":") >> +range('0', '9');
    smatch what;
    assert_true(regex_match(std::string{p.what()}, what, re));
  }
  std::cout << "unwrap test passed !\n";
}
{
  try {
    Result<u32, str> x = Ok(2);
    x.unwrap_err(); // panics with `2`
  }
  catch (runtime_panic const &p)
  {
    using namespace boost::xpressive;
    sregex re =
        as_xpr(
            R"(runtime panicked at 'called `Result::unwrap_err() on an `Ok` value: 2', )") >>
        *_ >> as_xpr(":") >> +range('0', '9');
    smatch what;
    assert_true(regex_match(std::string{p.what()}, what, re));
  }

  {
    Result<u32, str> x = Err("emergency failure"s);
    assert_eq(x.unwrap_err(), "emergency failure"s);
  }
  std::cout << "unwrap_err test passed !\n";
}
{
  auto good_year_from_input = "1909"s;
  auto bad_year_from_input = "190blarg"s;
  auto good_year = parse<int>(good_year_from_input).unwrap_or_default();
  auto bad_year = parse<int>(bad_year_from_input).unwrap_or_default();

  assert_eq(1909, good_year);
  assert_eq(0, bad_year);
  assert_eq(Ok(1).unwrap_or_default(), 1);
  std::cout << "unwrap_or_default test passed !\n";
}
{
  auto even = [](u32 u) -> Result<u32, str> {
    if (u % 2 == 0)
      return Ok(u);
    else
      return Err("odd"s);
  };
  auto func = [](auto u) -> Result<u32, str> { if(u%3==0) return Ok(1u); else return Err("error"s); };
  assert_eq(even(2).and_then(func), Err("error"s));
  assert_eq(even(2), Ok(2u));
  std::cout << "basic usage test passed !\n";
}
{
  auto res = Result<int,int>{Ok(2)};
  res = Result<int, int>{Err(2)};
  (void)Result<std::string, double>{in_place_ok, "hoge"};
  (void)Result<double, std::string>{in_place_err, "hoge"};
  (void)Result<std::vector<int>, double>{in_place_ok, {1, 2, 3, 4}};
  (void)Result<double, std::vector<double>>{in_place_err, {1., 2., 3., 4.}};

  res = Ok(1);
  res = Err(2);

  std::cout << "constructors and assignments test passed !\n";
}
{
  using namespace std::literals;
  std::stringstream ss;
  ss << Ok(1);
  assert_eq(ss.str(), "Ok(1)"s);
}
{
  using namespace std::literals;
  std::stringstream ss;
  ss << Err(1);
  assert_eq(ss.str(), "Err(1)"s);
}
{
  using namespace std::literals;
  std::stringstream ss;
  ss << Result<int, std::string>{Ok(1)};
  assert_eq(ss.str(), "Ok(1)"s);
}
{
  using namespace std::literals;
  std::stringstream ss;
  ss << Result<int, std::string>{Err("hoge"s)};
  assert_eq(ss.str(), "Err(hoge)"s);
}
{
  using namespace std::literals;
  std::stringstream ss;
  ss << Ok(std::vector<std::string>{"foo"s, "bar"s});
  assert_eq(ss.str(), "Ok([foo,bar])"s);
}
{
  using namespace std::literals;
  std::stringstream ss;
  ss << Err(std::vector<std::string>{"foo"s, "bar"s});
  assert_eq(ss.str(), "Err([foo,bar])"s);
}
{
  using namespace std::literals;
  std::stringstream ss;
  ss << Err("foo"s);
  assert_eq(ss.str(), "Err(foo)"s);
}
{
  using namespace std::literals;
  auto res = Result<int, std::vector<int>>{in_place_err, {1,2,3}};
  assert_eq((boost::format("%1%") % res).str(), "Err([1,2,3])"s);
  res = Ok(1);
  assert_eq((boost::format("%1%") % res).str(), "Ok(1)"s);
}

std::cout << "\nall green !" << std::endl;

}
