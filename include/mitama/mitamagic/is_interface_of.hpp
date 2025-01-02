#pragma once

namespace mitama::mitamagic
{

template <typename Interface, typename Derived>
inline constexpr bool is_interface_of_v =
    requires(Interface* i, Derived* d) { i = d; };

} // namespace mitama::mitamagic
