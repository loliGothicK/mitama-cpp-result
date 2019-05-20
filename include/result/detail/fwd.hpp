#ifndef MITAMA_RESULT_FWD
#define MITAMA_RESULT_FWD
#include <variant>
namespace mitama {
inline namespace result {
template <class = std::monostate,   // Ok type
          class = std::monostate,   // Err type
          class = decltype(nullptr) // for detection idiom
>
class Result;

template <class T = std::monostate>
class Ok;

template <class = std::monostate>
class Err;
}}
#endif