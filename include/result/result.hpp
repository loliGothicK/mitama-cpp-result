#ifndef MITAMA_RESULT_HPP
#define MITAMA_RESULT_HPP
#include <result/detail/fwd.hpp>
#include <result/detail/meta.hpp>
#include <result/traits/perfect_traits_special_members.hpp>

#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/hana/functional/overload_linearly.hpp>
#include <boost/hana/functional/fix.hpp>
#include <boost/format.hpp>

#include <functional>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <string_view>

#define PANIC(...) \
  throw ::mitama::runtime_panic { ::mitama::macro_use, __FILE__, __LINE__, __VA_ARGS__ }

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

class macro_use_tag_t{};
inline static constexpr macro_use_tag_t macro_use{};

class runtime_panic : public std::runtime_error
{
public:
  template <class... Args>
  runtime_panic(boost::format fmt, Args &&... args) noexcept
      : std::runtime_error((fmt % ... % args).str()) {}

  template <class... Args>
  explicit runtime_panic(macro_use_tag_t, const char *func, int line, std::string fmt, Args &&... args) noexcept
      : std::runtime_error(
            std::string{"runtime panicked at '"} + (boost::format(fmt) % ... % [](auto&& arg [[maybe_unused]]){
              using namespace std::literals::string_view_literals;
              if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::monostate>) {
                return "()"sv;
              }
              else {
                return std::forward<decltype(arg)>(arg);
              }
            }(args)).str() +
            (boost::format("', %1%:%2%") % std::string{func} % line).str()) {}
};

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

#include <result/detail/result_impl.hpp>

