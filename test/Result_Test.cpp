#include <Result.hpp>
#include "gtest/gtest.h"
#include <boost/hana/assert.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <cstdint>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>

using namespace boost::xpressive;
using namespace rust_std;
using namespace std::string_literals;
using namespace std::string_view_literals;
namespace hana = boost::hana;
using boost::lambda::_1;
using boost::lambda::_2;

#define assert_eq ASSERT_EQ
#define assert_true ASSERT_TRUE
#define assert_false ASSERT_FALSE

struct is_complete_type_impl
{
  template <class T>
  static auto check(T *) -> decltype(
      sizeof(T),
      std::true_type());

  template <class T>
  static auto check(...) -> std::false_type;
};

template <class T>
struct is_complete_type
    : decltype(is_complete_type_impl::check<T>(nullptr))
{
};

template < class T >
inline constexpr bool is_complete_type_v = is_complete_type<T>::value;

/*
 * compile time type traits checking
 */

#include "special_member_tests.hpp"

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

static_assert(MITAMA_HAS_FUNC(unwrap_or_default, Result<DefaultConstructibleType, std::string>));
static_assert(!MITAMA_HAS_FUNC(unwrap_or_default, Result<NonDefaultConstructibleType, std::string>));
static_assert(MITAMA_HAS_FUNC(unwrap_or_default, Ok<DefaultConstructibleType>));
static_assert(!MITAMA_HAS_FUNC(unwrap_or_default, Ok<NonDefaultConstructibleType>));

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

class ResultTest : public ::testing::Test
{
protected:
  virtual void SetUp() {}
};

TEST_F(ResultTest, IsOkTest)
{
  Result<u32, str> x = Ok(-3);
  assert_eq(x.is_ok(), true);

  Result<u32, str> y = Err("Some error message");
  assert_eq(y.is_ok(), false);
}
TEST_F(ResultTest, IsErrTest)
{
  Result<u32, str> x = Ok(-3);
  assert_eq(x.is_err(), false);

  Result<u32, str> y = Err("Some error message");
  assert_eq(y.is_err(), true);
}
TEST_F(ResultTest, OkTest)
{
  Result<u32, str> x = Ok(2);
  assert_eq(x.err(), None);

  Result<int, str> y = Err("Nothing here");
  assert_eq(y.err(), Some("Nothing here"));
}
TEST_F(ResultTest, ErrTest)
{
  Result<u32, str> x = Ok(2);
  assert_eq(x.err(), None);

  Result<u32, str> y = Err("Nothing here");
  assert_eq(y.err(), Some("Nothing here"));
}
TEST_F(ResultTest, MapTest)
{
  std::string line = "1,3,5,7";

  for (auto num : split(line, ","))
  {
    if (auto res = parse<i32>(num).map(_1 * 2); res.is_ok())
    {
      assert_true(res.ok().value() % 2 == 0);
    }
  }
}
TEST_F(ResultTest, MapErrTest)
{
  auto stringify = [](u32 x) -> str {
    return "error code: "s + std::to_string(x);
  };

  Result<u32, u32> x = Ok(2);
  assert_eq(x.map_err(stringify), Ok(2u));

  Result<u32, u32> y = Err(13);
  assert_eq(y.map_err(stringify), Err("error code: 13"s));
}
TEST_F(ResultTest, LogicalAndTest)
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
}

TEST_F(ResultTest, AndThenTest)
{
  auto sq = [](u32 x) -> Result<u32, u32> { return Ok(x * x); };
  auto err = [](u32 x) -> Result<u32, u32> { return Err(x); };

  assert_eq(Ok(2u).and_then(sq).and_then(sq), Ok(16u));
  assert_eq(Ok(2u).and_then(sq).and_then(err), Err(4u));
  assert_eq(Ok(2u).and_then(err).and_then(sq), Err(2u));
  assert_eq(Err(3u).and_then(sq).and_then(sq), Err(3u));
}

TEST_F(ResultTest, LogicalOrTest)
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
}

TEST_F(ResultTest, OrElseTest)
{
  auto sq = [](u32 x) -> Result<u32, u32> { return Ok(x * x); };
  auto err = [](u32 x) -> Result<u32, u32> { return Err(x); };

  assert_eq(Ok(2).or_else(sq).or_else(sq), Ok(2u));
  assert_eq(Ok(2).or_else(err).or_else(sq), Ok(2u));
  assert_eq(Err(3).or_else(sq).or_else(err), Ok(9u));
  assert_eq(Err(3).or_else(err).or_else(err), Err(3u));
}
TEST_F(ResultTest, UnwrapOrTest)
{
  Result<u32, u32> err = Err(2);
  Result<u32, u32> ok = Ok(2);

  assert_eq(ok.unwrap_or(1u), 2u);
  assert_eq(err.unwrap_or(1u), 1u);
}

TEST_F(ResultTest, UnwrapOrElseTest)
{
  auto count = [](str x) -> size_t { return x.size(); };

  assert_eq(Ok(2).unwrap_or_else(count), 2);
  assert_eq(Err("foo"s).unwrap_or_else(count), 3);
}

TEST_F(ResultTest, UnwrapTest)
{
  {
    Result<u32, str> x = Ok(2);
    assert_eq(x.unwrap(), 2);
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
    std::cout << p.what() << std::endl;
    EXPECT_TRUE(regex_match(std::string{p.what()}, what, re));
  }
}

TEST_F(ResultTest, UnwrapErrTest)
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
    std::cout << p.what() << std::endl;
    EXPECT_TRUE(regex_match(std::string{p.what()}, what, re));
  }

  {
    Result<u32, str> x = Err("emergency failure"s);
    assert_eq(x.unwrap_err(), "emergency failure"s);
  }
}

TEST_F(ResultTest, UnwrapOrDefaultTest)
{
  auto good_year_from_input = "1909"s;
  auto bad_year_from_input = "190blarg"s;
  auto good_year = parse<int>(good_year_from_input).unwrap_or_default();
  auto bad_year = parse<int>(bad_year_from_input).unwrap_or_default();

  assert_eq(1909, good_year);
  assert_eq(0, bad_year);
}

TEST_F(ResultTest, BasicUsageTest)
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
}