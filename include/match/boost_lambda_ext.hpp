#ifndef MITAMA_MATCH_BOOST_LAMBDA_EXT_HPP
#define MITAMA_MATCH_BOOST_LAMBDA_EXT_HPP
#include <boost/lambda/lambda.hpp>

namespace mitama::match::_detail {
    template < std::size_t B, std::size_t E >
	struct Exponent_ : std::integral_constant<std::size_t, B*Exponent_<B,E-1>::value> {};
	
	template < std::size_t B >
	struct Exponent_<B, 1> : std::integral_constant<std::size_t, B> {};
	
	
	template < std::size_t Head, std::size_t... Digits >
	struct to_decimal
		: std::integral_constant<std::size_t, (Head-'0')*Exponent_<10, sizeof...(Digits)>::value + to_decimal<Digits...>::value > {};
	
	template < std::size_t Head >
	struct to_decimal<Head>
		: std::integral_constant<std::size_t, Head-'0'> {};

    template < std::size_t... Digits >
    inline constexpr std::size_t to_decimal_v = to_decimal<Digits...>::value;
}

namespace mitama::literals {
inline namespace placeholder_literals {
    template<char... Chars>
    inline constexpr
    boost::lambda::lambda_functor<boost::lambda::placeholder<static_cast<int>(match::_detail::to_decimal_v<Chars...>)>> 
    operator"" _() { return {}; }
}
}

#endif