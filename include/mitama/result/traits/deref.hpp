#pragma once

#include <mitama/result/detail/dangling.hpp>

#include <type_traits>

namespace mitama::traits
{

template <class T>
concept dereferenceable = requires { *std::declval<T>(); };

template <dereferenceable For>
using deref = decltype(*std::declval<For>());

} // namespace mitama::traits
