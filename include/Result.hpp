/**
 * @file Result.hpp
 * @brief emulation Rust std::result::Result
 * @author いなむのみたま
 */

#pragma once
#include <boost/format.hpp>
#include <functional>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include "traits/perfect_traits_special_members.hpp"
#include "result/detail.hpp"
#include "result/fwd.hpp"

/*!
 * \brief macro for expand throw expression that embedded __FILE__ and __LINE__.
 *
 * Empty, string(const char[N]) or format string and formattable args are acceptable macro arguments.
 * If args include a value not formattable, raise an exception.
 */
#define PANIC(...) \
  throw ::mitama::runtime_panic { macro_use, __FILE__, __LINE__, __VA_ARGS__ }

class macro_use_tag_t{};
inline static constexpr macro_use_tag_t macro_use{};

namespace mitama
{
/*!
 * \brief exceotion class.
 */
class runtime_panic : public std::runtime_error
{
public:
  /*!
   * \brief constructor with format.
   *
   *  constructor for format string and arguments.
   */
  template <class... Args>
  runtime_panic(boost::format fmt, Args &&... args) noexcept
      : std::runtime_error((fmt % ... % args).str()) {}

  /*!
   * \brief This constructor is designated for PANIC macro.
   *
   *  Don't use directly.
   */
  template <class... Args>
  explicit runtime_panic(macro_use_tag_t, const char *func, int line, std::string fmt, Args &&... args) noexcept
      : std::runtime_error(
            std::string{"runtime panicked at '"} + (boost::format(fmt) % ... % args).str() +
            (boost::format("', %1%:%2%") % std::string{func} % line).str()) {}
};

//! @cond
template <class>
struct is_result : std::false_type
{
};
template <class T, class E>
struct is_result<Result<T, E>> : std::true_type
{
};
template <class T>
inline constexpr bool is_result_v = is_result<T>::value;

template <class>
struct is_err_type : std::false_type
{
};
template <class T>
struct is_err_type<Err<T>> : std::true_type
{
};
template <class>
struct is_ok_type : std::false_type
{
};
template <class T>
struct is_ok_type<Ok<T>> : std::true_type
{
};
} // namespace mitama

#include "result/result_impl.hpp"
//! @endcond

namespace mitama
{
/** @defgroup Result
 *  
 *  @{
 */

//!  Ok stat generator class.
template <class T>
class[[nodiscard]] Ok
    : private mitamagic::perfect_trait_copy_move<
          std::is_copy_constructible_v<T>,
          std::conjunction_v<std::is_copy_constructible<T>, std::is_copy_assignable<T>>,
          std::is_move_constructible_v<T>,
          std::conjunction_v<std::is_move_constructible<T>, std::is_move_assignable<T>>,
          Ok<T>>,
      public result::impl<Ok<T>>
{
  //! @cond
  template <class>
  friend class Ok;
  T x; //!< ok value
  template <class, class, class>
  friend class Result;
  template <class, class>
  friend struct result::ok_trait_injector;
  template <class, class>
  friend struct result::err_trait_injector;
  template <class, class>
  friend struct result::ok_err_trait_injector;
  template <class, class>
  friend struct result::printer_friend_injector;
  template <class... Requiers>
  using where = std::enable_if_t<std::conjunction_v<Requiers...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<Ok, U>>;
  //! @endcond
public:
  using ok_type = T; //!< dependent type
  using err_type = void;

  /*!
   * \brief default constructor
   * 
   * This constructor is defined if and only if
   * T is default constructible,
   * otherwise explicitly deleted.
   */
  Ok() = default;

  /*!
   * \brief non-explicit generic constructor
   * 
   * This constructor be a overload condidate
   * if and only if
   * T is conctructible from U and
   * U is convertible to T,
   * otherwise be excluded from overload condidates.
   */
  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<T, U>,
                  std::is_convertible<U, T>> = required>
  Ok(U && u) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::forward<U>(u)) {}

