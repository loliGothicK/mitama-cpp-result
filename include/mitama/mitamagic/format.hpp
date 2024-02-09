#ifndef MITAMA_CPP_RESULT_FORMAT_HPP
#define MITAMA_CPP_RESULT_FORMAT_HPP

#ifdef __cpp_lib_format
#  include <format>
namespace fmt = std;
#elif __has_include(<fmt/format.h>)
#  include <fmt/format.h>
#else
#  error "mitama-cpp-result requires <fmt/format.h> or C++20 <format> header"
#endif

#endif // MITAMA_CPP_RESULT_FORMAT_HPP
