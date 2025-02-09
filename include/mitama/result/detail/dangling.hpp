#pragma once

#include <functional>
#include <utility>

namespace mitama
{

template <class T>
class dangling
{
  T value_;

public:
  template <class U>
  constexpr explicit dangling(U&& u) : value_(std::forward<U>(u))
  {
  }

  constexpr decltype(auto) transmute() const
  {
    return value_;
  }
};

template <class T>
class dangling<std::reference_wrapper<T>>
{
  std::reference_wrapper<T> ref_;

public:
  constexpr explicit dangling(std::reference_wrapper<T> ref) : ref_(ref) {}

  constexpr T& transmute() const&
  {
    return ref_.get();
  }
};

template <class T>
using dangle_ref = dangling<std::reference_wrapper<std::remove_reference_t<T>>>;
} // namespace mitama
