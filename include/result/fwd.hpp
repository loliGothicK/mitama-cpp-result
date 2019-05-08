#ifndef MITAMA_RESULT_FWD
#define MITAMA_RESULT_FWD
#include <variant>
namespace mitama {
template <class = std::monostate, // Ok type
          class = std::monostate, // Err type
          class /* for detection idiom */ = decltype(nullptr)>
class Result;

template <class T = std::monostate>
class Ok;

template <class = std::monostate>
class Err;
}
#endif