namespace mitama {

/// class success:
/// The main use of this class is to propagate successful results to the constructor of the result class.
template <class T>
class [[nodiscard]] success
    : private ::mitamagic::perfect_trait_copy_move<
          std::is_copy_constructible_v<T>,
          std::conjunction_v<std::is_copy_constructible<T>, std::is_copy_assignable<T>>,
          std::is_move_constructible_v<T>,
          std::conjunction_v<std::is_move_constructible<T>, std::is_move_assignable<T>>,
          success<T>>
{
  template <class>
  friend class success;
  T x;
  template <mutability, class, class, class>
  friend class basic_result;
  template <class, class>
  friend class transpose_friend_injector;
  template <class, class>
  friend class indirect_friend_injector;

  template <class... Requiers>
  using where = std::enable_if_t<std::conjunction_v<Requiers...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<success, U>>;
public:
  using ok_type = T;

  template <class U = T>
  constexpr success(std::enable_if_t<std::is_same_v<std::monostate, U>, std::nullptr_t> = nullptr)
  { /* whatever */ }

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<T, U>,
                  std::is_convertible<U, T>> = required>
  constexpr success(U && u) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::forward<U>(u)) {}

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<T, U>,
                  std::negation<std::is_convertible<U, T>>> = required>
  explicit constexpr success(U && u) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::forward<U>(u)) {}

  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, const U &>,
                  std::is_convertible<const U &, T>> = required>
  constexpr success(const success<U> &t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(t.x) {}

  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, const U &>,
                  std::negation<std::is_convertible<const U &, T>>> = required>
  explicit constexpr success(const success<U> &t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(t.x) {}

  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, U &&>,
                  std::is_convertible<U &&, T>> = required>
  constexpr success(success<U> && t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::move(t.x)) {}

  template <typename U,
            where<std::negation<std::is_same<T, U>>,
                  std::is_constructible<T, U &&>,
                  std::negation<std::is_convertible<U &&, T>>> = required>
  explicit constexpr success(success<U> && t) noexcept(std::is_nothrow_constructible_v<T, U>)
      : x(std::move(t.x)) {}

  template <class... Args,
            where<std::is_constructible<T, Args...>> = required>
  explicit constexpr success(std::in_place_t, Args && ... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
      : x(std::forward<Args>(args)...) {}

  template <mutability _mut, class T_, class E_>
  std::enable_if_t<
      is_comparable_with<T, T_>::value,
      bool>
  constexpr operator==(basic_result<_mut, T_, E_> const &rhs) const
  {
    return rhs.is_ok() ? rhs.unwrap() == this->x : false;
  }

  template <class T_>
  std::enable_if_t<
      is_comparable_with<T, T_>::value,
      bool>
  constexpr operator==(success<T_> const &rhs) const
  {
    return this->x == rhs.x;
  }

  template <class E_>
  constexpr bool operator==(failure<E_> const &) const
  {
    return false;
  }

  template <class U = T>
  friend
  std::enable_if_t<
    std::disjunction_v<
        trait::formattable<U>,
        trait::formattable_range<U>,
        trait::formattable_tuple<U>
    >,
  std::ostream&>
  operator<<(std::ostream& os, success const& ok) {
    using namespace std::literals::string_literals;
    auto inner_format = boost::hana::fix(boost::hana::overload_linearly(
        [](auto, auto const& x) -> std::enable_if_t<trait::formattable_element<std::decay_t<decltype(x)>>::value, std::string> {
          return boost::hana::overload_linearly(
            [](std::monostate) { return "()"s; },
            [](std::string_view x) { return (boost::format("\"%1%\"") % x).str(); },
            [](auto const& x) { return (boost::format("%1%") % x).str(); })
          (x);        
        },
        [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_dictionary<std::decay_t<decltype(x)>>::value, std::string> {
          if (x.empty()) return "{}"s;
          using std::begin, std::end;
          auto iter = begin(x);
          std::string str = "{"s + (boost::format("%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
          while (++iter != end(x)) {
            str += (boost::format(",%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
          }
          return str += "}";
        },
        [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_range<std::decay_t<decltype(x)>>::value, std::string> {
          if (x.empty()) return "[]"s;
          using std::begin, std::end;
          auto iter = begin(x);
          std::string str = "["s + _fmt(*iter);
          while (++iter != end(x)) {
            str += (boost::format(",%1%") % _fmt(*iter)).str();
          }
          return str += "]";
        },
        [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_tuple<std::decay_t<decltype(x)>>::value, std::string> {
          if constexpr (std::tuple_size_v<std::decay_t<decltype(x)>> == 0) {
            return "()"s;
          }
          else {
            return std::apply(
              [_fmt](auto const& head, auto const&... tail) {
                return "("s + _fmt(head) + ((("," + _fmt(tail))) + ...) + ")"s;
              }, x);
          }
        }));
    return os << boost::format("success(%1%)") % inner_format(ok.x);
  }
};

/// Deduction guide for `success`
template <class T>
success(T&&)->success<T>;


/// class failure:
/// The main use of this class is to propagate unsuccessful results to the constructor of the result class.
template <class E>
class [[nodiscard]] failure
    : private ::mitamagic::perfect_trait_copy_move<
          std::is_copy_constructible_v<E>,
          std::conjunction_v<std::is_copy_constructible<E>, std::is_copy_assignable<E>>,
          std::is_move_constructible_v<E>,
          std::conjunction_v<std::is_move_constructible<E>, std::is_move_assignable<E>>,
          failure<E>>
{
  template <class>
  friend class failure;
  E x;
  template <mutability, class, class, class>
  friend class basic_result;
  template <class, class>
  friend class printer_friend_injector;
  template <class, class>
  friend class transpose_friend_injector;
  template <class, class>
  friend class indirect_friend_injector;

  template <class... Requiers>
  using where = std::enable_if_t<std::conjunction_v<Requiers...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<failure, U>>;
public:
  using err_type = E;

  template <class F = E>
  constexpr failure(std::enable_if_t<std::is_same_v<std::monostate, F>, std::nullptr_t> = nullptr)
  { /* whatever */ }

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<E, U>,
                  std::is_convertible<U, E>> = required>
  constexpr failure(U && u) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::forward<U>(u)) {}

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<E, U>,
                  std::negation<std::is_convertible<U, E>>> = required>
  explicit constexpr failure(U && u) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::forward<U>(u)) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, const U &>,
                  std::is_convertible<const U &, E>> = required>
  constexpr failure(const failure<U> &t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(t.x) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, const U &>,
                  std::negation<std::is_convertible<const U &, E>>> = required>
  explicit constexpr failure(const failure<U> &t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(t.x) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, U &&>,
                  std::is_convertible<U &&, E>> = required>
  constexpr failure(failure<U> && t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::move(t.x)) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, U &&>,
                  std::negation<std::is_convertible<U &&, E>>> = required>
  explicit constexpr failure(failure<U> && t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::move(t.x)) {}

  template <class... Args,
            where<std::is_constructible<E, Args...>> = required>
  explicit constexpr failure(std::in_place_t, Args && ... args) noexcept(std::is_nothrow_constructible_v<E, Args...>)
      : x(std::forward<Args>(args)...) {}

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
      is_comparable_with<E, E_>::value,
      bool>
  operator==(basic_result<_mut, T_, E_> const &rhs) const
  {
    return rhs.is_err() ? rhs.unwrap_err() == this->x : false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
      is_comparable_with<E, E_>::value,
      bool>
  operator==(failure<E_> const &rhs) const
  {
    return this->x == rhs.x;
  }

  template <class T_>
  constexpr bool operator==(success<T_> const &) const
  {
    return false;
  }

  template <class F = E>
  friend
  std::enable_if_t<
    std::disjunction_v<
        trait::formattable<F>,
        trait::formattable_range<F>,
        trait::formattable_tuple<F>
    >,
  std::ostream&>
  operator<<(std::ostream& os, failure const& err) {
    using namespace std::literals::string_literals;
    auto inner_format = boost::hana::fix(boost::hana::overload_linearly(
        [](auto, auto const& x) -> std::enable_if_t<trait::formattable_element<std::decay_t<decltype(x)>>::value, std::string> {
          return boost::hana::overload_linearly(
            [](std::monostate) { return "()"s; },
            [](std::string_view x) { return (boost::format("\"%1%\"") % x).str(); },
            [](auto const& x) { return (boost::format("%1%") % x).str(); })
          (x);        
        },
        [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_dictionary<std::decay_t<decltype(x)>>::value, std::string> {
          if (x.empty()) return "{}"s;
          using std::begin, std::end;
          auto iter = begin(x);
          std::string str = "{"s + (boost::format("%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
          while (++iter != end(x)) {
            str += (boost::format(",%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
          }
          return str += "}";
        },
        [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_range<std::decay_t<decltype(x)>>::value, std::string> {
          if (x.empty()) return "[]"s;
          using std::begin, std::end;
          auto iter = begin(x);
          std::string str = "["s + _fmt(*iter);
          while (++iter != end(x)) {
            str += (boost::format(",%1%") % _fmt(*iter)).str();
          }
          return str += "]";
        },
        [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_tuple<std::decay_t<decltype(x)>>::value, std::string> {
          if constexpr (std::tuple_size_v<std::decay_t<decltype(x)>> == 0) {
            return "()"s;
          }
          else {
            return std::apply(
              [_fmt](auto const& head, auto const&... tail) {
                return "("s + _fmt(head) + ((("," + _fmt(tail))) + ...) + ")"s;
              }, x);
          }
        }));
    return os << boost::format("failure(%1%)") % inner_format(err.x);
  }
};

/// Deduction guide for `failure`
template <class E>
failure(E&&)->failure<E>;

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

/// alias template for immutable result
template <class T = std::monostate, class E = std::monostate>
using result = basic_result<mutability::immut, T, E>;

/// alias template for mutable result
template <class T = std::monostate, class E = std::monostate>
using mut_result = basic_result<mutability::mut, T, E>;

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
  template <class, class>
  friend class printer_friend_injector;
  template <class, class>
  friend class transpose_friend_injector;
  template <class, class>
  friend class indirect_friend_injector;
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

  /// @brief
  ///   in-place constructor for successful result
  template <class... Args,
            where<std::is_constructible<T, Args...>> = required>
  constexpr explicit basic_result(in_place_ok_t, Args && ... args)
    : storage_{success<T>{std::forward<Args>(args)...}}
  {}

  /// @brief
  ///   in-place constructor for unsuccessful result
  template <class... Args,
            where<std::is_constructible<E, Args...>> = required>
  constexpr explicit basic_result(in_place_err_t, Args && ... args)
    : storage_{failure<E>{std::forward<Args>(args)...}}
  {}

  /// @brief
  ///   in-place constructor with initializer_list for successful result
  template <class U, class... Args,
            where<std::is_constructible<T, std::initializer_list<U>, Args...>> = required>
  constexpr explicit basic_result(in_place_ok_t, std::initializer_list<U> il, Args && ... args)
    : storage_{success<T>{il, std::forward<Args>(args)...}}
  {}

  /// @brief
  ///   in-place constructor with initializer_list for unsuccessful result
  template <class U, class... Args,
            where<std::is_constructible<E, Args...>> = required>
  constexpr explicit basic_result(in_place_err_t, std::initializer_list<U> il, Args && ... args)
    : storage_{failure<E>{il, std::forward<Args>(args)...}}
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
  ///   Converts from basic_result to boost::optional.
  ///
  /// @note
  ///   If self is
  ///   - immutable: returns `boost::optional<const T>`
  ///   -  mutable : returns `boost::optional<T>`
  constexpr
  std::conditional_t<is_mut_v<_mutability>,
    boost::optional<ok_type>,
    boost::optional<force_add_const_t<ok_type>>
  >
  ok() & noexcept {
    using ret = std::conditional_t<is_mut_v<_mutability>, boost::optional<ok_type>, boost::optional<force_add_const_t<ok_type>>>;
    if (is_ok()) {
      return ret{boost::get<success<T>>(storage_).x};
    }
    else {
      return ret{boost::none};
    }
  }

  /// @brief
  ///   Converts from basic_result to `boost::optional<const T>`.
  ///
  /// @note
  ///   Converts self into a `boost::optional<const T>`, and discarding the failure, if any.
  constexpr
  boost::optional<force_add_const_t<ok_type>>
  ok() const & noexcept {
    if (is_ok()) {
      return boost::optional<force_add_const_t<ok_type>>{boost::get<success<T>>(std::move(storage_)).x};
    }
    else {
      return boost::optional<force_add_const_t<ok_type>>{boost::none};
    }
  }

  /// @brief
  ///   Converts from basic_result to `boost::optional`.
  ///
  /// @note
  ///   If T is lvalue reference and self is
  ///     - immutable: returns `boost::optional<dangling<std::reference_wrapper<const T>>>`
  ///     -  mutable : returns `boost::optional<dangling<std::reference_wrapper<T>>>`
  ///   T is not lvalue reference and self is
  ///     - immutable: returns `oost::optional<const T>`
  ///     -  mutable : returns `boost::optional<T>`
  constexpr auto
  ok() && noexcept {
    using ret = std::conditional_t<is_mut_v<_mutability>, boost::optional<ok_type>, boost::optional<force_add_const_t<ok_type>>>;
    if constexpr (std::is_lvalue_reference_v<ok_type>) {
      if (is_ok()) {
        if constexpr (is_mut_v<_mutability>) {
          return boost::optional<dangle_ref<std::remove_reference_t<ok_type>>>{boost::in_place(std::ref(boost::get<success<T>>(storage_).x))};
        }
        else {
          return boost::optional<dangle_ref<std::add_const_t<std::remove_reference_t<ok_type>>>>{boost::in_place(std::cref(boost::get<success<T>>(storage_).x))};
        }
      }
      else {
        return boost::optional<dangle_ref<std::conditional_t<is_mut_v<_mutability>, std::remove_reference_t<ok_type>, force_add_const_t<std::remove_reference_t<ok_type>>>>>{boost::none};
      }
    }
    else {
      if (is_ok()) {
        return ret{boost::get<success<T>>(std::move(storage_)).x};
      }
      else {
        return ret{boost::none};
      }
    }
  }

  void ok() const&& = delete;

  /// @brief
  ///   Converts from basic_result to `boost::optional`.
  ///
  /// @note
  ///   Converts self into a `boost::optional<const E>`, and discarding the success, if any.
  constexpr
  boost::optional<force_add_const_t<err_type>>
  err() const & noexcept {
    if (is_err()) {
      return boost::optional<E>{boost::get<failure<E>>(storage_).x};
    }
    else {
      return boost::optional<E>{boost::none};
    }
  }

  /// @brief
  ///   Converts from basic_result to `boost::optional<const E>`.
  ///
  /// @note
  ///   If self is
  ///   - immutable: returns `boost::optional<const E>`
  ///   -  mutable : returns `boost::optional<E>`
  constexpr
  std::conditional_t<is_mut_v<_mutability>,
    boost::optional<err_type>,
    boost::optional<force_add_const_t<err_type>>
  >
  err() & noexcept {
    using ret = std::conditional_t<is_mut_v<_mutability>, boost::optional<err_type>, boost::optional<force_add_const_t<err_type>>>;
    if (is_err()) {
      return ret{boost::get<failure<E>>(std::move(storage_)).x};
    }
    else {
      return ret{boost::none};
    }
  }

  /// @brief
  ///   Converts from basic_result to `boost::optional`.
  ///
  /// @note
  ///   If E is lvalue reference and self is
  ///     - immutable: returns boost::optional<dangling<std::reference_wrapper<const E>>>
  ///     -  mutable : returns boost::optional<dangling<std::reference_wrapper<E>>>
  ///   E is not lvalue reference and self is
  ///     - immutable: returns boost::optional<const E>
  ///     -  mutable : returns boost::optional<E>
  constexpr auto
  err() && noexcept {
    using ret = std::conditional_t<is_mut_v<_mutability>, boost::optional<err_type>, boost::optional<force_add_const_t<err_type>>>;
    if constexpr (std::is_lvalue_reference_v<ok_type>) {
      if (is_err()) {
        if constexpr (is_mut_v<_mutability>) {
          return boost::optional<dangle_ref<std::remove_reference_t<err_type>>>{boost::in_place(std::ref(boost::get<failure<E>>(storage_).x))};
        }
        else {
          return boost::optional<dangle_ref<std::add_const_t<std::remove_reference_t<err_type>>>>{boost::in_place(std::cref(boost::get<failure<E>>(storage_).x))};
        }
      }
      else {
        return boost::optional<dangle_ref<std::conditional_t<is_mut_v<_mutability>, std::remove_reference_t<err_type>, force_add_const_t<std::remove_reference_t<err_type>>>>>{boost::none};
      }
    }
    else {
      if (is_err()) {
        return ret{boost::get<failure<E>>(std::move(storage_)).x};
      }
      else {
        return ret{boost::none};
      }
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
  T unwrap() const {
    if constexpr (trait::formattable_element<E>::value) {
      if ( is_ok() ) {
        return boost::get<success<T>>(storage_).x;
      }
      else {
        PANIC(R"(called `basic_result::unwrap()` on a value: %1%)", *this);
      }      
    }
    else {
      if ( is_ok() ) {
        return boost::get<success<T>>(storage_).x;
      }
      else {
        PANIC(R"(called `basic_result::unwrap()` on a value `failure(???)`)");
      }
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an failure.
  ///
  /// @panics
  ///   Panics if the value is an success, with a panic message provided by the success's value.
  E unwrap_err() const {
    if constexpr (trait::formattable<T>::value) {
      if ( is_err() ) {
        return boost::get<failure<E>>(storage_).x;
      }
      else {
        PANIC(R"(called `basic_result::unwrap_err()` on a value: %1%)", *this);
      }
    }
    else {
      if ( is_err() ) {
        return boost::get<failure<E>>(storage_).x;
      }
      else {
        PANIC(R"(called `basic_result::unwrap_err()` on a value `success(???)`)");
      }
    }
  }

  /// @brief
  ///   Unwraps a result, yielding the content of an success.
  ///
  /// @panics
  ///   Panics if the value is an failure, with a panic message including the passed message, and the content of the failure.
  T expect(std::string_view msg) const {
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
  void expect_err(std::string_view msg) const {
    if ( is_ok() )
      PANIC("%1%: %2%", msg, unwrap());
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

  /// @brief
  ///   ostream output operator
  ///
  /// @requires
  ///   Format<T>;
  ///   Format<E>
  ///
  /// @note
  ///   Output its contained value with pretty format, and is used by `operator<<` found by ADL.
  template <class U = T, class F = E>
  friend
  std::enable_if_t<std::conjunction_v<trait::formattable<U>, trait::formattable<F>>,
  std::ostream&>
  operator<<(std::ostream& os, basic_result const& res) {
    using namespace std::literals::string_literals;
    auto inner_format = boost::hana::fix(boost::hana::overload_linearly(
        [](auto, auto const& x) -> std::enable_if_t<trait::formattable_element<std::decay_t<decltype(x)>>::value, std::string> {
          return boost::hana::overload_linearly(
            [](std::monostate) { return "()"s; },
            [](std::string_view x) { return (boost::format("\"%1%\"") % x).str(); },
            [](auto const& x) { return (boost::format("%1%") % x).str(); })
          (x);        
        },
        [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_dictionary<std::decay_t<decltype(x)>>::value, std::string> {
          if (x.empty()) return "{}"s;
          using std::begin, std::end;
          auto iter = begin(x);
          std::string str = "{"s + (boost::format("%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
          while (++iter != end(x)) {
            str += (boost::format(",%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
          }
          return str += "}";
        },
        [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_range<std::decay_t<decltype(x)>>::value, std::string> {
          if (x.empty()) return "[]"s;
          using std::begin, std::end;
          auto iter = begin(x);
          std::string str = "["s + _fmt(*iter);
          while (++iter != end(x)) {
            str += (boost::format(",%1%") % _fmt(*iter)).str();
          }
          return str += "]";
        },
        [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_tuple<std::decay_t<decltype(x)>>::value, std::string> {
          if constexpr (std::tuple_size_v<std::decay_t<decltype(x)>> == 0) {
            return "()"s;
          }
          else {
            return std::apply(
              [_fmt](auto const& head, auto const&... tail) {
                return "("s + _fmt(head) + ((("," + _fmt(tail))) + ...) + ")"s;
              }, x);
          }
        }));
    return res.is_ok() ? os << boost::format("success(%1%)") % inner_format( res.unwrap() )
                       : os << boost::format("failure(%1%)") % inner_format( res.unwrap_err() );
  }
};

} // namespace mitama

#define TRY(target) \
  if (auto res = target; res.is_err()) \
    return ::mitama::failure(res.unwrap_err());

#define TRY_LET(var, target) \
  if (auto var = target; var.is_err()) \
    return ::mitama::failure(var.unwrap_err());
  
#endif
