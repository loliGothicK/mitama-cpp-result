#ifndef MITAMA_MITAMAGIC_IS_PUBLIC_DERIVED_HPP
#define MITAMA_MITAMAGIC_IS_PUBLIC_DERIVED_HPP
#include <type_traits>

namespace mitama::mitamagic {

template <class, class, class = void>
struct is_interface_of: std::false_type {};

template <class Interface, class Derived>
struct is_interface_of<Interface, Derived, std::void_t<decltype(std::declval<Interface*>() = std::declval<Derived*>())>>: std::false_type {};

template <class Interface, class Derived>
inline constexpr bool is_interface_of_v = is_interface_of<Interface, Derived>::value;

}

#endif
