#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <mitama/result/result.hpp>
#include <mitama/match/match.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/type_index.hpp>
#include <boost/format.hpp>

#include <string>

TEST_CASE("match basic", "[match]") {
    namespace match = mitama::match;

    auto match_ = match::match(
        match::Guard(match::range(1, 10)) <<= 1,
        match::Guard(match::range(10, 20)) <<= 2,
        match::Guard(match::range(30, 40)) <<= 3,
        match::Default <<= 0
    );

    REQUIRE(match_(2) == 1);
    REQUIRE(match_(12) == 2);
    REQUIRE(match_(32) == 3);
    REQUIRE(match_(22) == 0);
}

TEST_CASE("match result", "[match],[result]") {
    namespace match = mitama::match;
    using mitama::result, mitama::success, mitama::failure;
    using mitama::match::_;
    using namespace std::literals::string_literals;

    auto match_ = match::match(
        match::Case(success(_)) <<= 1,
        match::Case(failure(_)) <<= 2
    );
    auto even = [](int u) -> result<int, std::string> {
        if (u % 2 == 0)
            return success(u);
        else
            return failure("odd"s);
    };
    REQUIRE(match_(even(2)) == 1);
    REQUIRE(match_(even(3)) == 2);
}

TEST_CASE("match result action", "[match],[result],[action]") {
    namespace match = mitama::match;
    using mitama::result, mitama::success, mitama::failure;
    using mitama::match::_;
    using namespace std::literals::string_literals;

    int calc;
    auto match_ = match::match(
        match::Case(success(_)) >>= [&]{ calc = 1; },
        match::Case(failure(_)) >>= [&]{ calc = 2; }
    );
    auto even = [](int u) -> result<int, std::string> {
        if (u % 2 == 0)
            return success(u);
        else
            return failure("odd"s);
    };
    REQUIRE((match_(even(2)), calc) == 1);
    REQUIRE((match_(even(3)), calc) == 2);
}

TEST_CASE("match result sequence A", "[match],[result]") {
    namespace match = mitama::match;
    using mitama::result, mitama::success, mitama::failure;
    using mitama::match::_;
    using namespace std::literals::string_literals;

    auto match_ = match::match(
        match::Case(success(2))  >>= []{ return "2"s; },
        match::Case(success(4))  >>= []{ return "4"s; },
        match::Case(success(6))  >>= []{ return "6"s; },
        match::Case(success(_))  >>= [](int a){ return std::to_string(a); },
        match::Case(failure(_)) >>= [](std::string s){ return s; }
    );

    auto even = [](int u) -> result<int, std::string> {
        if (u % 2 == 0)
            return success(u);
        else
            return failure("odds"s);
    };

    {
        auto s = match_(even(2));
        REQUIRE(s == "2"s);
    }
    {
        auto s = match_(even(4));
        REQUIRE(s == "4"s);
    }
    {
        auto s = match_(even(6));
        REQUIRE(s == "6"s);
    }
    {
        auto s = match_(even(8));
        REQUIRE(s == "8"s);
    }
    {
        auto res = even(5);
        auto s = match_(res);
        REQUIRE(s == "odds"s);
    }
}

TEST_CASE("match result sequence B", "[match],[result]") {
    namespace match = mitama::match;
    using mitama::result, mitama::success, mitama::failure;
    using mitama::match::_;
    using boost::lambda::_1;

    auto match_ = match::match<double>(
        match::Case(success(2)) <<= 1,
        match::Case(success(4)) <<= 1,
        match::Case(success(6)) <<= 1,
        match::Case(success(_)) >>= _1,
        match::Case(failure(_)) >>= [](auto v) { return v; }
    );
    auto even = [](int u) -> result<int, int> {
        if (u % 2 == 0)
            return success(u);
        else
            return failure(u+1);
    };
    REQUIRE(match_(even(2)) == 1);
    REQUIRE(match_(even(4)) == 1);
    REQUIRE(match_(even(6)) == 1);
    REQUIRE(match_(even(8)) == 8);
    REQUIRE(match_(even(3)) == 4);
    REQUIRE(match_(even(5)) == 6);
    REQUIRE(match_(even(7)) == 8);
}
