#pragma once

#include <mitama/anyhow/error.hpp>
#include <mitama/mitamagic/format.hpp>
#include <mitama/panic.hpp>
#include <mitama/result/detail/fwd.hpp>
#include <mitama/result/detail/meta.hpp>
#include <mitama/result/factory/failure.hpp>
#include <mitama/result/factory/success.hpp>

#include <cstddef>
#include <fmt/core.h>
#include <fmt/std.h>
#include <functional>
#include <memory>
#include <source_location>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <version>

#ifndef MITAMA_VARIANT_CONSTEXPR
#  if __cpp_lib_variant >= 202106L
#    define MITAMA_VARIANT_CONSTEXPR constexpr
#  else
#    define MITAMA_VARIANT_CONSTEXPR
#  endif
#endif

namespace mitama::_result_detail
{

template <class... Fs>
struct overload : Fs...
{
  using Fs::operator()...;
};

template <class... Fs>
overload(Fs...) -> overload<Fs...>;

template <class... Fs>
struct overload_linearly
{
  std::tuple<Fs...> funcs;

  constexpr explicit overload_linearly(Fs... fs) : funcs(std::move(fs)...) {}

  template <class... Args>
  constexpr decltype(auto) operator()(Args&&... args) const
  {
    return invoke_impl<0>(std::forward<Args>(args)...);
  }

private:
  template <std::size_t Index, class... Args>
  constexpr decltype(auto) invoke_impl(Args&&... args) const
  {
    if constexpr (Index < sizeof...(Fs))
    {
      using Func = std::tuple_element_t<Index, std::tuple<Fs...>>;
      if constexpr (std::is_invocable_v<Func, Args...>)
      {
        return std::get<Index>(funcs)(std::forward<Args>(args)...);
      }
      else
      {
        return invoke_impl<Index + 1>(std::forward<Args>(args)...);
      }
    }
    else
    {
      static_assert(Index < sizeof...(Fs), "No matching callable found.");
    }
  }
};

template <typename T>
constexpr T
id(T&& t)
{
  return static_cast<T&&>(t);
}

} // namespace mitama::_result_detail

namespace mitama
{

template <class>
struct is_result : std::false_type
{
};
template <mutability _mut, class T, class E>
struct is_result<basic_result<_mut, T, E>> : std::true_type
{
};
template <class T>
inline constexpr bool is_result_v = is_result<T>::value;

template <class, class...>
struct is_convertible_result_with : std::false_type
{
};
template <mutability _mut, class T, class E, class U>
struct is_convertible_result_with<basic_result<_mut, T, E>, success_t<U>>
    : std::is_constructible<U, T>
{
};
template <mutability _mut, class T, class E, class F>
struct is_convertible_result_with<basic_result<_mut, T, E>, failure_t<F>>
    : std::is_constructible<F, E>
{
};
template <mutability _mut, class T, class E, class U, class F>
struct is_convertible_result_with<
    basic_result<_mut, T, E>, success_t<U>, failure_t<F>>
    : std::conjunction<std::is_constructible<U, T>, std::is_constructible<F, E>>
{
};

template <class T, class... Requires>
inline constexpr bool is_convertible_result_with_v =
    is_convertible_result_with<std::remove_cvref_t<T>, Requires...>::value;

template <class>
struct is_err_type : std::false_type
{
};
template <class T>
struct is_err_type<failure_t<T>> : std::true_type
{
};
template <class>
struct is_ok_type : std::false_type
{
};
template <class T>
struct is_ok_type<success_t<T>> : std::true_type
{
};

} // namespace mitama

#include <mitama/result/detail/result_impl.hpp>