  /*!
   * \brief explicit generic constructor
   * 
   * This constructor be a overload condidate
   * if and only if
   * T is conctructible from U and
   * U is not convertible to T,
   * otherwise be excluded from overload condidates.
   */
  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit Ok(U && u) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::forward<U>(u)) {}

  /*!
   * \brief non-explicit generic copy constructor
   * 
   * This constructor be a overload condidate
   * if and only if
   * T is conctructible from U and
   * U is convertible to T,
   * otherwise be excluded from overload condidates.
   */
  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, const U &>,
                  std::is_convertible<const U &, T>> = required>
  constexpr Ok(const Ok<U> &t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(t.x) {}

  /*!
   * \brief explicit generic copy constructor
   * 
   * This constructor be a overload condidate
   * if and only if
   * T is conctructible from U and
   * U is not convertible to T,
   * otherwise be excluded from overload condidates.
   */
  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, const U &>,
                  std::negation<std::is_convertible<const U &, T>>> = required>
  explicit constexpr Ok(const Ok<U> &t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(t.x) {}

  /*!
   * \brief non-explicit generic move constructor
   * 
   * This constructor be a overload condidate
   * if and only if
   * T is conctructible from U and
   * U is convertible to T,
   * otherwise be excluded from overload condidates.
   */
  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, U &&>,
                  std::is_convertible<U &&, T>> = required>
  constexpr Ok(Ok<U> && t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::move(t.x)) {}

  /*!
   * \brief explicit generic move constructor
   * 
   * This constructor be a overload condidate
   * if and only if
   * T is conctructible from U and
   * U is not convertible to T,
   * otherwise be excluded from overload condidates.
   */
  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, U &&>,
                  std::negation<std::is_convertible<U &&, T>>> = required>
  explicit constexpr Ok(Ok<U> && t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::move(t.x)) {}

  /*!
   * \brief in-place constructor
   * 
   * Direct initialize to perfect forawrding args... to T().
   * 
   * This constructor be a overload condidate
   * T is constructible from Args... ,
   * otherwise be excluded from overload condidates.
   */
  template <class... Args,
            where<std::is_constructible<T, Args...>> = required>
  explicit constexpr Ok(std::in_place_t, Args && ... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
      : x(std::forward<Args>(args)...) {}

  /*!
   * \brief has ok value?
   * 
   * Always return true_type
   */
  constexpr std::true_type is_ok() const noexcept { return {}; }

  /*!
   * \brief has err value?
   * 
   * Always return false_type
   */
  constexpr std::false_type is_err() const noexcept { return {}; }

  /*!
   * \brief If has ok value apply operator op
   * 
   * Always apply operator
   */
  template <class Op>
  constexpr auto and_then(Op && op) noexcept(noexcept(std::declval<Op>()(std::declval<T &>())))
      ->std::enable_if_t<is_result_v<std::invoke_result_t<Op, T>>,
                         std::invoke_result_t<Op, T>>
  {
    return std::forward<Op>(op)(this->x);
  }

  /*!
   * \brief If has err value apply operator
   * 
   * Always return *this
   */
  template <class Op>
  constexpr auto or_else(Op &&) noexcept
  {
    return *this;
  }

  /*!
   * \brief Return x if has ok value, otherwise apply operator
   * 
   * Always return x
   */
  template <class O>
  T unwrap_or_else(O &&) const noexcept
  {
    return x;
  }

  template <class T_, class E_>
  std::enable_if_t<
      mitama::is_comparable_with<T, T_>::value,
      bool>
  operator==(Result<T_, E_> const &rhs) const
  {
    return rhs.is_ok() ? rhs.unwrap() == this->x : false;
  }

  template <class T_>
  std::enable_if_t<
      mitama::is_comparable_with<T, T_>::value,
      bool>
  operator==(Ok<T_> const &rhs) const
  {
    return this->x == rhs.x;
  }

  template <class E_>
  bool operator==(Err<E_> const &) const
  {
    return false;
  }

};

/*!
 * \brief deduction guide for Ok
 */
template <class T>
Ok(T &&)->Ok<std::decay_t<T>>;

/*
 * \brief Error stat generator class
 * 
 * Using convert to Result
 */
template <class E>
class[[nodiscard]] Err
    : private mitamagic::perfect_trait_copy_move<
          std::is_copy_constructible_v<E>,
          std::conjunction_v<std::is_copy_constructible<E>, std::is_copy_assignable<E>>,
          std::is_move_constructible_v<E>,
          std::conjunction_v<std::is_move_constructible<E>, std::is_move_assignable<E>>,
          Err<E>>,
      public result::impl<Err<E>>

{
  //! @cond
  template <class>
  friend class Err;
  E x; //!< error value
       // internal helpers
  template <class, class, class>
  friend class Result;
  template <class, class>
  friend struct result::ok_trait_injector;
  template <class, class>
  friend struct result::err_trait_injector;
  template <class, class>
  friend struct result::ok_err_trait_injector;
  template <class, class>
  friend struct result::printer_friend_injector;
  template <class... Requiers>
  using where = std::enable_if_t<std::conjunction_v<Requiers...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<Err, U>>;
  //! @endcond
public:
  using ok_type = void;
  using err_type = E; //!< dependent type

  /*!
   * \brief default constructor
   * 
   * This constructor is defined if and only if
   * E is default constructible,
   * otherwise explicitly deleted.
   */
  Err() = default;

  /*!
   * \brief non-explicit generic constructor
   * 
   * This constructor be a overload condidate
   * if and only if
   * E is conctructible from U and
   * U is convertible to E,
   * otherwise be excluded from overload condidates.
   */
  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<E, U>,
                  std::is_convertible<U, E>> = required>
  Err(U && u) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::forward<U>(u)) {}

  /*!
   * \brief explicit generic copy constructor
   * 
   * This constructor be a overload condidate
   * if and only if
   * E is conctructible from U and
   * U is not convertible to E,
   * otherwise be excluded from overload condidates.
   */
  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<E, U>,
                  std::negation<std::is_convertible<U, E>>> = required>
  explicit Err(U && u) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::forward<U>(u)) {}

  /*!
   * \brief non-explicit generic copy constructor
   * 
   * This constructor be a overload condidate
   * if and only if
   * E is conctructible from U and
   * U is convertible to E,
   * otherwise be excluded from overload condidates.
   */
  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, const U &>,
                  std::is_convertible<const U &, E>> = required>
  constexpr Err(const Err<U> &t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(t.x) {}

  /*!
   * \brief explicit generic copy constructor
   * 
   * This constructor be a overload condidate
   * if and only if
   * E is conctructible from U and
   * U is not convertible to E,
   * otherwise be excluded from overload condidates.
   */
  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, const U &>,
                  std::negation<std::is_convertible<const U &, E>>> = required>
  explicit constexpr Err(const Err<U> &t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(t.x) {}

  /*!
   * \brief non-explicit generic move constructor
   * 
   * This constructor be a overload condidate
   * if and only if
   * E is conctructible from U and
   * U is convertible to E,
   * otherwise be excluded from overload condidates.
   */
  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, U &&>,
                  std::is_convertible<U &&, E>> = required>
  constexpr Err(Err<U> && t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::move(t.x)) {}

  /*!
   * \brief explicit generic move constructor
   * 
   * This constructor be a overload condidate
   * if and only if
   * E is conctructible from U and
   * U is not convertible to E,
   * otherwise be excluded from overload condidates.
   */
  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, U &&>,
                  std::negation<std::is_convertible<U &&, E>>> = required>
  explicit constexpr Err(Err<U> && t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::move(t.x)) {}

  /*!
   * \brief in-place constructor
   * 
   * Direct initialize to perfect forawrding args... to E().
   * 
   * This constructor be a overload condidate
   * E is constructible from Args... ,
   * otherwise be excluded from overload condidates.
   */
  template <class... Args,
            where<std::is_constructible<E, Args...>> = required>
  explicit constexpr Err(std::in_place_t, Args && ... args) noexcept(std::is_nothrow_constructible_v<E, Args...>)
      : x(std::forward<Args>(args)...) {}

  /*!
   * \brief has ok value?
   * 
   * Always return false_type
   */
  constexpr std::false_type is_ok() const { return {}; }

  /*!
   * \brief has err value?
   * 
   * Always return true_type
   */
  constexpr std::true_type is_err() const { return {}; }

  /*!
   * \brief If has ok value apply operator op
   * 
   * Always return *this
   */
  template <class Op>
  constexpr auto and_then(Op &&)
  {
    return *this;
  }

  /*!
   * \brief If has err value apply operator
   * 
   * Always apply operator
   */
  template <class Op>
  constexpr auto or_else(Op && op)
      ->std::enable_if_t<is_result_v<std::invoke_result_t<Op, E>>,
                         std::invoke_result_t<Op, E>>
  {
    return std::forward<Op>(op)(this->x);
  }

  /*!
   * \brief Return x if has ok value, otherwise apply operator
   * 
   * Always apply operator
   */
  template <class O>
  auto unwrap_or_else(O && op) const noexcept
  {
    return std::forward<O>(op)(x);
  }

  template <class T_, class E_>
  std::enable_if_t<
      mitama::is_comparable_with<E, E_>::value,
      bool>
  operator==(Result<T_, E_> const &rhs) const
  {
    return rhs.is_err() ? rhs.unwrap_err() == this->x : false;
  }

  template <class E_>
  std::enable_if_t<
      mitama::is_comparable_with<E, E_>::value,
      bool>
  operator==(Err<E_> const &rhs) const
  {
    return this->x == rhs.x;
  }

  template <class T_>
  bool operator==(Ok<T_> const &) const
  {
    return false;
  }
};

/*!
 * \brief deduction guide for Err
 */
template <class E>
Err(E &&)->Err<std::decay_t<E>>;

//! @cond
inline constexpr auto None = std::nullopt;

template <class T>
constexpr std::optional<std::decay_t<T>> Some(T &&x)
{
  return {std::forward<T>(x)};
}
//! @endcond

/*!
 * \brief tag class for Result in-place constructor.
 */
class in_place_ok_t
{
};

/*!
 * \brief tag variable for Result in-place constructor.
 */
inline constexpr in_place_ok_t in_place_ok = {};

/*!
 * \brief tag class for Result in-place constructor.
 */
class in_place_err_t
{
};

/*!
 * \brief tag variable for Result in-place constructor.
 */
inline constexpr in_place_err_t in_place_err = {};

/*!
 * \brief data Result = Ok | Err
 * 
 * Result has a error value or ok value, like variant<Ok, Err>.
 */
template <class T, class E>
class[[nodiscard]] Result<T, E,
                          trait::where<std::is_destructible<T>,
                                       std::is_destructible<E>>>
    : private mitamagic::perfect_trait_copy_move<
          std::conjunction_v<std::is_copy_constructible<T>, std::is_copy_constructible<E>>,
          std::conjunction_v<std::is_copy_constructible<T>, std::is_copy_assignable<T>, std::is_copy_constructible<E>, std::is_copy_assignable<E>>,
          std::conjunction_v<std::is_move_constructible<T>, std::is_move_constructible<E>>,
          std::conjunction_v<std::is_move_constructible<T>, std::is_move_assignable<T>, std::is_move_constructible<E>, std::is_move_assignable<E>>,
          Result<T, E>>,
      public result::impl<Result<T, E>>
{
  //! @cond
  template <class, class>
  friend struct result::ok_trait_injector;
  template <class, class>
  friend struct result::err_trait_injector;
  template <class, class>
  friend struct result::ok_err_trait_injector;
  std::variant<Ok<T>, Err<E>> storage_;

  template <class... Requiers>
  using where = std::enable_if_t<std::conjunction_v<Requiers...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class T_, class E_>
  using not_self = std::negation<std::is_same<Result, Result<T_, E_>>>;
  //! @endcond
public:
  using ok_type = T;  //!< dependent type
  using err_type = E; //!< dependent type

  /*!
   * \brief default constructor is explicitly deleted
   *
   * Result is NonNullable.
   */
  constexpr Result() noexcept = delete;

  /*!
   * \brief copy constructor for Ok<U>
   *
   * This constructor be a overload condidate
   * if and only if
   * T is conctructible from U and
   * U is convertible to T,
   * otherwise be excluded from overload condidates.
   */
  template <class U,
            where<std::is_constructible<T, U>,
                  std::is_convertible<U, T>> = required>
  Result(Ok<U> const &ok)
      : storage_{std::in_place_type<Ok<T>>, ok.x}
  {
  }

  /*!
   * \brief copy constructor for Ok<U>
   *
   * This constructor be a overload condidate
   * if and only if
   * T is conctructible from U and
   * U is not convertible to T,
   * otherwise be excluded from overload condidates.
   */
  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit Result(Ok<U> const &ok)
      : storage_{std::in_place_type<Ok<T>>, ok.x}
  {
  }

  /*!
   * \brief move constructor for Ok<U>
   *
   * This constructor be a overload condidate
   * if and only if
   * T is conctructible from U and
   * U is convertible to T,
   * otherwise be excluded from overload condidates.
   */
  template <class U,
            where<std::is_constructible<T, U>,
                  std::is_convertible<U, T>> = required>
  Result(Ok<U> && ok)
      : storage_{std::in_place_type<Ok<T>>, ok.x}
  {
  }

  /*!
   * \brief move constructor for Ok<U>
   *
   * This constructor be a overload condidate
   * if and only if
   * T is conctructible from U and
   * U is not convertible to T,
   * otherwise be excluded from overload condidates.
   */
  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit Result(Ok<U> && ok)
      : storage_{std::in_place_type<Ok<T>>, ok.x}
  {
  }

  /*!
   * \brief copy constructor for Err<U>
   *
   * This constructor be a overload condidate
   * if and only if
   * E is conctructible from U and
   * U is convertible to E,
   * otherwise be excluded from overload condidates.
   */
  template <class U,
            where<std::is_constructible<E, U>,
                  std::is_convertible<U, E>> = required>
  Result(Err<U> const &err)
      : storage_{std::in_place_type<Err<E>>, err.x}
  {
  }

  /*!
   * \brief copy constructor for Err<U>
   *
   * This constructor be a overload condidate
   * if and only if
   * E is conctructible from U and
   * U is not convertible to E,
   * otherwise be excluded from overload condidates.
   */
  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit Result(Err<U> const &err)
      : storage_{std::in_place_type<Err<E>>, err.x}
  {
  }

  /*!
   * \brief move constructor for Err<U>
   *
   * This constructor be a overload condidate
   * if and only if
   * E is conctructible from U and
   * U is convertible to E,
   * otherwise be excluded from overload condidates.
   */
  template <class U,
            where<std::is_constructible<E, U>,
                  std::is_convertible<U, E>> = required>
  Result(Err<U> && err)
      : storage_{std::in_place_type<Err<E>>, err.x}
  {
  }

  /*!
   * \brief move constructor for Err<U>
   *
   * This constructor be a overload condidate
   * if and only if
   * E is conctructible from U and
   * U is not convertible to E,
   * otherwise be excluded from overload condidates.
   */
  template <class U,
            where<std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit Result(Err<U> && err)
      : storage_{std::in_place_type<Err<E>>, err.x}
  {
  }

  /*!
   * \biref constructor for ok value
   *
   * This constructor be a overload condidate
   * if and only if
   * T is conctructible from Args... ,
   * otherwise be excluded from overload condidates.
   * 
   * Example
   * -------
   * @code
   *     using my_result = Result<std::tuple<int, int>, std::string>;
   *     auto res = my_result( mitama::in_place_ok, 1, 1 ); // Ok(std::tuple{1,1})
   * @endcode
   */
  template <class... Args,
            where<std::is_constructible<T, Args...>> = required>
  explicit Result(in_place_ok_t, Args && ... args)
      : storage_{std::in_place_type<Ok<T>>, std::forward<Args>(args)...}
  {
  }

  /*!
   * \biref constructor for ok value
   *
   * This constructor be a overload condidate
   * if and only if
   * E is conctructible from Args... ,
   * otherwise be excluded from overload condidates.
   *
   * Example
   * -------
   * @code
   *     using my_result = Result<int, std::string>;
   *     auto res = my_result( mitama::in_place_err, 'a', 5 ); // Err("aaaaa")
   * @endcode
   */
  template <class... Args,
            where<std::is_constructible<E, Args...>> = required>
  explicit Result(in_place_err_t, Args && ... args)
      : storage_{std::in_place_type<Err<E>>, std::forward<Args>(args)...}
  {
  }

  /*!
   * \brief Returns true if the result is Ok.
   * 
   * Example
   * -------
   * @code
   *   Result<uint32_t, std::string> x = Ok(-3);
   *   assert(x.is_ok(), true);
   *
   *   Result<uint32_t, std::string> y = Err("Some error message");
   *   assert(y.is_ok(), false);
   * @endcode
   */
  constexpr bool is_ok() const noexcept { return std::holds_alternative<Ok<T>>(storage_); }

  /*!
   * \brief Returns true if the result is Err.
   *
   * Example
   * -------
   * @code
   *   Result<uint32_t, std::string> x = Ok(-3);
   *   assert(x.is_err(), false);
   *
   *   Result<uint32_t, std::string> y = Err("Some error message");
   *   assert_(y.is_err(), true);
   * @endcode
   */
  constexpr bool is_err() const noexcept { return std::holds_alternative<Err<E>>(storage_); }

  /*!
   * \brief Returns res if the result is Ok, otherwise returns the Err value of self.
   */
  template <class U>
  constexpr auto operator&&(Result<U, E> const &res) const &->Result<U, E>
  {
    using result_type = Result<U, E>;
    return this->is_err()
               ? static_cast<result_type>(Err{std::get<Err<E>>(static_cast<Result<T, E> const *>(this)->storage_).x})
               : res.is_err() ? static_cast<result_type>(Err{res.unwrap_err()})
                              : static_cast<result_type>(Ok{res.unwrap()});
  }

  /*!
   * \brief Returns res if the result is Err, otherwise returns the Ok value of self.
   * 
   * Arguments passed to or are eagerly evaluated;
   * if you are passing the result of a function call,
   * it is recommended to use or_else, which is lazily evaluated.
   */
  template <class F>
  constexpr auto operator||(Result<T, F> const &res) const &->Result<T, F>
  {
    using result_type = Result<T, F>;
    return this->is_ok()
               ? static_cast<result_type>(Ok{std::get<Ok<T>>(storage_).x})
               : res.is_ok() ? static_cast<result_type>(Ok{res.unwrap()})
                             : static_cast<result_type>(Err{res.unwrap_err()});
  }

  /*!
   * \brief Unwraps a result, yielding the content of an Ok. Else, it returns optb.
   * 
   * Arguments passed to unwrap_or are eagerly evaluated;
   * if you are passing the result of a function call,
   * it is recommended to use unwrap_or_else,
   * which is lazily evaluated.
   */
  T unwrap_or(T const &optb) const noexcept
  {
    return is_ok() ? std::get<Ok<T>>(storage_).x : optb;
  }

  /*!
   * \brief Unwraps a result, yielding the content of an Ok. If the value is an Err then it calls op with its value.
   */
  template <class O>
  auto unwrap_or_else(O && op) const noexcept(std::is_nothrow_invocable_r_v<T, O, E>)
      ->std::enable_if_t<std::is_invocable_r_v<T, O, E>, T>
  {
    return is_ok() ? std::get<Ok<T>>(storage_).x : std::forward<O>(op)(std::get<Err<E>>(storage_).x);
  }

  /*!
   * \brief Unwraps a result, yielding the content of an Ok.
   */
  T unwrap() const
  {
    if constexpr (trait::formattable<E>::value)
    {
      return is_ok()
                 ? std::get<Ok<T>>(storage_).x
                 : PANIC(R"(called `Result::unwrap() on an `Err` value: %1%)", std::get<Err<E>>(storage_).x);
    }
    else
    {
      return is_ok()
                 ? std::get<Ok<T>>(storage_).x
                 : PANIC(R"(called `Result::unwrap() on an `Err`)");
    }
  }

  /*!
   * \brief Unwraps a result, yielding the content of an Err.
   */
  E unwrap_err() const
  {
    if constexpr (trait::formattable<T>::value)
    {
      return is_err()
                 ? std::get<Err<E>>(storage_).x
                 : PANIC(R"(called `Result::unwrap_err() on an `Ok` value: %1%)", std::get<Ok<T>>(storage_).x);
    }
    else
    {
      return is_err()
                 ? std::get<Err<E>>(storage_).x
                 : PANIC(R"(called `Result::unwrap_err() on an `Ok` value)");
    }
  }

  template <class T_, class E_>
  std::enable_if_t<
      std::conjunction_v<mitama::is_comparable_with<T, T_>, mitama::is_comparable_with<E, E_>>,
      bool>
  operator==(Result<T_, E_> const &rhs) const &
  {
    return std::visit(::mitama::make_overload(
                          [](Ok<T> const &l, Ok<T_> const &r) { return l.x == r.x; },
                          [](Err<E> const &l, Err<E_> const &r) { return l.x == r.x; },
                          [](auto &&...) { return false; }),
                      this->storage_, rhs.storage_);
  }

  template <class T_>
  std::enable_if_t<
      mitama::is_comparable_with<T, T_>::value,
      bool>
  operator==(Ok<T_> const &rhs) const
  {
    return this->is_ok() ? this->unwrap() == rhs.x : false;
  }

  template <class E_>
  std::enable_if_t<
      mitama::is_comparable_with<E, E_>::value,
      bool>
  operator==(Err<E_> const &rhs) const
  {
    return this->is_err() ? this->unwrap_err() == rhs.x : false;
  }

};

template <class Ok, class Err, trait::where<trait::formattable<Ok>, trait::formattable<Err>> = nullptr>
std::ostream &operator<<(std::ostream &os, Result<Ok, Err> const &res)
{
  if (res.is_ok())
    return os << boost::format("Ok(%1%)") % res.unwrap();
  else
    return os << boost::format("Err(%1%)") % res.unwrap_err();
}

/** @} */ // end of Result

} // namespace mitama
