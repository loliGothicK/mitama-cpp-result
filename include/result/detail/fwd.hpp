#ifndef MITAMA_RESULT_FWD
#define MITAMA_RESULT_FWD
#include <variant>
namespace mitama {

template <class = std::monostate,   // success type
          class = std::monostate,   // failure type
          class = decltype(nullptr) // for detection idiom
>
class basic_result;

template <class T = std::monostate>
class success;

template <class = std::monostate>
class failure;
}
#endif
