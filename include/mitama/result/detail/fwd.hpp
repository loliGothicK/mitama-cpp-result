#ifndef MITAMA_RESULT_FWD
#define MITAMA_RESULT_FWD
#include <variant>
#include <tuple>
namespace mitama {

namespace _result_detail {
  template <class = void, class...>
  struct forward_mode {};
}

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

template <class T>
struct void_to_monostate: std::conditional<std::is_void_v<T>, std::monostate, T> {};

template <class T>
using void_to_monostate_t = typename void_to_monostate<T>::type;

template <mutability,
          class = std::monostate,   // success type
          class = std::monostate,   // failure type
          class = std::nullptr_t    // for detection idiom
>
class basic_result;

/// alias template for immutable result
template <class T = std::monostate, class E = std::monostate>
using result = basic_result<mutability::immut, void_to_monostate_t<T>, void_to_monostate_t<E>>;

/// alias template for mutable result
template <class T = std::monostate, class E = std::monostate>
using mut_result = basic_result<mutability::mut, void_to_monostate_t<T>, void_to_monostate_t<E>>;

template <class = std::monostate, class...> class success_t;
template <class T> success_t(T&&) -> success_t<T>;

template <class = std::monostate, class...> class failure_t;
template <class E> failure_t(E&&) -> failure_t<E>;

class in_place_ok_t {};
inline constexpr in_place_ok_t in_place_ok = {};

class in_place_err_t {};
inline constexpr in_place_err_t in_place_err = {};

}
#endif
