#pragma once

#include "detail.hpp"
#include "../traits/impl_traits.hpp"

namespace mitama::result
{

// default injectors
template <class, class = std::nullptr_t>
struct ok_trait_injector
{
  constexpr void ok() const & = delete;
  constexpr void ok() && = delete;
};
template <class, class = std::nullptr_t>
struct err_trait_injector
{
  constexpr void err() const & = delete;
  constexpr void err() && = delete;
};
template <class, class = std::nullptr_t>
struct ok_err_trait_injector
{
};

// copy-move injector
template <class T, class E>
struct ok_trait_injector<Result<T, E>,
                         trait::where<
                             std::is_copy_constructible<T>>>
{
  /*!
   * \brief Converts from Result<T, E> to std::optional<T>.
   * 
   * Converts self into an std::optional<T>, copying self, and discarding the error, if any.
   */
  constexpr std::optional<T> ok() const &
  {
    if (static_cast<Result<T, E> const *>(this)->is_ok())
    {
      return std::optional<T>{std::get<Ok<T>>(static_cast<Result<T, E> const *>(this)->storage_).x};
    }
    else
    {
      return std::optional<T>{std::nullopt};
    }
  }

  /*!
   * \brief Converts from Result<T, E> to std::optional<T>.
   * 
   * Converts self into an std::optional<T>, comsuming self, and discarding the error, if any.
   */
  constexpr std::optional<T> ok() &&
  {
    if (static_cast<Result<T, E> *>(this)->is_ok())
    {
      return std::optional<T>{std::get<Ok<T>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x};
    }
    else
    {
      return std::optional<T>{std::nullopt};
    }
  }
};
template <class T, class E>
struct err_trait_injector<Result<T, E>,
                          trait::where<
                              std::is_copy_constructible<E>>>
{
  /*!
   * \brief Converts from Result<T, E> to std::optional<E>.
   * 
   * Converts self into an std::optional<E>, copying self, and discarding the success value, if any.
   */
  constexpr std::optional<E> err() const &
  {
    if (static_cast<Result<T, E> const *>(this)->is_err())
    {
      return std::optional<E>{std::get<Err<E>>(static_cast<Result<T, E> const *>(this)->storage_).x};
    }
    else
    {
      return std::optional<E>{std::nullopt};
    }
  }
  /*!
   * \brief Converts from Result<T, E> to std::optional<E>.
   * 
   * Converts self into an std::optional<E>, consuming self, and discarding the success value, if any.
   */
  constexpr std::optional<E> err() &&
  {
    if (static_cast<Result<T, E> *>(this)->is_err())
    {
      return std::optional<E>{std::get<Err<E>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x};
    }
    else
    {
      return std::optional<E>{std::nullopt};
    }
  }
};

template <class T, class E>
struct ok_err_trait_injector<Result<T, E>,
                             trait::where<
                                 std::is_copy_constructible<T>,
                                 std::is_copy_constructible<E>>>
{
  /*!
   * \brief Maps a Result<T, E> to Result<U, E> by applying a function to a contained Ok value, leaving an Err value untouched.
   * 
   * This function can be used to compose the results of two functions.
   */
  template <class O>
  constexpr auto map(O &&op) const & -> std::enable_if_t<std::is_invocable_v<O, T>,
                                                         Result<std::invoke_result_t<O, T>, E>>
  {
    using result_type = Result<std::invoke_result_t<O, T>, E>;
    return static_cast<Result<T, E> const *>(this)->is_ok()
               ? static_cast<result_type>(Ok{std::forward<O>(op)(std::get<Ok<T>>(static_cast<Result<T, E> const *>(this)->storage_).x)})
               : static_cast<result_type>(Err{std::get<Err<E>>(static_cast<Result<T, E> const *>(this)->storage_).x});
  }

  template <class O>
  constexpr auto map(O &&op) && -> std::enable_if_t<std::is_invocable_v<O, T>,
                                                    Result<std::invoke_result_t<O, T>, E>>
  {
    using result_type = Result<std::invoke_result_t<O, T>, E>;
    return static_cast<Result<T, E> *>(this)->is_ok()
               ? static_cast<result_type>(Ok{std::forward<O>(op)(std::get<Ok<T>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x)})
               : static_cast<result_type>(Err{std::get<Err<E>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x});
  }
  /*!
   * \brief Maps a Result<T, E> to Result<T, F> by applying a function to a contained Err value, leaving an Ok value untouched.
   * 
   * This function can be used to pass through a successful result while handling an error.
   */
  template <class O>
  constexpr auto map_err(O &&op) const & -> std::enable_if_t<std::is_invocable_v<O, E>,
                                                             Result<T, std::invoke_result_t<O, E>>>
  {
    using result_type = Result<T, std::invoke_result_t<O, E>>;
    return static_cast<Result<T, E> const *>(this)->is_err()
               ? static_cast<result_type>(Err{std::forward<O>(op)(std::get<Err<E>>(static_cast<Result<T, E> const *>(this)->storage_).x)})
               : static_cast<result_type>(Ok{std::get<Ok<T>>(static_cast<Result<T, E> const *>(this)->storage_).x});
  }
  template <class O>
  constexpr auto map_err(O &&op) && -> std::enable_if_t<std::is_invocable_v<O, E>,
                                                        Result<T, std::invoke_result_t<O, E>>>
  {
    using result_type = Result<T, std::invoke_result_t<O, E>>;
    return static_cast<Result<T, E> *>(this)->is_err()
               ? static_cast<result_type>(Err{std::forward<O>(op)(std::get<Err<E>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x)})
               : static_cast<result_type>(Ok{std::get<Ok<T>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x});
  }
  /*!
   * \brief Calls op if the result is Ok, otherwise returns the Err value of self.
   * 
   * This function can be used for control flow based on Result values.
   */
  template <class O>
  constexpr auto and_then(O &&op) const & -> std::enable_if_t<is_result_v<std::invoke_result_t<O, T>>,
                                                              std::invoke_result_t<O, T>>
  {
    using result_type = std::invoke_result_t<O, T>;
    return static_cast<Result<T, E> const *>(this)->is_ok()
               ? std::forward<O>(op)(std::get<Ok<T>>(static_cast<Result<T, E> const *>(this)->storage_).x)
               : static_cast<result_type>(Err{typename result_type::err_type(std::get<Err<E>>(static_cast<Result<T, E> const *>(this)->storage_).x)});
  }
  /*!
   * \brief Calls op if the result is Ok, otherwise returns the Err value of self.
   * 
   * This function can be used for control flow based on Result values.
   */
  template <class O>
  constexpr auto and_then(O &&op) && -> std::enable_if_t<is_result_v<std::invoke_result_t<O, T>>,
                                                         std::invoke_result_t<O, T>>
  {
    using result_type = std::invoke_result_t<O, T>;
    return static_cast<Result<T, E> *>(this)->is_ok()
               ? std::forward<O>(op)(std::get<Ok<T>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x)
               : static_cast<result_type>(Err{typename result_type::err_type(std::get<Err<E>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x)});
  }
  /*!
   * \brief Calls op if the result is Err, otherwise returns the Ok value of self.
   * 
   * This function can be used for control flow based on result values.
   */
  template <class O>
  constexpr auto or_else(O &&op) const & -> std::enable_if_t<is_result_v<std::invoke_result_t<O, E>>,
                                                             std::invoke_result_t<O, E>>
  {
    using result_type = std::invoke_result_t<O, E>;
    return static_cast<Result<T, E> const *>(this)->is_err()
               ? std::forward<O>(op)(std::get<Err<E>>(static_cast<Result<T, E> const *>(this)->storage_).x)
               : static_cast<result_type>(Ok{typename result_type::ok_type(std::get<Ok<T>>(static_cast<Result<T, E> const *>(this)->storage_).x)});
  }
  template <class O>
  constexpr auto or_else(O &&op) && -> std::enable_if_t<is_result_v<std::invoke_result_t<O, E>>,
                                                        std::invoke_result_t<O, E>>
  {
    using result_type = std::invoke_result_t<O, E>;
    return static_cast<Result<T, E> *>(this)->is_err()
               ? std::forward<O>(op)(std::get<Err<E>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x)
               : static_cast<result_type>(Ok{typename result_type::ok_type(std::get<Ok<T>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x)});
  }
};

// move injectors
template <class T, class E>
struct ok_trait_injector<Result<T, E>,
                   trait::where<
                       std::negation<std::is_copy_constructible<T>>,
                       std::is_move_constructible<T>>>
{
  constexpr std::optional<T> ok() const & = delete;
  /*!
   * \brief Converts from Result<T, E> to std::optional<T>.
   * 
   * Converts self into an std::optional<T>, copying self, and discarding the error, if any.
   */
  constexpr std::optional<T> ok() &&
  {
    if (static_cast<Result<T, E> *>(this)->is_ok())
    {
      return std::optional<T>{std::get<Ok<T>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x};
    }
    else
    {
      return std::optional<T>{std::nullopt};
    }
  }
};
template <class T, class E>
struct err_trait_injector<Result<T, E>,
                   trait::where<
                       std::negation<std::is_copy_constructible<E>>,
                       std::is_move_constructible<E>>>
{
  constexpr std::optional<E> err() const & = delete;
  /*!
   * \brief Converts from Result<T, E> to std::optional<E>.
   * 
   * Converts self into an std::optional<E>, consuming self, and discarding the success value, if any.
   */
  constexpr auto err() &&
  {
    if (static_cast<Result<T, E> *>(this)->is_err())
    {
      return std::optional<E>{std::get<Err<E>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x};
    }
    else
    {
      return std::optional<E>{std::nullopt};
    }
  }
};

template <class T, class E>
struct ok_err_trait_injector<Result<T, E>,
                       trait::where<
                           std::negation<std::is_copy_constructible<T>>,
                           std::is_move_constructible<T>>>
{
  template <class O>
  constexpr auto map(O &&op) const & -> Result<std::invoke_result_t<O, T>, E> = delete;

  template <class O>
  constexpr auto map(O &&op) && -> std::enable_if_t<std::is_invocable_v<O, T>,
                                                    Result<std::invoke_result_t<O, T>, E>>
  {
    using result_type = Result<std::invoke_result_t<O, T>, E>;
    return static_cast<Result<T, E> *>(this)->is_ok()
               ? static_cast<result_type>(Ok{std::forward<O>(op)(std::get<Ok<T>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x)})
               : static_cast<result_type>(Err{std::get<Err<E>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x});
  }

  template <class O>
  constexpr auto map_err(O &&op) const & -> Result<T, std::invoke_result_t<O, E>> = delete;

  template <class O>
  constexpr auto map_err(O &&op) const & -> std::enable_if_t<std::is_invocable_v<O, E>,
                                                             Result<T, std::invoke_result_t<O, E>>>
  {
    using result_type = Result<T, std::invoke_result_t<O, E>>;
    return static_cast<Result<T, E> *>(this)->is_err()
               ? static_cast<result_type>(Err{std::forward<O>(op)(std::get<Err<E>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x)})
               : static_cast<result_type>(Ok{std::get<Ok<T>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x});
  }
  /*!
   * \brief Calls op if the result is Ok, otherwise returns the Err value of self.
   * 
   * This function can be used for control flow based on Result values.
   */
  template <class O>
  constexpr auto and_then(O &&op) && -> std::enable_if_t<is_result_v<std::invoke_result_t<O, T>>,
                                                         std::invoke_result_t<O, T>>
  {
    using result_type = std::invoke_result_t<O, T>;
    return static_cast<Result<T, E> *>(this)->is_ok()
               ? std::forward<O>(op)(std::get<Ok<T>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x)
               : static_cast<result_type>(Err{typename result_type::err_type(std::get<Err<E>>(std::move(static_cast<Result<T, E> *>(this)->storage_)).x)});
  }
};

template <class Derived, class = std::nullptr_t>
struct unwrap_or_default_injector
{
  void unwrap_or_default() const & = delete;
};

template <class T, class E>
struct unwrap_or_default_injector<Result<T, E>,
                                  trait::where<std::is_default_constructible<T>>>
{
  T unwrap_or_default() const &
  {
    return static_cast<const Result<T, E> *>(this)->is_ok()
               ? static_cast<const Result<T, E> *>(this)->unwrap()
               : T{};
  }
};

template <class T>
struct unwrap_or_default_injector<Ok<T>,
                                  trait::where<std::is_default_constructible<T>>>
{
  T unwrap_or_default() const &
  {
    return static_cast<const Ok<T> *>(this)->x;
  }
};

template <class T>
struct impl
    : unwrap_or_default_injector<T>,
      ok_trait_injector<T>,
      err_trait_injector<T>,
      ok_err_trait_injector<T>
{
};

} // namespace rust_std::result