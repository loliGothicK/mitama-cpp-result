#ifndef MITAMA_RESULT_FWD
#define MITAMA_RESULT_FWD
#include <variant>
namespace mitama {

enum class mutability: bool {
    mut,
    constant
};

template <mutability,
          class = std::monostate,   // Ok type
          class = std::monostate,   // Err type
          class = decltype(nullptr) // for detection idiom
>
class basic_result;

template <class T = std::monostate>
class Ok;

template <class = std::monostate>
class Err;
}
#endif
