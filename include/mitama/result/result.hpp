#ifndef MITAMA_RESULT_HPP
#define MITAMA_RESULT_HPP
#include <mitama/result/detail/fwd.hpp>
#include <mitama/result/detail/meta.hpp>
#include <mitama/result/traits/perfect_traits_special_members.hpp>
#include <mitama/panic.hpp>
#include <mitama/result/factory/success.hpp>
#include <mitama/result/factory/failure.hpp>

#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/hana/functional/overload_linearly.hpp>
#include <boost/hana/functional/fix.hpp>
#include <boost/format.hpp>

#include <functional>
#include <optional>
#include <type_traits>
#include <utility>
#include <string_view>

namespace mitama::workaround {
  template < class T, class... Ts >
  bool holds_alternative(boost::variant<Ts...> const& var) {
    return boost::get<T>(&var) != nullptr;
  }
}

namespace boost {
template < class T, class U >
std::enable_if_t<std::conjunction_v<std::negation<std::is_same<T, U>>, mitama::is_comparable_with<T, U>>,
bool>
operator==(optional<T> const& lhs, optional<U> const& rhs) {
  if (!bool(lhs) || !bool(rhs)) {
    return false;
  }
  else {
    return lhs.value() == rhs.value();
  }
}
template < class T, class U >
std::enable_if_t<std::conjunction_v<std::negation<std::is_same<T, U>>, mitama::is_comparable_with<T, U>>,
bool>
operator!=(optional<T> const& lhs, optional<U> const& rhs) {
  if (!bool(lhs) || !bool(rhs)) {
    return true;
  }
  else {
    return !(lhs.value() == rhs.value());
  }
}

}

namespace mitama {

template <class>
struct is_result : std::false_type {};
template <mutability _mut, class T, class E>
struct is_result<basic_result<_mut, T, E>> : std::true_type {};
template <class T>
inline constexpr bool is_result_v = is_result<T>::value;

template <class, class...>
struct is_convertible_result_with : std::false_type {};
template <mutability _mut, class T, class E, class U>
struct is_convertible_result_with<basic_result<_mut, T, E>, success<U>>: std::is_constructible<U, T> {};
template <mutability _mut, class T, class E, class F>
struct is_convertible_result_with<basic_result<_mut, T, E>, failure<F>>: std::is_constructible<F, E> {};
template <mutability _mut, class T, class E, class U, class F>
struct is_convertible_result_with<basic_result<_mut, T, E>, success<U>, failure<F>>
  : std::conjunction<
      std::is_constructible<U, T>,
      std::is_constructible<F, E>>
{};

template <class T, class... Requires>
inline constexpr bool is_convertible_result_with_v = is_convertible_result_with<meta::remove_cvr_t<T>, Requires...>::value;

template <class>
struct is_err_type : std::false_type {};
template <class T>
struct is_err_type<failure<T>> : std::true_type {};
template <class>
struct is_ok_type : std::false_type {};
template <class T>
struct is_ok_type<success<T>> : std::true_type {};

} // !namespace mitama

#include <mitama/result/detail/result_impl.hpp>

