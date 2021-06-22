#ifndef MITAMA_CPP_RESULT_FORMAT_HPP
#define MITAMA_CPP_RESULT_FORMAT_HPP
#if defined(__cpp_lib_format) and __has_include(<format>) and __cpp_lib_format >= 201907L
#include <format>
namespace mitama::fmt { using ::std::format, ::std::formatter, ::std::format_context; }
#else
#include <fmt/format.h>
namespace mitama::fmt { using ::fmt::format, ::fmt::formatter, ::fmt::format_context; }
#endif
#include <string>
#include <iostream>

namespace mitama {
#if defined(__cpp_deduction_guides) and __cpp_deduction_guides >= 201907L && \
		defined(__cpp_concepts)         and __cpp_concepts         >= 201907L
	template <class T>
	requires requires (T const& t) { std::cout << t; }
	struct display {
		T const& value_;
	};
#else
	template<class, class = void>
	struct display;
	template <class T>
	struct display<T, std::void_t<decltype(std::cout << std::declval<T const&>())>> {
		T const& value_;
	};
	template<class T> display(T)->display<T, void>;
#endif

	template<class... Args>
	std::string format(std::string_view fmt, const Args&... args) {
		return mitama::fmt::format(fmt, args...);
	}

	template<class... Args>
	std::wstring format(std::wstring_view fmt, const Args&... args) {
		return mitama::fmt::format(fmt, args...);
	}
}

template <class T>
struct mitama::fmt::formatter<::mitama::display<T>> : mitama::fmt::formatter<std::string> {
	auto format(::mitama::display<T> const& d, mitama::fmt::format_context& ctx) {
		std::stringstream ss; ss << d.value_;
		return formatter<std::string>::format(ss.str(), ctx);
	}
};

#endif //MITAMA_CPP_RESULT_FORMAT_HPP
