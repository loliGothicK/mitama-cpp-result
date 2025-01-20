#pragma once

#include <mitama/maybe/maybe.hpp>
#include <mitama/result/result.hpp>

#include <type_traits>
#include <utility>

namespace mitama
{
inline constexpr maybe<std::monostate>
as_maybe(bool b)
{
  return b ? maybe<std::monostate>{ std::in_place } : nothing;
}

template <class T>
inline constexpr maybe<T>
as_just(bool b, T&& some)
{
  return b ? maybe<T>{ std::forward<T>(some) } : nothing;
}

template <class F>
inline constexpr maybe<std::invoke_result_t<F&&>>
as_just_from(bool b, F&& some)
{
  return b ? maybe{ std::invoke(std::forward<F>(some)) } : nothing;
}

template <class T>
inline constexpr maybe<T>
and_maybe(bool b, const maybe<T>& may)
{
  return b ? may : nothing;
}

template <class F>
  requires std::is_invocable_v<F&&>
           && is_maybe<std::invoke_result_t<F&&>>::value
inline constexpr auto
and_maybe_from(bool b, F&& may) -> std::invoke_result_t<F&&>
{
  return b ? std::invoke(std::forward<F>(may)) : nothing;
}

template <class T>
inline constexpr result<T, void>
as_ok(bool b, T&& o)
{
  return b ? result<T, void>(ok(std::forward<T>(o))) : err();
}

template <class T, class E>
inline constexpr result<T, E>
as_result(bool b, T&& o, E&& e)
{
  return b ? result<T, E>{ ok(std::forward<T>(o)) }
           : result<T, E>{ err(std::forward<E>(e)) };
}

template <class F, class G>
inline constexpr result<std::invoke_result_t<F>, std::invoke_result_t<G&&>>
as_result_from(bool b, F&& o, G&& e)
{
  using result_type =
      result<std::invoke_result_t<F>, std::invoke_result_t<G&&>>;
  return b ? result_type{ ok(std::invoke(std::forward<F>(o))) }
           : result_type{ err(std::invoke(std::forward<G>(e))) };
}

template <class E>
inline constexpr result<void, E>
ok_or(bool b, E&& e)
{
  return b ? result<void, E>{ ok() }
           : result<void, E>{ err(std::forward<E>(e)) };
}

template <class G>
inline constexpr result<void, std::invoke_result_t<G&&>>
ok_or_else(bool b, G&& e)
{
  using result_type = result<void, std::invoke_result_t<G>>;
  return b ? result_type{ ok() }
           : result_type{ err(std::invoke(std::forward<G>(e))) };
}
} // namespace mitama