namespace mitama {

/// Optional aliases (for migration)
inline auto none = boost::none;

template <class T>
inline boost::optional<T> some(T &&x) {
  return {std::forward<T>(x)};
}

template <class T, class... Args>
inline boost::optional<T> some(Args&&... args) {
  return boost::optional<T>{boost::in_place(std::forward<Args>(args)...)};
}

/// @brief class basic_result
/// @param _mutability: enum class value for mutability control
/// @param T: Type of successful value
/// @param E: Type of unsuccessful value
template <mutability _mutability, class T, class E>
class [[nodiscard]] basic_result<_mutability, T, E,
  /* bounded types requirements (see the document below) */
  /* https://www.boost.org/doc/libs/1_64_0/doc/html/variant/reference.html#variant.concepts */
  trait::where<
    std::disjunction<
      std::conjunction<
        std::is_copy_constructible<meta::remove_cvr_t<T>>, 
        std::is_copy_constructible<meta::remove_cvr_t<E>>
      >,
      std::conjunction<
        std::is_move_constructible<meta::remove_cvr_t<T>>, 
        std::is_move_constructible<meta::remove_cvr_t<E>>
      >
    >,
    std::is_nothrow_destructible<meta::remove_cvr_t<T>>,
    std::is_nothrow_destructible<meta::remove_cvr_t<E>>
  >>
  : /* method injection selectors */ 
  public unwrap_or_default_friend_injector<basic_result<_mutability, T, E>>,
  public transpose_friend_injector<basic_result<_mutability, T, E>>,
  public indirect_friend_injector<basic_result<_mutability, T, E>>
{
  /// result storage
  boost::variant<success<T>, failure<E>> storage_;
  /// friend accessors
  template <mutability, class, class, class>
  friend class basic_result;
  /// private aliases
  template <class... Requiers>
  using where = std::enable_if_t<std::conjunction_v<Requiers...>, std::nullptr_t>;
  static constexpr std::nullptr_t required = nullptr;
  template <mutability _mut, class T_, class E_>
  using not_self = std::negation<std::is_same<basic_result, basic_result<_mut, T_, E_>>>;
public:
  /// type fields
  using ok_type = T;
  using err_type = E;
  using ok_reference_type = std::remove_reference_t<T>&;
  using err_reference_type = std::remove_reference_t<E>&;
  using ok_const_reference_type = meta::remove_cvr_t<T> const&;
  using err_const_reference_type = meta::remove_cvr_t<E> const&;
  /// mutability
  static constexpr bool is_mut = !static_cast<bool>(_mutability);

  /* Constructors */

  /// default constructor is not permitted
  constexpr basic_result() noexcept = delete;

  /// @brief
  ///   explicit copy construcor for convertible basic_result
  template <mutability _mu, class U, class F,
            where<std::is_constructible<T, U>,
                  std::is_constructible<E, F>,
                  std::disjunction<
                    std::negation<std::is_convertible<F, E>>,
                    std::negation<std::is_convertible<U, T>>
                  >
            > = required>
  explicit constexpr basic_result(basic_result<_mu, U, F> const& res)
    : storage_(res.storage_)
  {}

  /// @brief
  ///   non-explicit copy construcor for convertible basic_result
  template <mutability _mu, class U, class F,
            where<std::is_constructible<T, U>,
                  std::is_constructible<E, F>,
                  std::is_convertible<U, T>,
                  std::is_convertible<F, E>
            > = required>
  constexpr basic_result(basic_result<_mu, U, F> const& res)
    : storage_(res.storage_)
  {}

  /// @brief
  ///   explicit move construcor for convertible basic_result
  template <mutability _mu, class U, class F,
            where<std::is_constructible<T, U>,
                  std::is_constructible<E, F>,
                  std::disjunction<
                    std::negation<std::is_convertible<F, E>>,
                    std::negation<std::is_convertible<U, T>>
                  >
            > = required>
  explicit constexpr basic_result(basic_result<_mu, U, F>&& res)
    : storage_(std::move(res.storage_))
  {}

  /// @brief
  ///   non-explicit copy construcor for convertible basic_result
  template <mutability _mu, class U, class F,
            where<std::is_constructible<T, U>,
                  std::is_constructible<E, F>,
                  std::is_convertible<U, T>,
                  std::is_convertible<F, E>
            > = required>
  constexpr basic_result(basic_result<_mu, U, F>&& res)
    : storage_(std::move(res.storage_))
  {}

  /// @brief
  ///   copy assignment operator for convertible basic_result
  template <mutability _mu, class U, class F,
            where<std::is_constructible<T, U>,
                  std::is_constructible<E, F>
            > = required>
  constexpr basic_result& operator=(basic_result<_mu, U, F> const& res)
  {
    static_assert(is_mut_v<_mutability>, "Error: assignment to immutable result");
    if (res.is_ok())
      { this->storage_ = success<T>(res.unwrap()); }
    else
      { this->storage_ = failure<E>(res.unwrap_err()); }
    return *this;
  }

  /// @brief
  ///   move assignment operator for convertible basic_result
  template <mutability _mu, class U, class F,
            where<std::is_constructible<T, U>,
                  std::is_constructible<E, F>
            > = required>
  constexpr basic_result& operator=(basic_result<_mu, U, F>&& res)
  {
    static_assert(is_mut_v<_mutability>, "Error: assignment to immutable result");
    if (res.is_ok())
      { this->storage_ = success<T>(res.unwrap()); }
    else
      { this->storage_ = failure<E>(res.unwrap_err()); }
    return *this;
  }

  /// @brief
  ///   copy assignment operator for convertible success
  template <class U,
            where<std::is_constructible<T, U>> = required>
  constexpr basic_result& operator=(success<U> const& _ok)
  {
    static_assert(is_mut_v<_mutability>, "Error: assignment to immutable result");
    this->storage_ = success<T>(_ok.x);
    return *this;
  }

  /// @brief
  ///   copy assignment operator for convertible failure
  template <class F,
            where<std::is_constructible<E, F>> = required>
  constexpr basic_result& operator=(failure<F> const& _err)
  {
    static_assert(is_mut_v<_mutability>, "Error: assignment to immutable result");
    this->storage_ = failure<E>(_err.x);
    return *this;
  }

  /// @brief
  ///   move assignment operator for convertible success
  template <class U,
            where<std::is_constructible<T, U>> = required>
  constexpr basic_result& operator=(success<U>&& _ok)
  {
    static_assert(is_mut_v<_mutability>, "Error: assignment to immutable result");
    this->storage_ = success<T>(std::move(_ok.x));
    return *this;
  }

  /// @brief
  ///   move assignment operator for convertible failure
  template <class F,
            where<std::is_constructible<E, F>> = required>
  constexpr basic_result& operator=(failure<F>&& _err)
  {
    static_assert(is_mut_v<_mutability>, "Error: assignment to immutable result");
    this->storage_ = failure<E>(std::move(_err.x));
    return *this;
  }

  /// @brief
  ///   non-explicit constructor for successful lvalue
  template <class U,
            where<std::is_constructible<T, U>,
                  std::is_convertible<U, T>> = required>
  constexpr basic_result(success<U> const &ok)
    : storage_{ok}
  {}

  /// @brief
  ///   explicit constructor for successful lvalue
  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  constexpr explicit basic_result(success<U> const &ok)
    : storage_{ok}
  {}

  /// @brief
  ///   non-explicit constructor for successful rvalue
  template <class U,
            where<std::is_constructible<T, U>,
                  std::is_convertible<U, T>> = required>
  constexpr basic_result(success<U> && ok)
    : storage_{std::move(ok)}
  {}

  /// @brief
  ///   explicit constructor for successful rvalue
  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  constexpr explicit basic_result(success<U> && ok)
    : storage_{std::move(ok)}
  {}

  /// @brief
  ///   non-explicit constructor for unsuccessful lvalue
  template <class U,
            where<std::is_constructible<E, U>,
                  std::is_convertible<U, E>> = required>
  constexpr basic_result(failure<U> const &err)
    : storage_{err}
  {}

  /// @brief
  ///   explicit constructor for unsuccessful lvalue
  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  constexpr explicit basic_result(failure<U> const &err)
    : storage_{err}
  {}

  /// @brief
  ///   non-explicit constructor for unsuccessful rvalue
  template <class U,
            where<std::is_constructible<E, U>,
                  std::is_convertible<U, E>> = required>
  constexpr basic_result(failure<U> && err)
    : storage_{err}
  {}

  /// @brief
  ///   explicit constructor for unsuccessful lvalue
  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  constexpr explicit basic_result(failure<U> && err)
    : storage_{err}
  {}

  constexpr basic_result(success<> ok)
    : storage_{ok}
  {}

  constexpr basic_result(failure<> err)
    : storage_{err}
  {}

  /// @brief
  ///   in-place constructor for successful result
  template <class... Args,
            where<std::is_constructible<T, Args&&...>> = required>
  constexpr explicit basic_result(in_place_ok_t, Args && ... args)
    : storage_{success<T>{std::in_place, std::forward<Args>(args)...}}
  {}

  /// @brief
  ///   in-place constructor for unsuccessful result
  template <class... Args,
            where<std::is_constructible<E, Args&&...>> = required>
  constexpr explicit basic_result(in_place_err_t, Args && ... args)
    : storage_{failure<E>{std::in_place, std::forward<Args>(args)...}}
  {}

  /// @brief
  ///   in-place constructor with initializer_list for successful result
  template <class U, class... Args,
            where<std::is_constructible<T, std::initializer_list<U>, Args&&...>> = required>
  constexpr explicit basic_result(in_place_ok_t, std::initializer_list<U> il, Args && ... args)
    : storage_{success<T>{std::in_place, il, std::forward<Args>(args)...}}
  {}

  /// @brief
  ///   in-place constructor with initializer_list for unsuccessful result
  template <class U, class... Args,
            where<std::is_constructible<E, Args&&...>> = required>
  constexpr explicit basic_result(in_place_err_t, std::initializer_list<U> il, Args && ... args)
    : storage_{failure<E>{std::in_place, il, std::forward<Args>(args)...}}
  {}

  /// @brief
  ///   Checks if self has a success value.
  ///
  /// @note
  ///   Returns true if the result is succsess.
  constexpr bool is_ok() const noexcept { return ::mitama::workaround::holds_alternative<success<T>>(storage_); }

  /// @brief
  ///   Checks if self has a failure value.
  ///
  /// @note
  ///   Returns true if the result is failure.
  constexpr bool is_err() const noexcept { return ::mitama::workaround::holds_alternative<failure<E>>(storage_); }

  /// @brief
  ///   Converts from basic_result to bool.
  ///
  /// @note
  ///   Covert result to bool and returns true if the result is succsess.
  explicit constexpr operator bool() const noexcept { return ::mitama::workaround::holds_alternative<success<T>>(storage_); }

  /// @brief
  ///   Converts from basic_result to bool.
  ///
  /// @note
  ///   Covert result to bool and returns true if the result is failure.
  constexpr bool operator !() const noexcept { return ::mitama::workaround::holds_alternative<failure<E>>(storage_); }

  /// @brief
  ///   Converts from basic_result to `boost::optional<const T>`.
  ///
  /// @note
  ///   Converts self into a `boost::optional<const T>`, and discarding the failure, if any.
  constexpr
  maybe<std::remove_reference_t<ok_type>>
  ok() const & noexcept {
    if (is_ok()) {
      return maybe<std::remove_reference_t<ok_type>>(std::in_place, unwrap());
    }
    else {
      return nothing;
    }
  }

  /// @brief
  ///   Converts from basic_result to `boost::optional`.
  ///
  /// @note
  ///   Converts self into a `boost::optional<const E>`, and discarding the success, if any.
  constexpr
  maybe<std::remove_reference_t<err_type>>
  err() const & noexcept {
    if (is_err()) {
      return maybe<std::remove_reference_t<err_type>>(std::in_place, unwrap_err());
    }
    else {
      return nothing;
    }
  }

  /// @brief
  ///   Produces a new basic_result, containing a reference into the original, leaving the original in place.
  constexpr auto as_ref() const&
    noexcept
    -> basic_result<_mutability, meta::remove_cvr_t<T> const&, meta::remove_cvr_t<E> const&>
  {
    if ( is_ok() )
      return basic_result<_mutability, meta::remove_cvr_t<T> const&, meta::remove_cvr_t<E> const&>{in_place_ok, boost::get<success<T>>(storage_).x};
    else
      return basic_result<_mutability, meta::remove_cvr_t<T> const&, meta::remove_cvr_t<E> const&>{in_place_err, boost::get<failure<E>>(storage_).x};
  }

  /// @brief
  ///   Converts from `basic_result<mutability::mut, T, E>&` to `basic_result<mutability::immut, T&, E&>`.
  constexpr
  auto as_mut() &
    noexcept
    -> basic_result<mutability::immut, std::remove_reference_t<T>&, std::remove_reference_t<E>&>
  {
    static_assert(
      !std::is_const_v<std::remove_reference_t<T>>,
      "Error: ok_type is immutable");
    static_assert(
      !std::is_const_v<std::remove_reference_t<E>>,
      "Error: err_type is immutable");
    static_assert(
      is_mut_v<_mutability>,
      "Error: result is immutable");

    if ( is_ok() )
      return basic_result<mutability::immut, std::remove_reference_t<T>&, std::remove_reference_t<E>&>{in_place_ok, boost::get<success<T>>(storage_).x};
    else
      return basic_result<mutability::immut, std::remove_reference_t<T>&, std::remove_reference_t<E>&>{in_place_err, boost::get<failure<E>>(storage_).x};
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<U, E> by applying a function to a contained success value,
  ///   leaving an failure value untouched.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap()) }
  ///
  /// @note
  ///   This function can be used to compose the results of two functions.
  template <class O>
  constexpr auto map(O && op) const &
    noexcept(std::is_nothrow_invocable_v<O, T>)
    -> std::enable_if_t<std::is_invocable_v<O, T>,
    basic_result<_mutability, std::invoke_result_t<O, T>, E>>
  {
    using result_type = basic_result<_mutability, std::invoke_result_t<O, T>, E>;
    return is_ok()
               ? static_cast<result_type>(success{std::invoke(std::forward<O>(op), boost::get<success<T>>(storage_).x)})
               : static_cast<result_type>(failure{boost::get<failure<E>>(storage_).x});
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<U, E> by applying a function to a contained success value,
  ///   leaving an failure value untouched.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap()) }
  ///
  /// @note
  ///   This function can be used to compose the results of two functions.
  template <class O>
  constexpr auto map(O && op) &&
    noexcept(std::is_nothrow_invocable_v<O, T>)
    -> std::enable_if_t<std::is_invocable_v<O, T>,
    basic_result<_mutability, std::invoke_result_t<O, T>, E>>
  {
    using result_type = basic_result<_mutability, std::invoke_result_t<O, T>, E>;
    return is_ok()
               ? static_cast<result_type>(success{std::invoke(std::forward<O>(op), std::move(boost::get<success<T>>(storage_).x))})
               : static_cast<result_type>(failure{std::move(boost::get<failure<E>>(storage_).x)});
  }

  /// @brief
  ///   Maps a basic_result<T, E> to U by applying a function to a contained success value,
  ///   or a fallback function to a contained failure value.
  ///
  /// @requires
  ///   { std::invoke(_fallback, unwrap_err()) };
  ///   { std::invoke(_map, unwrap()) };
  ///   Common< decltype(std::invoke(_fallback, unwrap_err())), decltype(std::invoke(_map, unwrap())) >;
  ///
  /// @note
  ///   This function can be used to unpack a successful result while handling an error.
  template <class Map, class Fallback>
  constexpr auto map_or_else(Fallback&& _fallback, Map&& _map) const&
    noexcept(std::is_nothrow_invocable_v<Fallback, E> && std::is_nothrow_invocable_v<Map, T>)
    -> std::enable_if_t<
          std::conjunction_v<std::is_invocable<Map, T>,
            std::is_invocable<Fallback, E>,
            std::is_convertible<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>,
            std::is_convertible<std::invoke_result_t<Fallback, E>, std::invoke_result_t<Map, T>>
          >,
    std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>>
  {
    using result_type = std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>;
    return is_ok()
               ? static_cast<result_type>(std::invoke(std::forward<Map>(_map), boost::get<success<T>>(storage_).x))
               : static_cast<result_type>(std::invoke(std::forward<Fallback>(_fallback), boost::get<failure<E>>(storage_).x));
  }

  /// @brief
  ///   Maps a basic_result<T, E> to U by applying a function to a contained success value,
  ///   or a fallback function to a contained failure value.
  ///
  /// @requires
  ///   { std::invoke(_fallback, unwrap_err()) };
  ///   { std::invoke(_map, unwrap()) };
  ///   Common< decltype(std::invoke(_fallback, unwrap_err())), decltype(std::invoke(_map, unwrap())) >;
  ///
  /// @note
  ///   This function can be used to unpack a successful result while handling an error.
  template <class Map, class Fallback>
  constexpr auto map_or_else(Fallback&& _fallback, Map&& _map) && 
    noexcept(std::is_nothrow_invocable_v<Fallback, E> && std::is_nothrow_invocable_v<Map, T>)
    -> std::enable_if_t<
          std::conjunction_v<std::is_invocable<Map, T>,
            std::is_invocable<Fallback, E>,
            std::is_convertible<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>,
            std::is_convertible<std::invoke_result_t<Fallback, E>, std::invoke_result_t<Map, T>>
          >,
    std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>>
  {
    using result_type = std::common_type_t<std::invoke_result_t<Map, T>, std::invoke_result_t<Fallback, E>>;
    return is_ok()
               ? static_cast<result_type>(std::invoke(std::forward<Map>(_map), std::move(boost::get<success<T>>(storage_).x)))
               : static_cast<result_type>(std::invoke(std::forward<Fallback>(_fallback), std::move(boost::get<failure<E>>(storage_).x)));
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<T, F> by applying a function to a contained failure value,
  ///   leaving an success value untouched.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap_err()) }
  ///
  /// @note
  ///   This function can be used to pass through a successful result while handling an error.
  template <class O>
  constexpr auto map_err(O && op) const &
    noexcept(std::is_nothrow_invocable_v<O, E>)
    -> std::enable_if_t<std::is_invocable_v<O, E>,
    basic_result<_mutability, T, std::invoke_result_t<O, E>>>
  {
    using result_type = basic_result<_mutability, T, std::invoke_result_t<O, E>>;
    return is_err()
               ? static_cast<result_type>(failure{std::invoke(std::forward<O>(op), boost::get<failure<E>>(storage_).x)})
               : static_cast<result_type>(success{boost::get<success<T>>(storage_).x});
  }

  /// @brief
  ///   Maps a basic_result<T, E> to basic_result<T, F> by applying a function to a contained failure value,
  ///   leaving an success value untouched.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap_err()) }
  ///
  /// @note
  ///   This function can be used to pass through a successful result while handling an error.
  template <class O>
  constexpr auto map_err(O && op) &&
    noexcept(std::is_nothrow_invocable_v<O, E>)
    -> std::enable_if_t<std::is_invocable_v<O, E>,
    basic_result<_mutability, T, std::invoke_result_t<O, E>>>
  {
    using result_type = basic_result<_mutability, T, std::invoke_result_t<O, E>>;
    return is_err()
               ? static_cast<result_type>(failure{std::invoke(std::forward<O>(op), std::move(boost::get<failure<E>>(storage_).x))})
               : static_cast<result_type>(success{std::move(boost::get<success<T>>(storage_).x)});
  }

  /// @brief
  ///   Calls `op` if the result is success, otherwise; returns the failure value of self.
  ///
  /// @requires
  ///   requires requires is_convertible_result_with<std::invoke_result_t<O, T>, failure<F>>
  ///
  /// @note
  ///   This function can be used for control flow based on result values.
  template <class O>
  constexpr auto and_then(O && op) const &
    noexcept(std::is_nothrow_invocable_v<O, T>)
    -> std::enable_if_t<is_convertible_result_with_v<std::invoke_result_t<O, T>, failure<E>>,
    std::invoke_result_t<O, T>>
  {
    using result_type = std::invoke_result_t<O, T>;
    return is_ok()
               ? std::invoke(std::forward<O>(op), boost::get<success<T>>(storage_).x)
               : static_cast<result_type>(failure{boost::get<failure<E>>(storage_).x});
  }

  /// @brief
  ///   Calls `op` if the result is success, otherwise; returns the failure value of self.
  ///
  /// @requires
  ///   requires requires is_convertible_result_with<std::invoke_result_t<O, T>, success<T>>
  ///
  /// @note
  ///   This function can be used for control flow based on result values.
  template <class O>
  constexpr auto and_then(O && op) &&
    noexcept(std::is_nothrow_invocable_v<O, T>)
    -> std::enable_if_t<is_convertible_result_with_v<std::invoke_result_t<O, T>, failure<E>>,
    std::invoke_result_t<O, T>>
  {
    using result_type = std::invoke_result_t<O, T>;
    return is_ok()
               ? std::invoke(std::forward<O>(op), std::move(boost::get<success<T>>(storage_).x))
               : static_cast<result_type>(failure{std::move(boost::get<failure<E>>(storage_).x)});
  }

  /// @brief
  ///   Calls `op` if the result is failure, otherwise; returns the success value of self.
  ///
  /// @requires
  ///   requires requires is_convertible_result_with<std::invoke_result_t<O, T>, success<T>>
  ///
  /// @note
  ///   This function can be used for control flow based on result values.
  template <class O>
  constexpr auto or_else(O && op) const &
    noexcept(std::is_nothrow_invocable_v<O, E>)
    -> std::enable_if_t<is_convertible_result_with_v<std::invoke_result_t<O, T>, success<T>>,
    std::invoke_result_t<O, E>>
  {
    using result_type = std::invoke_result_t<O, E>;
    return is_err()
               ? std::invoke(std::forward<O>(op), boost::get<failure<E>>(storage_).x)
               : static_cast<result_type>(success{boost::get<success<T>>(storage_).x});
  }

  /// @brief
  ///   Calls `op` if the result is failure, otherwise; returns the success value of self.
  ///
  /// @requires
  ///   { std::invoke(op, unwrap_err()) } -> ConvertibleTo<basic_result<T, U>>
  ///
  /// @note
  ///   This function can be used for control flow based on result values.
  template <class O>
  constexpr auto or_else(O && op) &&
    noexcept(std::is_nothrow_invocable_v<O, E>)
    -> std::enable_if_t<is_convertible_result_with_v<std::invoke_result_t<O, T>, success<T>>,
    std::invoke_result_t<O, E>>
  {
    using result_type = std::invoke_result_t<O, E>;
    return is_err()
               ? std::invoke(std::forward<O>(op), boost::get<failure<E>>(std::move(storage_)).x)
               : static_cast<result_type>(success{boost::get<success<T>>(std::move(storage_)).x});
  }

  /// @brief
  ///   Returns `res` if the result is success, otherwise; returns the failure value of self.
  template <mutability _mu, class U>
  constexpr decltype(auto) operator&&(basic_result<_mu, U, E> const &res) const & noexcept
  {
    using result_type = basic_result<_mutability && _mu, U, E>;
    return this->is_err()
               ? static_cast<result_type>(failure{boost::get<failure<E>>(storage_).x})
               : res.is_err() ? static_cast<result_type>(failure{res.unwrap_err()})
                              : static_cast<result_type>(success{res.unwrap()});
  }

  /// @brief
  ///   Returns res if the result is failure, otherwise returns the success value of self.
  ///
  /// @note
  ///   Arguments passed to or are eagerly evaluated;
  ///   if you are passing the result of a function call,
  ///   it is recommended to use `or_else`,
  ///   which is lazily evaluated.
  template <mutability _mut, class F>
  constexpr decltype(auto) operator||(basic_result<_mut, T, F> const &res) const & noexcept
  {
    using result_type = basic_result<_mutability, T, F>;
    return this->is_ok()
               ? static_cast<result_type>(success{boost::get<success<T>>(storage_).x})
               : res.is_ok() ? static_cast<result_type>(success{res.unwrap()})
                             : static_cast<result_type>(failure{res.unwrap_err()});
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
  template <class U,
            where<meta::has_common_type<T, U&&>> = required>
  decltype(auto) unwrap_or(U&& optb) const& noexcept
  {
    return is_ok() ? boost::get<success<T>>(storage_).x
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
  template <class U,
            where<meta::has_common_type<std::remove_reference_t<T>&&, U&&>> = required>
  decltype(auto) unwrap_or(U&& optb) && noexcept {
    return is_ok() ? std::move(boost::get<success<T>>(storage_).x)
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
  ///     - `std::is_invocable_r_v<T, O, E>` is true then; it invoke `op` with its value or,
  ///     - `std::is_invocable_r_v<T, O>` is true then; it invoke `op` without value,
  ///     - otherwise; static_assert.
  template <class O>
  std::enable_if_t<
    std::disjunction_v<
      std::is_invocable_r<T, O, E>,
      std::is_invocable_r<T, O>>,
  T>
  unwrap_or_else(O && op) const
    noexcept(
      std::disjunction_v<
        std::conjunction<
          std::is_invocable_r<T, O, E>,
          std::is_nothrow_invocable_r<T, O, E>
        >,
        std::conjunction<
          std::is_invocable_r<T, O, E>,
          std::is_nothrow_invocable_r<T, O, E>
        >
      >
    )
  {
    if constexpr (std::is_invocable_r_v<T, O, E>) {
      return is_ok() ? boost::get<success<T>>(storage_).x : std::invoke(std::forward<O>(op), boost::get<failure<E>>(storage_).x);
    }
    else if constexpr (std::is_invocable_r_v<T, O>) {
      return is_ok() ? boost::get<success<T>>(storage_).x : std::invoke(std::forward<O>(op));
    }
    else {
      static_assert([]{ return false; }(), "invalid argument: designated function object is not invocable");
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @panics
  ///   Panics if the value is an failure, with a panic message provided by the failure's value.
  force_add_const_t<T>&
  unwrap() const& {
    if constexpr (trait::formattable_element<E>::value) {
      if ( is_ok() ) {
        return boost::get<success<T>>(storage_).x;
      }
      else {
        PANIC("called `basic_result::unwrap()` on a value: `failure(%1%)`", unwrap_err());
      }      
    }
    else {
      if ( is_ok() ) {
        return boost::get<success<T>>(storage_).x;
      }
      else {
        PANIC("called `basic_result::unwrap()` on a value `failure(?)`");
      }
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @panics
  ///   Panics if the value is an failure, with a panic message provided by the failure's value.
  std::conditional_t<is_mut_v<_mutability>, T&, force_add_const_t<T>&>
  unwrap() & {
    if constexpr (trait::formattable_element<E>::value) {
      if ( is_ok() ) {
        return boost::get<success<T>>(storage_).x;
      }
      else {
        PANIC("called `basic_result::unwrap()` on a value: `failure(%1%)`", unwrap_err());
      }      
    }
    else {
      if ( is_ok() ) {
        return boost::get<success<T>>(storage_).x;
      }
      else {
        PANIC("called `basic_result::unwrap()` on a value `failure(?)`");
      }
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an failure.
  ///
  /// @panics
  ///   Panics if the value is an success, with a panic message provided by the success's value.
  force_add_const_t<E>&
  unwrap_err() const& {
    if constexpr (trait::formattable_element<T>::value) {
      if ( is_err() ) {
        return boost::get<failure<E>>(storage_).x;
      }
      else {
        PANIC("called `basic_result::unwrap_err()` on a value: success(%1%)", unwrap());
      }
    }
    else {
      if ( is_err() ) {
        return boost::get<failure<E>>(storage_).x;
      }
      else {
        PANIC("called `basic_result::unwrap_err()` on a value `success(?)`");
      }
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an failure.
  ///
  /// @panics
  ///   Panics if the value is an success, with a panic message provided by the success's value.
  std::conditional_t<is_mut_v<_mutability>, E&, force_add_const_t<E>&>
  unwrap_err() & {
    if constexpr (trait::formattable_element<T>::value) {
      if ( is_err() ) {
        return boost::get<failure<E>>(storage_).x;
      }
      else {
        PANIC("called `basic_result::unwrap_err()` on a value: success(%1%)", unwrap());
      }
    }
    else {
      if ( is_err() ) {
        return boost::get<failure<E>>(storage_).x;
      }
      else {
        PANIC("called `basic_result::unwrap_err()` on a value `success(?)`)");
      }
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @panics
  ///   Panics if the value is an failure, with a panic message including the passed message, and the content of the failure.
  force_add_const_t<T>&
  expect(std::string_view msg) const& {
    if ( is_err() )
      PANIC("%1%: %2%", msg, unwrap_err());
    else
      return unwrap();
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @panics
  ///   Panics if the value is an failure, with a panic message including the passed message, and the content of the failure.
  decltype(auto)
  expect(std::string_view msg) & {
    if ( is_err() )
      PANIC("%1%: %2%", msg, unwrap_err());
    else
      return unwrap();
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an failure.
  ///
  /// @panics
  ///   Panics if the value is an success, with a panic message including the passed message, and the content of the success.
  force_add_const_t<E>&
  expect_err(std::string_view msg) const& {
    if ( is_ok() )
      PANIC("%1%: %2%", msg, unwrap());
    else
      return unwrap_err();
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an failure.
  ///
  /// @panics
  ///   Panics if the value is an success, with a panic message including the passed message, and the content of the success.
  decltype(auto)
  expect_err(std::string_view msg) & {
    if ( is_ok() )
      PANIC("%1%: %2%", msg, unwrap());
    else
      return unwrap_err();
  }

  template <class F>
  std::enable_if_t<std::is_invocable_v<F&&, T>>
  and_finally(F&& f) const& {
    if (this->is_ok())
      std::invoke(std::forward<F>(f), unwrap());
  }

  template <class F>
  std::enable_if_t<std::is_invocable_v<F&&, E>>
  or_finally(F&& f) const& {
    if (this->is_err())
      std::invoke(std::forward<F>(f), unwrap_err());
  }

  /// @brief
  ///   equal compare
  ///
  /// @requires
  ///   (T a, U b) { a == b } -> Same<bool>;
  ///   (E a, F b) { a == b } -> Same<bool>
  ///
  /// @note
  ///   This method tests for self and other values to be equal, and is used by `==` found by ADL.
  template <mutability _mut, class U, class F>
  std::enable_if_t<
    std::conjunction_v<is_comparable_with<T, U>, is_comparable_with<E, F>>,
  bool>
  operator==(basic_result<_mut, U, F> const &rhs) const & {
    return boost::apply_visitor(
      boost::hana::overload(
        [](success<T> const& l, success<U> const& r) { return l.x == r.x; },
        [](failure<E> const& l, failure<F> const& r) { return l.x == r.x; },
        [](auto&&...) { return false; }),
      this->storage_, rhs.storage_);
  }

  /// @brief
  ///   not equal compare
  ///
  /// @requires
  ///   (T a, U b) { a == b } -> Same<bool>;
  ///   (E a, F b) { a == b } -> Same<bool>
  ///
  /// @note
  ///   This method tests for self and other values to be not equal, and is used by `==` found by ADL.
  template <mutability _mut, class U, class F>
  std::enable_if_t<
    std::conjunction_v<is_comparable_with<T, U>, is_comparable_with<E, F>>,
  bool>
  operator!=(basic_result<_mut, U, F> const &rhs) const & {
    return boost::apply_visitor(
      boost::hana::overload(
        [](success<T> const& l, success<U> const& r) { return !(l.x == r.x); },
        [](failure<E> const& l, failure<F> const& r) { return !(l.x == r.x); },
        [](auto&&...) { return true; }),
      this->storage_, rhs.storage_);
  }

  /// @brief
  ///   equal compare
  ///
  /// @requires
  ///   (T a, U b) { a == b } -> Same<bool>
  ///
  /// @note
  ///   This method tests for self and other values to be equal, and is used by `==` found by ADL.
  template <class U>
  std::enable_if_t<
    is_comparable_with<T, U>::value,
  bool>
  operator==(success<U> const &rhs) const {
    return this->is_ok() ? this->unwrap() == rhs.x : false;
  }

  /// @brief
  ///   not equal compare
  ///
  /// @requires
  ///   (T a, U b) { a == b } -> Same<bool>
  ///
  /// @note
  ///   This method tests for self and other values to be not equal, and is used by `==` found by ADL.
  template <class U>
  std::enable_if_t<
    is_comparable_with<T, U>::value,
  bool>
  operator!=(success<U> const &rhs) const {
    return this->is_ok() ? !(this->unwrap() == rhs.x) : true;
  }

  /// @brief
  ///   equal compare
  ///
  /// @requires
  ///   (E a, F b) { a == b } -> Same<bool>
  ///
  /// @note
  ///   This method tests for self and other values to be equal, and is used by `==` found by ADL.
  template <class F>
  std::enable_if_t<
    is_comparable_with<E, F>::value,
  bool>
  operator==(failure<F> const &rhs) const {
    return this->is_err() ? this->unwrap_err() == rhs.x : false;
  }

  /// @brief
  ///   not equal compare
  ///
  /// @requires
  ///   (E a, F b) { a == b } -> Same<bool>
  ///
  /// @note
  ///   This method tests for self and other values to be equal, and is used by `==` found by ADL.
  template <class F>
  std::enable_if_t<
    is_comparable_with<E, F>::value,
  bool>
  operator!=(failure<F> const &rhs) const {
    return this->is_err() ? !(this->unwrap_err() == rhs.x) : true;
  }

  template <mutability _, class U, class F>
  bool operator<(basic_result<_, U, F> const& rhs) const {
    return boost::apply_visitor(
      boost::hana::overload(
        [](success<T> const& l, success<U> const& r) { return l.x < r.x; },
        [](failure<E> const& l, failure<F> const& r) { return l.x < r.x; },
        [](failure<E> const&, success<U> const&) { return true; },
        [](success<T> const&, failure<F> const&) { return false; }),
      this->storage_, rhs.storage_);
  }

  template <mutability _, class U, class F>
  bool operator>(basic_result<_, U, F> const& rhs) const {
    return boost::apply_visitor(
      boost::hana::overload(
        [](success<T> const& l, success<U> const& r) { return r.x < l.x; },
        [](failure<E> const& l, failure<F> const& r) { return r.x < l.x; },
        [](failure<E> const&, success<U> const&) { return false; },
        [](success<T> const&, failure<F> const&) { return true; }),
      this->storage_, rhs.storage_);
  }

  template <mutability _, class U, class F>
  bool operator<=(basic_result<_, U, F> const& rhs) const {
    return boost::apply_visitor(
      boost::hana::overload(
        [](success<T> const& l, success<U> const& r) { return l.x <= r.x; },
        [](failure<E> const& l, failure<F> const& r) { return l.x <= r.x; },
        [](failure<E> const&, success<U> const&) { return true; },
        [](success<T> const&, failure<F> const&) { return false; }),
      this->storage_, rhs.storage_);
  }

  template <mutability _, class U, class F>
  bool operator>=(basic_result<_, U, F> const& rhs) const {
    return boost::apply_visitor(
      boost::hana::overload(
        [](success<T> const& l, success<U> const& r) { return r.x <= l.x; },
        [](failure<E> const& l, failure<F> const& r) { return r.x <= l.x; },
        [](failure<E> const&, success<U> const&) { return false; },
        [](success<T> const&, failure<F> const&) { return true; }),
      this->storage_, rhs.storage_);
  }

};

} // namespace mitama

#endif
