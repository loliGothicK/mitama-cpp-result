#pragma once

#include <variant>

namespace mitama
{

template <class...>
class just_t;
template <class T>
just_t(T&&) -> just_t<T>;

template <class>
class maybe;

template <class T>
maybe(just_t<T>&) -> maybe<T>;
template <class T>
maybe(const just_t<T>&) -> maybe<const T>;
template <class T>
maybe(just_t<T>&&) -> maybe<std::remove_reference_t<T>>;

} // namespace mitama