namespace mitama
{

/// @brief class basic_result
/// @param _mutability: enum class value for mutability control
/// @param T: Type of successful value
/// @param E: Type of unsuccessful value
template <mutability _mutability, class T, class E>
  requires std::is_object_v<std::remove_cvref_t<T>>
               && std::is_object_v<std::remove_cvref_t<E>>
               && (!std::is_array_v<std::remove_cvref_t<T>>)
               && (!std::is_array_v<std::remove_cvref_t<E>>)
class [[nodiscard]] basic_result<_mutability, T, E>
    : /* method injection selectors */
      public unwrap_or_default_friend_injector<basic_result<_mutability, T, E>>,
      public transpose_friend_injector<basic_result<_mutability, T, E>>,
      public indirect_friend_injector<basic_result<_mutability, T, E>>,
      public map_apply_friend_injector<basic_result<_mutability, T, E>>,
      public map_err_apply_friend_injector<basic_result<_mutability, T, E>>,
      public and_then_apply_friend_injector<basic_result<_mutability, T, E>>,
      public or_else_apply_friend_injector<basic_result<_mutability, T, E>>
{
  /// result storage
  std::variant<std::monostate, success_t<T>, failure_t<E>> storage_;
  /// friend accessors
  template <mutability, class, class>
  friend class basic_result;
  /// private aliases
  template <mutability _mut, class T_, class E_>
  using not_self =
      std::negation<std::is_same<basic_result, basic_result<_mut, T_, E_>>>;

public:
  /// associated types
  using ok_type = T;
  using err_type = E;
  using ok_reference_type = std::remove_reference_t<T>&;
  using err_reference_type = std::remove_reference_t<E>&;
  using ok_const_reference_type = const std::remove_cvref_t<T>&;
  using err_const_reference_type = const std::remove_cvref_t<E>&;
  /// mutability
  static constexpr bool is_mut = !static_cast<bool>(_mutability);

  /* Constructors */

  /// default constructor is not permitted
  constexpr basic_result() noexcept = delete;

  /// @brief
  ///   explicit copy construcor for convertible basic_result
  template <mutability _mu, class U, class F>
    requires std::is_constructible_v<T, U> && std::is_constructible_v<E, F>
             && (!std::is_convertible_v<F, E> || !std::is_convertible_v<U, T>)
  explicit constexpr basic_result(const basic_result<_mu, U, F>& res)
      : storage_(res.storage_)
  {
  }

  /// @brief
  ///   non-explicit copy construcor for convertible basic_result
  template <mutability _mu, class U, class F>
    requires std::is_constructible_v<T, U> && std::is_constructible_v<E, F>
             && std::is_convertible_v<U, T> && std::is_convertible_v<F, E>
  constexpr basic_result(const basic_result<_mu, U, F>& res)
      : storage_(res.storage_)
  {
  }

  /// @brief
  ///   explicit move construcor for convertible basic_result
  template <mutability _mu, class U, class F>
    requires std::is_constructible_v<T, U> && std::is_constructible_v<E, F>
             && (!std::is_convertible_v<F, E> || !std::is_convertible_v<U, T>)
  explicit constexpr basic_result(basic_result<_mu, U, F>&& res)
      : storage_(std::move(res.storage_))
  {
  }

  /// @brief
  ///   non-explicit copy construcor for convertible basic_result
  template <mutability _mu, class U, class F>
    requires std::is_constructible_v<T, U> && std::is_constructible_v<E, F>
             && std::is_convertible_v<U, T> && std::is_convertible_v<F, E>
  constexpr basic_result(basic_result<_mu, U, F>&& res)
      : storage_(std::move(res.storage_))
  {
  }

  /// @brief
  ///   copy assignment operator for convertible basic_result
  template <mutability _mu, class U, class F>
    requires std::is_constructible_v<T, U> && std::is_constructible_v<E, F>
  MITAMA_VARIANT_CONSTEXPR basic_result&
  operator=(const basic_result<_mu, U, F>& res)
  {
    static_assert(
        is_mut_v<_mutability>, "Error: assignment to immutable result"
    );
    if (res.is_ok())
    {
      this->storage_ = success_t<T>(res.unwrap());
    }
    else
    {
      this->storage_ = failure_t<E>(res.unwrap_err());
    }
    return *this;
  }

  /// @brief
  ///   move assignment operator for convertible basic_result
  template <mutability _mu, class U, class F>
    requires std::is_constructible_v<T, U> && std::is_constructible_v<E, F>
  MITAMA_VARIANT_CONSTEXPR basic_result& operator=(basic_result<_mu, U, F>&& res
  )
  {
    static_assert(
        is_mut_v<_mutability>, "Error: assignment to immutable result"
    );
    if (res.is_ok())
    {
      this->storage_ = success_t<T>(res.unwrap());
    }
    else
    {
      this->storage_ = failure_t<E>(res.unwrap_err());
    }
    return *this;
  }

  /// @brief
  ///   copy assignment operator for convertible success_t
  template <class U>
    requires std::is_constructible_v<T, U>
  MITAMA_VARIANT_CONSTEXPR basic_result& operator=(const success_t<U>& _ok)
  {
    static_assert(
        is_mut_v<_mutability>, "Error: assignment to immutable result"
    );
    this->storage_ = success_t<T>(_ok.get());
    return *this;
  }

  /// @brief
  ///   copy assignment operator for convertible failure_t
  template <class F>
    requires std::is_constructible_v<E, F>
  MITAMA_VARIANT_CONSTEXPR basic_result& operator=(const failure_t<F>& _err)
  {
    static_assert(
        is_mut_v<_mutability>, "Error: assignment to immutable result"
    );
    this->storage_ = failure_t<E>(_err.get());
    return *this;
  }

  /// @brief
  ///   move assignment operator for convertible success_t
  template <class U>
    requires std::is_constructible_v<T, U>
  MITAMA_VARIANT_CONSTEXPR basic_result& operator=(success_t<U>&& _ok)
  {
    static_assert(
        is_mut_v<_mutability>, "Error: assignment to immutable result"
    );
    this->storage_ = success_t<T>(std::move(_ok.get()));
    return *this;
  }

  /// @brief
  ///   move assignment operator for convertible failure_t
  template <class F>
    requires std::is_constructible_v<E, F>
  MITAMA_VARIANT_CONSTEXPR basic_result& operator=(failure_t<F>&& _err)
  {
    static_assert(
        is_mut_v<_mutability>, "Error: assignment to immutable result"
    );
    this->storage_ = failure_t<E>(std::move(_err.get()));
    return *this;
  }

  /// @brief
  ///   non-explicit constructor for successful lvalue
  template <class U>
    requires std::is_constructible_v<T, U> && std::is_convertible_v<U, T>
  constexpr basic_result(const success_t<U>& ok)
      : storage_{ std::in_place_type<success_t<T>>, std::in_place, ok.get() }
  {
  }

  /// @brief
  ///   explicit constructor for successful lvalue
  template <class U>
    requires std::is_constructible_v<T, U> && (!std::is_convertible_v<U, T>)
  constexpr explicit basic_result(const success_t<U>& ok)
      : storage_{ std::in_place_type<success_t<T>>, std::in_place, ok.get() }
  {
  }

  /// @brief
  ///   non-explicit constructor for successful rvalue
  template <class U>
    requires std::is_constructible_v<T, U> && std::is_convertible_v<U, T>
  constexpr basic_result(success_t<U>&& ok)
      : storage_{ std::in_place_type<success_t<T>>, std::move(ok) }
  {
  }

  /// @brief
  ///   explicit constructor for successful rvalue
  template <class U>
    requires std::is_constructible_v<T, U> && (!std::is_convertible_v<U, T>)
  constexpr explicit basic_result(success_t<U>&& ok)
      : storage_{ std::in_place_type<success_t<T>>, std::move(ok) }
  {
  }

  /// @brief
  ///   non-explicit constructor for unsuccessful lvalue
  template <class U>
    requires std::is_constructible_v<E, U> && std::is_convertible_v<U, E>
  constexpr basic_result(const failure_t<U>& err)
      : storage_{ std::in_place_type<failure_t<E>>, std::in_place, err.get() }
  {
  }

  /// @brief
  ///   explicit constructor for unsuccessful lvalue
  template <class U>
    requires std::is_constructible_v<E, U> && (!std::is_convertible_v<U, E>)
  constexpr explicit basic_result(const failure_t<U>& err)
      : storage_{ std::in_place_type<failure_t<E>>, std::in_place, err.get() }
  {
  }

  /// @brief
  ///   non-explicit constructor for unsuccessful rvalue
  template <class U>
    requires std::is_constructible_v<E, U> && std::is_convertible_v<U, E>
  constexpr basic_result(failure_t<U>&& err)
      : storage_{ std::in_place_type<failure_t<E>>, std::move(err) }
  {
  }

  /// @brief
  ///   explicit constructor for unsuccessful lvalue
  template <class U>
    requires std::is_constructible_v<E, U> && (!std::is_convertible_v<U, E>)
  constexpr explicit basic_result(failure_t<U>&& err)
      : storage_{ std::in_place_type<failure_t<E>>, std::move(err) }
  {
  }

  constexpr basic_result(success_t<>)
      : storage_{ std::in_place_type<success_t<>>, std::monostate{} }
  {
  }

  constexpr basic_result(failure_t<>)
      : storage_{ std::in_place_type<failure_t<>>, std::monostate{} }
  {
  }

  /// @brief
  ///   in-place constructor for successful result
  template <class... Args>
    requires std::is_constructible_v<T, Args...>
  constexpr explicit basic_result(in_place_ok_t, Args&&... args)
      : storage_{ std::in_place_type<success_t<T>>, std::in_place,
                  std::forward<Args>(args)... }
  {
  }

  /// @brief
  ///   in-place constructor for unsuccessful result
  template <class... Args>
    requires std::is_constructible_v<E, Args&&...>
  constexpr explicit basic_result(in_place_err_t, Args&&... args)
      : storage_{ std::in_place_type<failure_t<E>>, std::in_place,
                  std::forward<Args>(args)... }
  {
  }

  /// @brief
  ///   in-place constructor with initializer_list for successful result
  template <class U, class... Args>
    requires std::is_constructible_v<T, std::initializer_list<U>, Args&&...>
  constexpr explicit basic_result(
      in_place_ok_t, std::initializer_list<U> il, Args&&... args
  )
      : storage_{ std::in_place_type<success_t<T>>, std::in_place, il,
                  std::forward<Args>(args)... }
  {
  }

  /// @brief
  ///   in-place constructor with initializer_list for unsuccessful result
  template <class U, class... Args>
    requires std::is_constructible_v<E, Args&&...>
  constexpr explicit basic_result(
      in_place_err_t, std::initializer_list<U> il, Args&&... args
  )
      : storage_{ std::in_place_type<failure_t<E>>, std::in_place, il,
                  std::forward<Args>(args)... }
  {
  }

  template <class... Args>
    requires std::is_constructible_v<T, Args...>
  MITAMA_VARIANT_CONSTEXPR
  basic_result(success_t<_result_detail::forward_mode<T>, Args...> fwd)
      : storage_()
  {
    std::apply(
        [&](auto&&... args)
        {
          storage_.template emplace<success_t<T>>(
              std::in_place, std::forward<decltype(args)>(args)...
          );
        },
        std::move(fwd)()
    );
  }

  template <class... Args>
    requires std::is_constructible_v<T, Args...>
  MITAMA_VARIANT_CONSTEXPR
  basic_result(success_t<_result_detail::forward_mode<void>, Args...> fwd)
      : storage_()
  {
    std::apply(
        [&](auto&&... args)
        {
          storage_.template emplace<success_t<T>>(
              std::in_place, std::forward<decltype(args)>(args)...
          );
        },
        std::move(fwd)()
    );
  }

  template <class... Args>
    requires std::is_constructible_v<E, Args...>
  MITAMA_VARIANT_CONSTEXPR
  basic_result(failure_t<_result_detail::forward_mode<E>, Args...> fwd)
      : storage_()
  {
    std::apply(
        [&](auto&&... args)
        {
          storage_.template emplace<failure_t<E>>(
              std::in_place, std::forward<decltype(args)>(args)...
          );
        },
        std::move(fwd)()
    );
  }

  template <class... Args>
    requires std::is_constructible_v<E, Args...>
  MITAMA_VARIANT_CONSTEXPR
  basic_result(failure_t<_result_detail::forward_mode<void>, Args...> fwd)
      : storage_()
  {
    std::apply(
        [&](auto&&... args)
        {
          storage_.template emplace<failure_t<E>>(
              std::in_place, std::forward<decltype(args)>(args)...
          );
        },
        std::move(fwd)()
    );
  }

  /// @brief
  ///   Checks if self has a success value.
  ///
  /// @note
  ///   Returns true if the result is succsess.
  constexpr bool is_ok() const noexcept
  {
    return std::holds_alternative<success_t<T>>(storage_);
  }

  /// @brief
  ///   Checks if self has a failure value.
  ///
  /// @note
  ///   Returns true if the result is failure.
  constexpr bool is_err() const noexcept
  {
    return std::holds_alternative<failure_t<E>>(storage_);
  }

  /// @brief
  ///   Returns result storage.
  constexpr decltype(auto) into_storage() &
  {
    return storage_;
  }

  /// @brief
  ///   Returns result storage.
  constexpr decltype(auto) into_storage() const&
  {
    return storage_;
  }

  /// @brief
  ///   Returns result storage.
  constexpr decltype(auto) into_storage() &&
  {
    return std::move(storage_);
  }

  void into_storage() const&& = delete;

  /// @brief
  ///   Converts from basic_result to `maybe<const T>`.
  ///
  /// @note
  ///   Converts self into a `maybe<const T>`, and discarding the failure, if
  ///   any.
  constexpr maybe<std::remove_reference_t<ok_type>> ok() const& noexcept
  {
    if (is_ok())
    {
      return maybe<std::remove_reference_t<ok_type>>(std::in_place, unwrap());
    }
    else
    {
      return nothing;
    }
  }

  /// @brief
  ///   Converts from basic_result to `maybe`.
  ///
  /// @note
  ///   Converts self into a `maybe<const E>`, and discarding the success_t, if
  ///   any.
  constexpr maybe<std::remove_reference_t<err_type>> err() const& noexcept
  {
    if (is_err())
    {
      return maybe<std::remove_reference_t<err_type>>(
          std::in_place, unwrap_err()
      );
    }
    else
    {
      return nothing;
    }
  }

  /// @brief
  ///   Produces a new basic_result, containing a reference into the original,
  ///   leaving the original in place.
  constexpr auto as_ref() const& noexcept -> basic_result<
      _mutability, const std::remove_cvref_t<T>&, const std::remove_cvref_t<E>&>
  {
    if (is_ok())
    {
      return basic_result<
          _mutability, const std::remove_cvref_t<T>&,
          const std::remove_cvref_t<E>&>{
        in_place_ok, std::get<success_t<T>>(storage_).get()
      };
    }
    else
    {
      return basic_result<
          _mutability, const std::remove_cvref_t<T>&,
          const std::remove_cvref_t<E>&>{
        in_place_err, std::get<failure_t<E>>(storage_).get()
      };
    }
  }

  /// @brief
  ///   Converts from `basic_result<mutability::mut, T, E>&` to
  ///   `basic_result<mutability::immut, T&, E&>`.
  constexpr auto as_mut() & noexcept -> basic_result<
      mutability::immut, std::remove_reference_t<T>&,
      std::remove_reference_t<E>&>
  {
    static_assert(
        !std::is_const_v<std::remove_reference_t<T>>,
        "Error: ok_type is immutable"
    );
    static_assert(
        !std::is_const_v<std::remove_reference_t<E>>,
        "Error: err_type is immutable"
    );
    static_assert(is_mut_v<_mutability>, "Error: result is immutable");

    if (is_ok())
    {
      return basic_result<
          mutability::immut, std::remove_reference_t<T>&,
          std::remove_reference_t<E>&>{
        in_place_ok, std::get<success_t<T>>(storage_).get()
      };
    }
    else
    {
      return basic_result<
          mutability::immut, std::remove_reference_t<T>&,
          std::remove_reference_t<E>&>{
        in_place_err, std::get<failure_t<E>>(storage_).get()
      };
    }
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<U, E> by applying a function
  ///   to a contained success value, leaving an failure value untouched.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap()) };
  ///
  /// @note
  ///   This function can be used to compose the results of two functions.
  ///   result<T, E> -> result<U, E>
  template <class O, class... Args>
  constexpr auto
  map(O&& op, Args&&... args
  ) const& noexcept(std::is_nothrow_invocable_v<O, T, Args&&...>)
      -> basic_result<
          _mutability,
          void_to_monostate_t<std::invoke_result_t<O, T, Args&&...>>, E>
    requires std::is_invocable_v<O, T, Args&&...>
  {
    using result_type = basic_result<
        _mutability, void_to_monostate_t<std::invoke_result_t<O, T, Args&&...>>,
        E>;
    if constexpr (std::is_void_v<std::invoke_result_t<O, T, Args&&...>>)
      return is_ok()
             ? std::invoke(
                   std::forward<O>(op), std::get<success_t<T>>(storage_).get(),
                   std::forward<Args>(args)...
               ),
             static_cast<result_type>(success_t{})
             : static_cast<result_type>(failure_t{
                   std::get<failure_t<E>>(storage_).get() });
    else
      return is_ok() ? static_cast<result_type>(success_t{ std::invoke(
                           std::forward<O>(op),
                           std::get<success_t<T>>(storage_).get(),
                           std::forward<Args>(args)...
                       ) })
                     : static_cast<result_type>(failure_t{
                           std::get<failure_t<E>>(storage_).get() });
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<U, E> by applying a function
  ///   to a contained success value, leaving an failure value untouched.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap()) };
  ///
  /// @note
  ///   This function can be used to compose the results of two functions.
  ///   result<void, E> -> result<U, E>
  template <class O, class... Args>
  constexpr auto
  map(O&& op,
      Args&&... args) const& noexcept(std::is_nothrow_invocable_v<O, Args&&...>)
      -> basic_result<
          _mutability, void_to_monostate_t<std::invoke_result_t<O, Args&&...>>,
          E>
    requires std::is_same_v<T, std::monostate>
             && std::is_invocable_v<O, Args&&...>
  {
    using result_type = basic_result<
        _mutability, void_to_monostate_t<std::invoke_result_t<O, Args&&...>>,
        E>;
    if constexpr (std::is_void_v<std::invoke_result_t<O, Args&&...>>)
      return is_ok()
             ? std::invoke(std::forward<O>(op), std::forward<Args>(args)...),
             static_cast<result_type>(success_t{})
             : static_cast<result_type>(failure_t{
                   std::get<failure_t<E>>(storage_).get() });
    else
      return is_ok() ? static_cast<result_type>(success_t{ std::invoke(
                           std::forward<O>(op), std::forward<Args>(args)...
                       ) })
                     : static_cast<result_type>(failure_t{
                           std::get<failure_t<E>>(storage_).get() });
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<U, E> by applying a function
  ///   to a contained success value, leaving an failure value untouched.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap()) }
  ///
  /// @note
  ///   This function can be used to compose the results of two functions.
  ///   result<T, E> -> result<U, E>
  template <class O, class... Args>
  constexpr auto
  map(O&& op,
      Args&&... args) && noexcept(std::is_nothrow_invocable_v<O, T, Args&&...>)
      -> basic_result<
          _mutability,
          void_to_monostate_t<std::invoke_result_t<O, T, Args&&...>>, E>
    requires std::is_invocable_v<O, T, Args&&...>
  {
    using result_type = basic_result<
        _mutability, void_to_monostate_t<std::invoke_result_t<O, T, Args&&...>>,
        E>;
    if constexpr (std::is_void_v<std::invoke_result_t<O, T, Args&&...>>)
      return is_ok() ? std::invoke(
                           std::forward<O>(op),
                           std::move(
                               std::get<success_t<T>>(storage_).get(),
                               std::forward<Args>(args)...
                           )
                       ),
             static_cast<result_type>(success_t{})
                     : static_cast<result_type>(failure_t{
                           std::move(std::get<failure_t<E>>(storage_).get()) });
    else
      return is_ok() ? static_cast<result_type>(success_t{ std::invoke(
                           std::forward<O>(op),
                           std::move(
                               std::get<success_t<T>>(storage_).get(),
                               std::forward<Args>(args)...
                           )
                       ) })
                     : static_cast<result_type>(failure_t{
                           std::move(std::get<failure_t<E>>(storage_).get()) });
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<U, E> by applying a function
  ///   to a contained success value, leaving an failure value untouched.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap()) }
  ///
  /// @note
  ///   This function can be used to compose the results of two functions.
  ///   result<void, E> -> result<U, E>
  template <class O, class... Args>
  constexpr auto
  map(O&& op,
      Args&&... args) && noexcept(std::is_nothrow_invocable_v<O, Args&&...>)
      -> basic_result<
          _mutability, void_to_monostate_t<std::invoke_result_t<O, Args&&...>>,
          E>
    requires std::is_same_v<T, std::monostate>
             && std::is_invocable_v<O, Args&&...>
  {
    using result_type = basic_result<
        _mutability, void_to_monostate_t<std::invoke_result_t<O, Args&&...>>,
        E>;
    if constexpr (std::is_void_v<std::invoke_result_t<O, Args&&...>>)
      return is_ok()
             ? std::invoke(std::forward<O>(op), std::forward<Args>(args)...),
             static_cast<result_type>(success_t{})
             : static_cast<result_type>(failure_t{
                   std::move(std::get<failure_t<E>>(storage_).get()) });
    else
      return is_ok() ? static_cast<result_type>(success_t{ std::invoke(
                           std::forward<O>(op), std::forward<Args>(args)...
                       ) })
                     : static_cast<result_type>(failure_t{
                           std::move(std::get<failure_t<E>>(storage_).get()) });
  }

  /// @brief
  ///   Maps a basic_result<T, E> to U by applying a function to a contained
  ///   success value, or a fallback function to a contained failure value.
  ///
  /// @requires
  ///   { std::invoke(_fallback, unwrap_err()) };
  ///   { std::invoke(_map, unwrap()) };
  ///   Common< decltype(std::invoke(_fallback, unwrap_err())),
  ///   decltype(std::invoke(_map, unwrap())) >;
  ///
  /// @note
  ///   This function can be used to unpack a successful result while handling
  ///   an error.
  template <class Map, class Fallback>
  constexpr auto map_or_else(
      Fallback&& _fallback, Map&& _map
  ) & noexcept(std::is_nothrow_invocable_v<Fallback, E> && std::is_nothrow_invocable_v<Map, T>)
      -> std::common_type_t<
          std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>
    requires std::is_invocable_v<Map, T&> && std::is_invocable_v<Fallback, E&>
             && std::is_convertible_v<
                 std::invoke_result_t<Map, T&>,
                 std::invoke_result_t<Fallback, E&>>
             && std::is_convertible_v<
                 std::invoke_result_t<Fallback, E&>,
                 std::invoke_result_t<Map, T&>>
  {
    using result_type = std::common_type_t<
        std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>;
    return is_ok() ? static_cast<result_type>(std::invoke(
                         std::forward<Map>(_map),
                         std::get<success_t<T>>(storage_).get()
                     ))
                   : static_cast<result_type>(std::invoke(
                         std::forward<Fallback>(_fallback),
                         std::get<failure_t<E>>(storage_).get()
                     ));
  }

  /// @brief
  ///   Maps a basic_result<T, E> to U by applying a function to a contained
  ///   success value, or a fallback function to a contained failure value.
  ///
  /// @requires
  ///   { std::invoke(_fallback, unwrap_err()) };
  ///   { std::invoke(_map, unwrap()) };
  ///   Common< decltype(std::invoke(_fallback, unwrap_err())),
  ///   decltype(std::invoke(_map, unwrap())) >;
  ///
  /// @note
  ///   This function can be used to unpack a successful result while handling
  ///   an error.
  template <class Map, class Fallback>
  constexpr auto map_or_else(
      Fallback&& _fallback, Map&& _map
  ) const& noexcept(std::is_nothrow_invocable_v<Fallback, E> && std::is_nothrow_invocable_v<Map, T>)
      -> std::common_type_t<
          std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>
    requires std::is_invocable_v<Map, T> && std::is_invocable_v<Fallback, E>
             && std::is_convertible_v<
                 std::invoke_result_t<Map, T>,
                 std::invoke_result_t<Fallback, E>>
             && std::is_convertible_v<
                 std::invoke_result_t<Fallback, E>,
                 std::invoke_result_t<Map, T>>
  {
    using result_type = std::common_type_t<
        std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>;
    return is_ok() ? static_cast<result_type>(std::invoke(
                         std::forward<Map>(_map),
                         std::get<success_t<T>>(storage_).get()
                     ))
                   : static_cast<result_type>(std::invoke(
                         std::forward<Fallback>(_fallback),
                         std::get<failure_t<E>>(storage_).get()
                     ));
  }

  /// @brief
  ///   Maps a basic_result<T, E> to U by applying a function to a contained
  ///   success value, or a fallback function to a contained failure value.
  ///
  /// @requires
  ///   { std::invoke(_fallback, unwrap_err()) };
  ///   { std::invoke(_map, unwrap()) };
  ///   Common< decltype(std::invoke(_fallback, unwrap_err())),
  ///   decltype(std::invoke(_map, unwrap())) >;
  ///
  /// @note
  ///   This function can be used to unpack a successful result while handling
  ///   an error.
  template <class Map, class Fallback>
  constexpr auto map_or_else(
      Fallback&& _fallback, Map&& _map
  ) && noexcept(std::is_nothrow_invocable_v<Fallback, E> && std::is_nothrow_invocable_v<Map, T>)
      -> std::common_type_t<
          std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>
    requires std::is_invocable_v<Map, T> && std::is_invocable_v<Fallback, E>
             && std::is_convertible_v<
                 std::invoke_result_t<Map, T>,
                 std::invoke_result_t<Fallback, E>>
             && std::is_convertible_v<
                 std::invoke_result_t<Fallback, E>,
                 std::invoke_result_t<Map, T>>
  {
    using result_type = std::common_type_t<
        std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>;
    return is_ok() ? static_cast<result_type>(std::invoke(
                         std::forward<Map>(_map),
                         std::move(std::get<success_t<T>>(storage_).get())
                     ))
                   : static_cast<result_type>(std::invoke(
                         std::forward<Fallback>(_fallback),
                         std::move(std::get<failure_t<E>>(storage_).get())
                     ));
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<T, F> by applying a function
  ///   to a contained failure value, leaving an success value untouched.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap_err()) }
  ///
  /// @note
  ///   This function can be used to pass through a successful result while
  ///   handling an error. result<T, E> -> result<T, F>
  template <class O, class... Args>
  constexpr auto map_err(O&& op, Args&&... args)
      const& noexcept(std::is_nothrow_invocable_v<O, E, Args&&...>)
          -> basic_result<
              _mutability, T,
              void_to_monostate_t<std::invoke_result_t<O, E, Args&&...>>>
    requires std::is_invocable_v<O, E, Args&&...>
  {
    using result_type = basic_result<
        _mutability, T,
        void_to_monostate_t<std::invoke_result_t<O, E, Args&&...>>>;
    if constexpr (std::is_void_v<std::invoke_result_t<O, E, Args&&...>>)
      return is_err()
             ? std::invoke(
                   std::forward<O>(op), std::get<failure_t<E>>(storage_).get(),
                   std::forward<Args>(args)...
               ),
             static_cast<result_type>(failure_t{})
             : static_cast<result_type>(success_t{
                   std::get<success_t<T>>(storage_).get() });
    else
      return is_err() ? static_cast<result_type>(failure_t{ std::invoke(
                            std::forward<O>(op),
                            std::get<failure_t<E>>(storage_).get(),
                            std::forward<Args>(args)...
                        ) })
                      : static_cast<result_type>(success_t{
                            std::get<success_t<T>>(storage_).get() });
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<T, F> by applying a function
  ///   to a contained failure value, leaving an success value untouched.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap_err()) }
  ///
  /// @note
  ///   This function can be used to pass through a successful result while
  ///   handling an error. result<T, void> -> result<T, F>
  template <class O, class... Args>
  constexpr auto map_err(O&& op, Args&&... args)
      const& noexcept(std::is_nothrow_invocable_v<O, Args&&...>)
          -> basic_result<
              _mutability, T,
              void_to_monostate_t<std::invoke_result_t<O, Args&&...>>>
    requires std::is_same_v<E, std::monostate>
             && std::is_invocable_v<O, Args&&...>
  {
    using result_type = basic_result<
        _mutability, T,
        void_to_monostate_t<std::invoke_result_t<O, Args&&...>>>;
    if constexpr (std::is_void_v<std::invoke_result_t<O, Args&&...>>)
      return is_err()
             ? std::invoke(std::forward<O>(op), std::forward<Args>(args)...),
             static_cast<result_type>(failure_t{})
             : static_cast<result_type>(success_t{
                   std::get<success_t<T>>(storage_).get() });
    else
      return is_err() ? static_cast<result_type>(failure_t{ std::invoke(
                            std::forward<O>(op), std::forward<Args>(args)...
                        ) })
                      : static_cast<result_type>(success_t{
                            std::get<success_t<T>>(storage_).get() });
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<T, F> by applying a function
  ///   to a contained failure value, leaving an success value untouched.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap_err()) }
  ///
  /// @note
  ///   This function can be used to pass through a successful result while
  ///   handling an error. result<T, E> -> result<T, F>
  template <class O, class... Args>
  constexpr auto map_err(
      O&& op, Args&&... args
  ) && noexcept(std::is_nothrow_invocable_v<O, E, Args&&...>)
      -> basic_result<
          _mutability, T,
          void_to_monostate_t<std::invoke_result_t<O, E, Args&&...>>>
    requires std::is_invocable_v<O, E, Args&&...>
  {
    using result_type = basic_result<
        _mutability, T,
        void_to_monostate_t<std::invoke_result_t<O, E, Args&&...>>>;
    if constexpr (std::is_void_v<std::invoke_result_t<O, E, Args&&...>>)
      return is_err()
             ? std::invoke(
                   std::forward<O>(op),
                   std::move(std::get<failure_t<E>>(storage_).get()),
                   std::forward<Args>(args)...
               ),
             static_cast<result_type>(failure_t{})
             : static_cast<result_type>(success_t{
                   std::move(std::get<success_t<T>>(storage_).get()) });
    else
      return is_err()
                 ? static_cast<result_type>(failure_t{ std::invoke(
                       std::forward<O>(op),
                       std::move(std::get<failure_t<E>>(storage_).get()),
                       std::forward<Args>(args)...
                   ) })
                 : static_cast<result_type>(success_t{
                       std::move(std::get<success_t<T>>(storage_).get()) });
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<T, F> by applying a function
  ///   to a contained failure value, leaving an success value untouched.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap_err()) }
  ///
  /// @note
  ///   This function can be used to pass through a successful result while
  ///   handling an error. result<T, void> -> result<T, F>
  template <class O, class... Args>
  constexpr auto map_err(
      O&& op, Args&&... args
  ) && noexcept(std::is_nothrow_invocable_v<O, Args&&...>)
      -> basic_result<
          _mutability, T,
          void_to_monostate_t<std::invoke_result_t<O, Args&&...>>>
    requires std::is_same_v<E, std::monostate>
             && std::is_invocable_v<O, Args&&...>
  {
    using result_type = basic_result<
        _mutability, T,
        void_to_monostate_t<std::invoke_result_t<O, Args&&...>>>;
    if constexpr (std::is_void_v<std::invoke_result_t<O, Args&&...>>)
      return is_err()
             ? std::invoke(std::forward<O>(op), std::forward<Args>(args)...),
             static_cast<result_type>(failure_t{})
             : static_cast<result_type>(success_t{
                   std::move(std::get<success_t<T>>(storage_).get()) });
    else
      return is_err()
                 ? static_cast<result_type>(failure_t{ std::invoke(
                       std::forward<O>(op), std::forward<Args>(args)...
                   ) })
                 : static_cast<result_type>(success_t{
                       std::move(std::get<success_t<T>>(storage_).get()) });
  }

  /// @brief
  ///   Calls `op` if the result is success, otherwise; returns the failure
  ///   value of self.
  ///
  /// @requires
  ///   requires requires is_convertible_result_with<std::invoke_result_t<O, T>,
  ///   failure_t<F>>
  ///
  /// @note
  ///   This function can be used for control flow based on result values.
  template <class O, class... Args>
  constexpr auto and_then(O&& op, Args&&... args)
      const& noexcept(std::is_nothrow_invocable_v<O, T, Args&&...>)
          -> std::invoke_result_t<O, T, Args&&...>
    requires is_convertible_result_with_v<
        std::invoke_result_t<O, T, Args&&...>, failure_t<E>>
  {
    using result_type = std::invoke_result_t<O, T, Args&&...>;
    return is_ok() ? std::invoke(
                         std::forward<O>(op),
                         std::get<success_t<T>>(storage_).get(),
                         std::forward<Args>(args)...
                     )
                   : static_cast<result_type>(failure_t{
                         std::get<failure_t<E>>(storage_).get() });
  }

  /// @brief
  ///   Calls `op` if the result is success, otherwise; returns the failure
  ///   value of self.
  ///
  /// @requires
  ///   requires requires is_convertible_result_with<std::invoke_result_t<O, T>,
  ///   success_t<T>>
  ///
  /// @note
  ///   This function can be used for control flow based on result values.
  template <class O, class... Args>
  constexpr auto and_then(
      O&& op, Args&&... args
  ) && noexcept(std::is_nothrow_invocable_v<O, T, Args&&...>)
      -> std::invoke_result_t<O, T, Args&&...>
    requires is_convertible_result_with_v<
        std::invoke_result_t<O, T, Args&&...>, failure_t<E>>
  {
    using result_type = std::invoke_result_t<O, T, Args&&...>;
    return is_ok() ? std::invoke(
                         std::forward<O>(op),
                         std::move(std::get<success_t<T>>(storage_).get()),
                         std::forward<Args>(args)...
                     )
                   : static_cast<result_type>(failure_t{
                         std::move(std::get<failure_t<E>>(storage_).get()) });
  }

  /// @brief
  ///   Calls `op` if the result is failure, otherwise; returns the success
  ///   value of self.
  ///
  /// @requires
  ///   requires requires is_convertible_result_with<std::invoke_result_t<O, T>,
  ///   success_t<T>>
  ///
  /// @note
  ///   This function can be used for control flow based on result values.
  template <class O, class... Args>
  constexpr auto or_else(O&& op, Args&&... args)
      const& noexcept(std::is_nothrow_invocable_v<O, E, Args&&...>)
          -> std::invoke_result_t<O, E, Args&&...>
    requires is_convertible_result_with_v<
        std::invoke_result_t<O, T, Args&&...>, success_t<T>>
  {
    using result_type = std::invoke_result_t<O, E, Args&&...>;
    return is_err() ? std::invoke(
                          std::forward<O>(op),
                          std::get<failure_t<E>>(storage_).get(),
                          std::forward<Args>(args)...
                      )
                    : static_cast<result_type>(success_t{
                          std::get<success_t<T>>(storage_).get() });
  }

  /// @brief
  ///   Calls `op` if the result is failure, otherwise; returns the success
  ///   value of self.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap_err()) } -> ConvertibleTo<basic_result<T, U>>
  ///
  /// @note
  ///   This function can be used for control flow based on result values.
  template <class O, class... Args>
  constexpr auto or_else(
      O&& op, Args&&... args
  ) && noexcept(std::is_nothrow_invocable_v<O, E, Args&&...>)
      -> std::invoke_result_t<O, E, Args&&...>
    requires is_convertible_result_with_v<
        std::invoke_result_t<O, T, Args&&...>, success_t<T>>
  {
    using result_type = std::invoke_result_t<O, E, Args&&...>;
    return is_err() ? std::invoke(
                          std::forward<O>(op),
                          std::get<failure_t<E>>(std::move(storage_)).get(),
                          std::forward<Args>(args)...
                      )
                    : static_cast<result_type>(success_t{
                          std::get<success_t<T>>(std::move(storage_)).get() });
  }

  /// @brief
  ///   Returns `res` if the result is success_t, otherwise; returns the failure
  ///   value of self.
  template <mutability _mu, class U>
  constexpr decltype(auto) conj(const basic_result<_mu, U, E>& res
  ) const& noexcept
  {
    using result_type = basic_result<_mutability && _mu, U, E>;
    return this->is_err() ? static_cast<result_type>(failure_t{
                                std::get<failure_t<E>>(storage_).get() })
           : res.is_err()
               ? static_cast<result_type>(failure_t{ res.unwrap_err() })
               : static_cast<result_type>(success_t{ res.unwrap() });
  }

  /// @brief
  ///   Returns `res` if the result is success, otherwise; returns the failure
  ///   value of self.
  template <mutability _mu, class U>
  constexpr decltype(auto) operator&&(const basic_result<_mu, U, E>& res
  ) const& noexcept
  {
    return this->conj(res);
  }

  /// @brief
  ///   Returns res if the result is failure, otherwise returns the success
  ///   value of self.
  ///
  /// @note
  ///   Arguments passed to or are eagerly evaluated;
  ///   if you are passing the result of a function call,
  ///   it is recommended to use `or_else`,
  ///   which is lazily evaluated.
  template <mutability _mut, class F>
  constexpr decltype(auto) disj(const basic_result<_mut, T, F>& res
  ) const& noexcept
  {
    using result_type = basic_result<_mutability, T, F>;
    return this->is_ok() ? static_cast<result_type>(success_t{
                               std::get<success_t<T>>(storage_).get() })
           : res.is_ok()
               ? static_cast<result_type>(success_t{ res.unwrap() })
               : static_cast<result_type>(failure_t{ res.unwrap_err() });
  }

  /// @brief
  ///   Returns res if the result is failure, otherwise returns the success
  ///   value of self.
  ///
  /// @note
  ///   Arguments passed to or are eagerly evaluated;
  ///   if you are passing the result of a function call,
  ///   it is recommended to use `or_else`,
  ///   which is lazily evaluated.
  template <mutability _mut, class F>
  constexpr decltype(auto) operator||(const basic_result<_mut, T, F>& res
  ) const& noexcept
  {
    return this->disj(res);
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success_t.
  ///   Else, it returns optb.
  ///
  /// @requires
  ///   { is_ok() ? unwrap() : optb }
  ///
  /// @note
  ///   Arguments passed to `unwrap_or` are eagerly evaluated;
  ///   if you are passing the result of a function call,
  ///   it is recommended to use `unwrap_or_else`,
  ///   which is lazily evaluated.
  template <class U>
    requires meta::has_common_type<T, U&&>::value
  constexpr decltype(auto) unwrap_or(U&& optb) const& noexcept
  {
    return is_ok() ? std::get<success_t<T>>(storage_).get()
                   : std::forward<U>(optb);
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///   Else, it returns optb.
  ///
  /// @requires
  ///   { is_ok() ? unwrap() : optb }
  ///
  /// @note
  ///   Arguments passed to `unwrap_or` are eagerly evaluated;
  ///   if you are passing the result of a function call,
  ///   it is recommended to use `unwrap_or_else`,
  ///   which is lazily evaluated.
  template <class U>
    requires meta::has_common_type<std::remove_reference_t<T>&&, U&&>::value
  constexpr decltype(auto) unwrap_or(U&& optb) && noexcept
  {
    return is_ok() ? std::move(std::get<success_t<T>>(storage_).get())
                   : std::forward<U>(optb);
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap_err()) } -> ConvertibleTo<T> ||
  ///   { std::invoke(op) } -> ConvertibleTo<T>
  ///
  /// @note
  ///   If the value is an failure then;
  ///     - `std::is_invocable_r_v<T, O, E>` is true then; it invoke `op` with
  ///     its value or,
  ///     - `std::is_invocable_r_v<T, O>` is true then; it invoke `op` without
  ///     value,
  ///     - otherwise; static_assert.
  template <class O>
    requires std::is_invocable_r_v<T, O, E> || std::is_invocable_r_v<T, O>
  constexpr T unwrap_or_else(O&& op) const
      noexcept(std::disjunction_v<
               std::conjunction<
                   std::is_invocable_r<T, O, E>,
                   std::is_nothrow_invocable_r<T, O, E>>,
               std::conjunction<
                   std::is_invocable_r<T, O>,
                   std::is_nothrow_invocable_r<T, O>>>)
  {
    if constexpr (std::is_invocable_r_v<T, O, E>)
    {
      return is_ok() ? std::get<success_t<T>>(storage_).get()
                     : std::invoke(
                           std::forward<O>(op),
                           std::get<failure_t<E>>(storage_).get()
                       );
    }
    else if constexpr (std::is_invocable_r_v<T, O>)
    {
      return is_ok() ? std::get<success_t<T>>(storage_).get()
                     : std::invoke(std::forward<O>(op));
    }
    else
    {
      static_assert(
          [] { return false; }(),
          "invalid argument: designated function object is not invocable"
      );
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @panics
  ///   Panics if the value is an failure, with a panic message provided by the
  ///   failure's value.
  constexpr force_add_const_t<T>& unwrap(
      const std::source_location& loc = std::source_location::current()
  ) const&
  {
    if constexpr (fmt::is_formattable<E>::value)
    {
      if (is_ok())
      {
        return std::get<success_t<T>>(storage_).get();
      }
      else
      {
        PANIC(
            "called `basic_result::unwrap()` on a value: `{}`",
            std::get<failure_t<E>>(storage_), loc
        );
      }
    }
    else
    {
      if (is_ok())
      {
        return std::get<success_t<T>>(storage_).get();
      }
      else
      {
        PANIC("called `basic_result::unwrap()` on a value `failure(?)`", loc);
      }
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @panics
  ///   Panics if the value is an failure, with a panic message provided by the
  ///   failure's value.
  constexpr std::conditional_t<is_mut_v<_mutability>, T&, force_add_const_t<T>&>
  unwrap(const std::source_location& loc = std::source_location::current()) &
  {
    if constexpr (fmt::is_formattable<E>::value)
    {
      if (is_ok())
      {
        return std::get<success_t<T>>(storage_).get();
      }
      else
      {
        PANIC(
            "called `basic_result::unwrap()` on a value: `{}`",
            std::get<failure_t<E>>(storage_), loc
        );
      }
    }
    else
    {
      if (is_ok())
      {
        return std::get<success_t<T>>(storage_).get();
      }
      else
      {
        PANIC("called `basic_result::unwrap()` on a value `failure_t(?)`", loc);
      }
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an failure.
  ///
  /// @panics
  ///   Panics if the value is an success, with a panic message provided by the
  ///   success's value.
  constexpr force_add_const_t<E>& unwrap_err(
      const std::source_location& loc = std::source_location::current()
  ) const&
  {
    if constexpr (fmt::is_formattable<T>::value)
    {
      if (is_err())
      {
        return std::get<failure_t<E>>(storage_).get();
      }
      else
      {
        PANIC(
            "called `basic_result::unwrap_err()` on a value: `{}`",
            std::get<success_t<T>>(storage_), loc
        );
      }
    }
    else
    {
      if (is_err())
      {
        return std::get<failure_t<E>>(storage_).get();
      }
      else
      {
        PANIC(
            "called `basic_result::unwrap_err()` on a value `success_t(?)`", loc
        );
      }
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an failure.
  ///
  /// @panics
  ///   Panics if the value is an success, with a panic message provided by the
  ///   success's value.
  constexpr std::conditional_t<is_mut_v<_mutability>, E&, force_add_const_t<E>&>
  unwrap_err(
      const std::source_location& loc = std::source_location::current()
  ) &
  {
    if constexpr (fmt::is_formattable<T>::value)
    {
      if (is_err())
      {
        return std::get<failure_t<E>>(storage_).get();
      }
      else
      {
        PANIC(
            "called `basic_result::unwrap_err()` on a value: `{}`",
            std::get<success_t<T>>(storage_), loc
        );
      }
    }
    else
    {
      if (is_err())
      {
        return std::get<failure_t<E>>(storage_).get();
      }
      else
      {
        PANIC(
            "called `basic_result::unwrap_err()` on a value `success_t(?)`)",
            loc
        );
      }
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @panics
  ///   Panics if the value is an failure, with a panic message including the
  ///   passed message, and the content of the failure.
  constexpr force_add_const_t<T>& expect(
      std::string_view msg,
      const std::source_location& loc = std::source_location::current()
  ) const&
  {
    if (is_err())
    {
      PANIC("{}: {}", msg, unwrap_err(), loc);
    }
    else
    {
      return unwrap();
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @panics
  ///   Panics if the value is an failure, with a panic message including the
  ///   passed message, and the content of the failure.
  constexpr decltype(auto) expect(
      std::string_view msg,
      const std::source_location& loc = std::source_location::current()
  ) &
  {
    if (is_err())
    {
      PANIC("{}: {}", msg, unwrap_err(), loc);
    }
    else
    {
      return unwrap();
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an failure.
  ///
  /// @panics
  ///   Panics if the value is an success, with a panic message including the
  ///   passed message, and the content of the success.
  constexpr force_add_const_t<E>& expect_err(
      std::string_view msg,
      const std::source_location& loc = std::source_location::current()
  ) const&
  {
    if (is_ok())
    {
      PANIC("{}: {}", msg, unwrap_err(), loc);
    }
    else
    {
      return unwrap_err();
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an failure.
  ///
  /// @panics
  ///   Panics if the value is an success, with a panic message including the
  ///   passed message, and the content of the success.
  constexpr decltype(auto) expect_err(
      std::string_view msg,
      const std::source_location& loc = std::source_location::current()
  ) &
  {
    if (is_ok())
    {
      PANIC("{}: {}", msg, unwrap_err(), loc);
    }
    else
    {
      return unwrap_err();
    }
  }

  template <class F>
    requires std::is_invocable_v<F&&, T>
  constexpr void and_finally(F&& f) const&
  {
    if (this->is_ok())
      std::invoke(std::forward<F>(f), unwrap());
  }

  template <class F>
    requires std::is_invocable_v<F&&, E>
  constexpr void or_finally(F&& f) const&
  {
    if (this->is_err())
      std::invoke(std::forward<F>(f), unwrap_err());
  }

  template <class F>
    requires std::is_invocable_v<F, T&> || std::is_invocable_v<F>
  constexpr basic_result& and_peek(F&& f) &
  {
    if constexpr (std::is_invocable_v<F, T&>)
    {
      if (is_ok())
        std::invoke(std::forward<F>(f), unwrap());
    }
    else
    {
      if (is_ok())
        std::invoke(std::forward<F>(f));
    }
    return *this;
  }

  template <class F>
    requires std::is_invocable_v<F, const T&> || std::is_invocable_v<F>
  constexpr const basic_result& and_peek(F&& f) const&
  {
    if constexpr (std::is_invocable_v<F, const T&>)
    {
      if (is_ok())
        std::invoke(std::forward<F>(f), unwrap());
    }
    else
    {
      if (is_ok())
        std::invoke(std::forward<F>(f));
    }
    return *this;
  }

  template <class F>
    requires std::is_invocable_v<F, T&&> || std::is_invocable_v<F>
  constexpr basic_result&& and_peek(F&& f) &&
  {
    if constexpr (std::is_invocable_v<F, T&&>)
    {
      if (is_ok())
        std::invoke(std::forward<F>(f), unwrap());
    }
    else
    {
      if (is_ok())
        std::invoke(std::forward<F>(f));
    }
    return std::move(*this);
  }

  template <class F>
    requires std::is_invocable_v<F, E&> || std::is_invocable_v<F>
  constexpr basic_result& or_peek(F&& f) &
  {
    if constexpr (std::is_invocable_v<F, E&>)
    {
      if (is_err())
        std::invoke(std::forward<F>(f), unwrap_err());
    }
    else
    {
      if (is_err())
        std::invoke(std::forward<F>(f));
    }
    return *this;
  }

  template <class F>
    requires std::is_invocable_v<F, const E&> || std::is_invocable_v<F>
  constexpr const basic_result& or_peek(F&& f) const&
  {
    if constexpr (std::is_invocable_v<F, const E&>)
    {
      if (is_err())
        std::invoke(std::forward<F>(f), unwrap_err());
    }
    else
    {
      if (is_err())
        std::invoke(std::forward<F>(f));
    }
    return *this;
  }

  template <class F>
    requires std::is_invocable_v<F, E&&> || std::is_invocable_v<F>
  constexpr basic_result&& or_peek(F&& f) &&
  {
    if constexpr (std::is_invocable_v<F, E&&>)
    {
      if (is_err())
        std::invoke(std::forward<F>(f), unwrap_err());
    }
    else
    {
      if (is_err())
        std::invoke(std::forward<F>(f));
    }
    return std::move(*this);
  }

  template <class F>
  constexpr auto map_anything_else(F&& f
  ) & noexcept(std::is_nothrow_invocable_v<F, E&> && std::is_nothrow_invocable_v<F, T&>)
      -> std::common_type_t<
          std::invoke_result_t<F, T&>, std::invoke_result_t<F, E&>>
    requires std::is_invocable_v<F, T&> && std::is_invocable_v<F, E&>
             && std::is_convertible_v<
                 std::invoke_result_t<F, T&>, std::invoke_result_t<F, E&>>
             && std::is_convertible_v<
                 std::invoke_result_t<F, E&>, std::invoke_result_t<F, T&>>
  {
    auto decay_copy =
        [](auto&& some
        ) -> std::remove_const_t<std::remove_reference_t<decltype(some)>>
    { return std::forward<decltype(some)>(some); };
    return this->map_or_else(
        decay_copy(std::forward<F>(f)), decay_copy(std::forward<F>(f))
    );
  }

  template <class F>
  constexpr auto map_anything_else(F&& f
  ) const& noexcept(std::is_nothrow_invocable_v<F, const E&> && std::is_nothrow_invocable_v<F, const T&>)
      -> std::common_type_t<
          std::invoke_result_t<F, const T&>, std::invoke_result_t<F, const E&>>
    requires std::is_invocable_v<F, const T&>
             && std::is_invocable_v<F, const E&>
             && std::is_convertible_v<
                 std::invoke_result_t<F, const T&>,
                 std::invoke_result_t<F, const E&>>
             && std::is_convertible_v<
                 std::invoke_result_t<F, const E&>,
                 std::invoke_result_t<F, const T&>>
  {
    auto decay_copy =
        [](auto&& some
        ) -> std::remove_const_t<std::remove_reference_t<decltype(some)>>
    { return std::forward<decltype(some)>(some); };
    return this->map_or_else(
        decay_copy(std::forward<F>(f)), decay_copy(std::forward<F>(f))
    );
  }

  template <class F>
  constexpr auto map_anything_else(F&& f
  ) && noexcept(std::is_nothrow_invocable_v<F, E&&> && std::is_nothrow_invocable_v<F, T&&>)
      -> std::common_type_t<
          std::invoke_result_t<F, T&&>, std::invoke_result_t<F, E&&>>
    requires std::is_invocable_v<F, T&&> && std::is_invocable_v<F, E&&>
             && std::is_convertible_v<
                 std::invoke_result_t<F, T&&>, std::invoke_result_t<F, E&&>>
             && std::is_convertible_v<
                 std::invoke_result_t<F, E&&>, std::invoke_result_t<F, T&&>>
  {
    auto decay_copy =
        [](auto&& some
        ) -> std::remove_const_t<std::remove_reference_t<decltype(some)>>
    { return std::forward<decltype(some)>(some); };
    return std::move(*this).map_or_else(
        decay_copy(std::forward<F>(f)), decay_copy(std::forward<F>(f))
    );
  }

  /// @brief
  ///   equal compare
  ///
  /// @requires
  ///   (T a, U b) { a == b } -> Same<bool>;
  ///   (E a, F b) { a == b } -> Same<bool>
  ///
  /// @note
  ///   This method tests for self and other values to be equal, and is used by
  ///   `==` found by ADL.
  template <mutability _mut, class U, class F>
    requires is_comparable_with<T, U>::value && is_comparable_with<E, F>::value
  constexpr bool operator==(const basic_result<_mut, U, F>& rhs) const&
  {
    return std::visit(
        _result_detail::overload{
            [](const success_t<T>& l, const success_t<U>& r)
            { return l.get() == r.get(); },
            [](const failure_t<E>& l, const failure_t<F>& r)
            { return l.get() == r.get(); }, //
            [](auto&&...) { return false; } //
        },
        this->storage_, rhs.storage_
    );
  }

  /// @brief
  ///   not equal compare
  ///
  /// @requires
  ///   (T a, U b) { a == b } -> Same<bool>;
  ///   (E a, F b) { a == b } -> Same<bool>
  ///
  /// @note
  ///   This method tests for self and other values to be not equal, and is used
  ///   by `==` found by ADL.
  template <mutability _mut, class U, class F>
    requires is_comparable_with<T, U>::value && is_comparable_with<E, F>::value
  constexpr bool operator!=(const basic_result<_mut, U, F>& rhs) const&
  {
    return !(*this == rhs);
  }

  /// @brief
  ///   equal compare
  ///
  /// @requires
  ///   (T a, U b) { a == b } -> Same<bool>
  ///
  /// @note
  ///   This method tests for self and other values to be equal, and is used by
  ///   `==` found by ADL.
  template <class U>
    requires is_comparable_with<T, U>::value
  constexpr bool operator==(const success_t<U>& rhs) const
  {
    return this->is_ok() ? this->unwrap() == rhs.get() : false;
  }

  /// @brief
  ///   not equal compare
  ///
  /// @requires
  ///   (T a, U b) { a == b } -> Same<bool>
  ///
  /// @note
  ///   This method tests for self and other values to be not equal, and is used
  ///   by `==` found by ADL.
  template <class U>
    requires is_comparable_with<T, U>::value
  constexpr bool operator!=(const success_t<U>& rhs) const
  {
    return this->is_ok() ? !(this->unwrap() == rhs.get()) : true;
  }

  /// @brief
  ///   equal compare
  ///
  /// @requires
  ///   (E a, F b) { a == b } -> Same<bool>
  ///
  /// @note
  ///   This method tests for self and other values to be equal, and is used by
  ///   `==` found by ADL.
  template <class F>
    requires is_comparable_with<E, F>::value
  constexpr bool operator==(const failure_t<F>& rhs) const
  {
    return this->is_err() ? this->unwrap_err() == rhs.get() : false;
  }

  /// @brief
  ///   not equal compare
  ///
  /// @requires
  ///   (E a, F b) { a == b } -> Same<bool>
  ///
  /// @note
  ///   This method tests for self and other values to be equal, and is used by
  ///   `==` found by ADL.
  template <class F>
    requires is_comparable_with<E, F>::value
  constexpr bool operator!=(const failure_t<F>& rhs) const
  {
    return this->is_err() ? !(this->unwrap_err() == rhs.get()) : true;
  }

  template <mutability _, class U, class F>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_less_comparable_with<E, F>::value
  constexpr bool operator<(const basic_result<_, U, F>& rhs) const
  {
    return std::visit(
        _result_detail::overload_linearly(
            [](const success_t<T>& l, const success_t<U>& r)
            { return l.get() < r.get(); },
            [](const failure_t<E>& l, const failure_t<F>& r)
            { return l.get() < r.get(); },
            [](const failure_t<E>&, const success_t<U>&) { return true; },
            [](const success_t<T>&, const failure_t<F>&) { return false; },
            [](const auto&, const auto&) { return false; }
        ),
        this->storage_, rhs.storage_
    );
  }

  template <class U>
    requires meta::is_less_comparable_with<U, T>::value
  constexpr bool operator<(const success_t<U>& rhs) const
  {
    return rhs > *this;
  }

  template <class F>
    requires meta::is_less_comparable_with<F, E>::value
  constexpr bool operator<(const failure_t<F>& rhs) const
  {
    return rhs > *this;
  }

  template <mutability _, class U, class F>
    requires meta::is_less_comparable_with<U, T>::value
             && meta::is_less_comparable_with<F, E>::value
  constexpr bool operator>(const basic_result<_, U, F>& rhs) const
  {
    return std::visit(
        _result_detail::overload_linearly(
            [](const success_t<T>& l, const success_t<U>& r)
            { return r.get() < l.get(); },
            [](const failure_t<E>& l, const failure_t<F>& r)
            { return r.get() < l.get(); },
            [](const failure_t<E>&, const success_t<U>&) { return false; },
            [](const success_t<T>&, const failure_t<F>&) { return true; },
            [](const auto&, const auto&) { return false; }
        ),
        this->storage_, rhs.storage_
    );
  }

  template <class U>
    requires meta::is_less_comparable_with<U, T>::value
  constexpr bool operator>(const success_t<U>& rhs) const
  {
    return rhs < *this;
  }

  template <class F>
    requires meta::is_less_comparable_with<F, E>::value
  constexpr bool operator>(const failure_t<F>& rhs) const
  {
    return rhs < *this;
  }

  template <mutability _, class U, class F>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_less_comparable_with<E, F>::value
             && is_comparable_with<T, U>::value
             && is_comparable_with<E, F>::value
  constexpr bool operator<=(const basic_result<_, U, F>& rhs) const
  {
    return std::visit(
        _result_detail::overload_linearly(
            [](const success_t<T>& l, const success_t<U>& r)
            { return (l.get() == r.get()) || (l.get() < r.get()); },
            [](const failure_t<E>& l, const failure_t<F>& r)
            { return (l.get() == r.get()) || (l.get() < r.get()); },
            [](const failure_t<E>&, const success_t<U>&) { return true; },
            [](const success_t<T>&, const failure_t<F>&) { return false; },
            [](const auto&, const auto&) { return false; }
        ),
        this->storage_, rhs.storage_
    );
  }

  template <class U>
    requires meta::is_less_comparable_with<U, T>::value
             && is_comparable_with<U, T>::value
  constexpr bool operator<=(const success_t<U>& rhs) const
  {
    return rhs >= *this;
  }

  template <class F>
    requires meta::is_less_comparable_with<F, E>::value
             && is_comparable_with<F, E>::value
  constexpr bool operator<=(const failure_t<F>& rhs) const
  {
    return rhs >= *this;
  }

  template <mutability _, class U, class F>
    requires meta::is_less_comparable_with<T, U>::value
             && meta::is_less_comparable_with<E, F>::value
             && is_comparable_with<T, U>::value
             && is_comparable_with<E, F>::value
  constexpr bool operator>=(const basic_result<_, U, F>& rhs) const
  {
    return std::visit(
        _result_detail::overload_linearly(
            [](const success_t<T>& l, const success_t<U>& r)
            { return (l.get() == r.get()) || (r.get() < l.get()); },
            [](const failure_t<E>& l, const failure_t<F>& r)
            { return (l.get() == r.get()) || (r.get() < l.get()); },
            [](const failure_t<E>&, const success_t<U>&) { return false; },
            [](const success_t<T>&, const failure_t<F>&) { return true; },
            [](const auto&, const auto&) { return false; }
        ),
        this->storage_, rhs.storage_
    );
  }

  template <class U>
    requires meta::is_less_comparable_with<U, T>::value
             && is_comparable_with<U, T>::value
  constexpr bool operator>=(const success_t<U>& rhs) const
  {
    return rhs <= *this;
  }

  template <class F>
    requires meta::is_less_comparable_with<F, E>::value
             && is_comparable_with<F, E>::value
  constexpr bool operator>=(const failure_t<F>& rhs) const
  {
    return rhs <= *this;
  }

  template <class Ctx>
  auto with_context(Ctx ctx)
      -> basic_result<_mutability, T, std::shared_ptr<anyhow::error>>
    requires std::is_invocable_r_v<std::shared_ptr<anyhow::error>, Ctx>
  {
    return this->map_err(
        [&](auto err) -> std::shared_ptr<anyhow::error>
        { return err->context(std::invoke(ctx)); }
    );
  }
};

template <mutability _, class T, class E, class U>
  requires(!(is_result<std::decay_t<U>>::value
             || is_ok_type<std::decay_t<U>>::value
             || is_err_type<std::decay_t<U>>::value))
          && is_comparable_with<T, U>::value
constexpr bool
operator==(const basic_result<_, T, E>& lhs, U&& rhs)
{
  return lhs == success_t(std::forward<U>(rhs));
}

template <mutability _, class T, class E, class U>
  requires(!(is_result<std::decay_t<T>>::value
             || is_ok_type<std::decay_t<T>>::value
             || is_err_type<std::decay_t<T>>::value))
          && is_comparable_with<T, U>::value
constexpr bool
operator==(T&& lhs, const basic_result<_, U, E>& rhs)
{
  return success_t(std::forward<T>(lhs)) == rhs;
}

template <mutability _, class T, class E, class U>
  requires(!(is_result<std::decay_t<U>>::value
             || is_ok_type<std::decay_t<U>>::value
             || is_err_type<std::decay_t<U>>::value))
          && is_comparable_with<T, U>::value
constexpr bool
operator!=(const basic_result<_, T, E>& lhs, U&& rhs)
{
  return lhs != success_t(std::forward<U>(rhs));
}

template <mutability _, class T, class E, class U>
  requires(!(is_result<std::decay_t<T>>::value
             || is_ok_type<std::decay_t<T>>::value
             || is_err_type<std::decay_t<T>>::value))
          && is_comparable_with<T, U>::value
constexpr bool
operator!=(T&& lhs, const basic_result<_, U, E>& rhs)
{
  return success_t(std::forward<T>(lhs)) != rhs;
}

template <mutability _, class T, class E, class U>
  requires(!(is_result<std::decay_t<U>>::value
             || is_ok_type<std::decay_t<U>>::value
             || is_err_type<std::decay_t<U>>::value))
          && meta::is_less_comparable_with<T, U>::value
constexpr bool
operator<(const basic_result<_, T, E>& lhs, U&& rhs)
{
  return lhs < success_t(std::forward<U>(rhs));
}

template <mutability _, class T, class E, class U>
  requires(!(is_result<std::decay_t<T>>::value
             || is_ok_type<std::decay_t<T>>::value
             || is_err_type<std::decay_t<T>>::value))
          && meta::is_less_comparable_with<T, U>::value
constexpr bool
operator<(T&& lhs, const basic_result<_, U, E>& rhs)
{
  return success_t(std::forward<T>(lhs)) < rhs;
}

template <mutability _, class T, class E, class U>
  requires(!(is_result<std::decay_t<U>>::value
             || is_ok_type<std::decay_t<U>>::value
             || is_err_type<std::decay_t<U>>::value))
          && meta::is_less_comparable_with<T, U>::value
          && is_comparable_with<T, U>::value
constexpr bool
operator<=(const basic_result<_, T, E>& lhs, U&& rhs)
{
  return lhs <= success_t(std::forward<U>(rhs));
}

template <mutability _, class T, class E, class U>
  requires(!(is_result<std::decay_t<T>>::value
             || is_ok_type<std::decay_t<T>>::value
             || is_err_type<std::decay_t<T>>::value))
          && meta::is_less_comparable_with<T, U>::value
          && is_comparable_with<T, U>::value
constexpr bool
operator<=(T&& lhs, const basic_result<_, U, E>& rhs)
{
  return success_t(std::forward<T>(lhs)) <= rhs;
}

template <mutability _, class T, class E, class U>
  requires(!(is_result<std::decay_t<U>>::value
             || is_ok_type<std::decay_t<U>>::value
             || is_err_type<std::decay_t<U>>::value))
          && meta::is_less_comparable_with<U, T>::value
constexpr bool
operator>(const basic_result<_, T, E>& lhs, U&& rhs)
{
  return lhs > success_t(std::forward<U>(rhs));
}

template <mutability _, class T, class E, class U>
  requires(!(is_result<std::decay_t<T>>::value
             || is_ok_type<std::decay_t<T>>::value
             || is_err_type<std::decay_t<T>>::value))
          && meta::is_less_comparable_with<U, T>::value
constexpr bool
operator>(T&& lhs, const basic_result<_, U, E>& rhs)
{
  return success_t(std::forward<T>(lhs)) > rhs;
}

template <mutability _, class T, class E, class U>
  requires(!(is_result<std::decay_t<U>>::value
             || is_ok_type<std::decay_t<U>>::value
             || is_err_type<std::decay_t<U>>::value))
          && meta::is_less_comparable_with<U, T>::value
          && is_comparable_with<U, T>::value
constexpr bool
operator>=(const basic_result<_, T, E>& lhs, U&& rhs)
{
  return lhs >= success_t(std::forward<U>(rhs));
}

template <mutability _, class T, class E, class U>
  requires(!(is_result<std::decay_t<T>>::value
             || is_ok_type<std::decay_t<T>>::value
             || is_err_type<std::decay_t<T>>::value))
          && meta::is_less_comparable_with<U, T>::value
          && is_comparable_with<U, T>::value
constexpr bool
operator>=(T&& lhs, const basic_result<_, U, E>& rhs)
{
  return success_t(std::forward<T>(lhs)) >= rhs;
}

} // namespace mitama

// MSVC does not implement compound statements (ref:
// https://stackoverflow.com/q/5291532)
#if defined(__clang__) || defined(__GNUC__)
#  define MITAMA_CPP_RESULT_TRY_MAY_NOT_PANIC true
#  define MITAMA_TRY_IMPL(...)                                              \
    ({                                                                      \
      auto&& result = ::mitama::_result_detail::id(__VA_ARGS__);            \
      static_assert(                                                        \
          ::mitama::is_result_v<std::remove_cvref_t<decltype(result)>>,     \
          "You should pass mitama::result type to this MITAMA_TRY macro."   \
      );                                                                    \
      if (result.is_err())                                                  \
      {                                                                     \
        using Err = ::mitama::failure_t<                                    \
            typename std::remove_cvref_t<decltype(result)>::err_type>;      \
        return ::std::get<Err>(                                             \
            std::forward<decltype(result)>(result).into_storage()           \
        );                                                                  \
      }                                                                     \
      using Ok = ::mitama::success_t<                                       \
          typename std::remove_cvref_t<decltype(result)>::ok_type>;         \
      ::std::get<Ok>(std::forward<decltype(result)>(result).into_storage()) \
          .get();                                                           \
    })
#  ifdef __clang__
#    define MITAMA_TRY(...)                                                \
      _Pragma("GCC diagnostic push")                                       \
          _Pragma("GCC diagnostic ignored \"-Wgnu-statement-expression\"") \
              MITAMA_TRY_IMPL(__VA_ARGS__) _Pragma("GCC diagnostic pop")
#  else
#    define MITAMA_TRY(...) MITAMA_TRY_IMPL(__VA_ARGS__)
#  endif
#else
#  define MITAMA_CPP_RESULT_TRY_MAY_NOT_PANIC false
#  define MITAMA_TRY(...) ::mitama::_result_detail::id(__VA_ARGS__).unwrap()
#endif
