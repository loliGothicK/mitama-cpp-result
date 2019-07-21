#ifndef MITAMA_RESULT_FWD
#define MITAMA_RESULT_FWD
#include <variant>

namespace mitama {

/// for mutability control
enum class mutability: bool {
    mut = false,
    immut = true,
};

constexpr mutability operator&&(mutability _1, mutability _2) {
    return mutability{ !(!static_cast<bool>(_1) && !static_cast<bool>(_2)) };
}

template < mutability Mut >
inline constexpr bool is_mut_v = !static_cast<bool>(Mut);

template <mutability,
          class = std::monostate,   // success type
          class = std::monostate,   // failure type
          class = decltype(nullptr) // for detection idiom
>
class basic_result;

/// alias template for immutable result
template <class T = std::monostate, class E = std::monostate>
using result = basic_result<mutability::immut, T, E>;

/// alias template for mutable result
template <class T = std::monostate, class E = std::monostate>
using mut_result = basic_result<mutability::mut, T, E>;

template <class T = std::monostate>
class success;

/// Deduction guide for `success`
template <class T>
success(T&&)->success<T>;

template <class = std::monostate>
class failure;

/// Deduction guide for `failure`
template <class E>
failure(E&&)->failure<E>;

class in_place_ok_t {};
inline constexpr in_place_ok_t in_place_ok = {};

class in_place_err_t {};
inline constexpr in_place_err_t in_place_err = {};

}
#endif
