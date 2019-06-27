#include <mitama/match/match.hpp>
#include <boost/lambda/lambda.hpp>
#include <iostream>

int main() {
    namespace match = mitama::match;

    auto match_ = match::match(
        match::Guard(match::range(1, 10)) <<= 1,
        match::Guard(match::range(10, 20)) <<= 2,
        match::Guard(match::range(30, 40)) <<= 3,
        match::Default <<= 0
    );

    std::cout << match_(2) << std::endl;
    std::cout << match_(12) << std::endl;
    std::cout << match_(22) << std::endl;

    using mitama::match::_;
    using boost::lambda::_1;
    using boost::lambda::_2;

    auto res = match::match[ std::tuple(1, 3) ](
        match::Case(_, 1) & match::Guard(_1 % 2 == 1)
            >>= _1 * 2,
        match::Case(1, _) & match::Guard(_2 % 2 == 1)
            >>= _2 * 2,
        match::Case(1, 3)
            >>= []{ return 3; },
        match::Default
            >>= []{ return 0; }
    );

    std::cout << res << std::endl;
}