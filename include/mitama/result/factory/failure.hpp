#ifndef MITAMA_RESULT_FACTORY_FAILURE_HPP
#define MITAMA_RESULT_FACTORY_FAILURE_HPP
#include <mitama/result/detail/fwd.hpp>
#include <mitama/result/detail/meta.hpp>
#include <mitama/result/traits/impl_traits.hpp>
#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/hana/functional/overload_linearly.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace mitama {
/// class failure:
/// The main use of this class is to propagate unsuccessful results to the constructor of the result class.
template <class E>
class [[nodiscard]] failure
{
  template <class>
  friend class failure;
  E x;

  template <class... Requires>
  using where = std::enable_if_t<std::conjunction_v<Requires...>, std::nullptr_t>;

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
  constexpr failure(U&& u) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::forward<U>(u)) {}

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<E, U>,
                  std::negation<std::is_convertible<U, E>>> = required>
  explicit constexpr failure(U&& u) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::forward<U>(u)) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, const U &>,
                  std::is_convertible<const U &, E>> = required>
  constexpr failure(const failure<U> &t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(t.get()) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, const U &>,
                  std::negation<std::is_convertible<const U &, E>>> = required>
  explicit constexpr failure(const failure<U> &t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(t.get()) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, U&&>,
                  std::is_convertible<U&&, E>> = required>
  constexpr failure(failure<U> && t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::move(t.get())) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, U&&>,
                  std::negation<std::is_convertible<U&&, E>>> = required>
  explicit constexpr failure(failure<U> && t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::move(t.get())) {}

  template <class... Args,
            where<std::is_constructible<E, Args...>> = required>
  explicit constexpr failure(std::in_place_t, Args && ... args) noexcept(std::is_nothrow_constructible_v<E, Args...>)
      : x(std::forward<Args>(args)...) {}

  template <mutability _mut, class T_, class E_>
  std::enable_if_t<
      is_comparable_with<E, E_>::value,
      bool>
  constexpr operator==(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_err() ? rhs.unwrap_err() == this->x : false;
  }

  template <class T_>
  constexpr bool
  operator==(success<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    is_comparable_with<E, E_>::value,
  bool>
  operator==(failure<E_> const& rhs) const {
    return this->x == rhs.x;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    is_comparable_with<E, E_>::value,
  bool>
  operator!=(basic_result<_mut, T_, E_> const& rhs) const {
    return !(*this == rhs);
  }

  template <class T_>
  constexpr bool
  operator!=(success<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    is_comparable_with<E, E_>::value,
  bool>
  operator!=(failure<E_> const& rhs) const {
    return !(this->x == rhs.x);
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    is_comparable_with<E, E_>::value,
  bool>
  operator<(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_err() ? this->x < rhs.unwrap_err() : true;
  }

  template <class T_>
  constexpr bool
  operator<(success<T_> const&) const {
    return true;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    meta::is_less_comparable_with<E, E_>::value,
  bool>
  operator<(failure<E_> const& rhs) const {
    return this->x < rhs.x;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      is_comparable_with<E, E_>,
      meta::is_less_comparable_with<E, E_>>,
  bool>
  operator<=(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_err() ? (this->x == rhs.unwrap_err()) || (this->x < rhs.unwrap_err()) : true;
  }

  template <class T_>
  constexpr bool
  operator<=(success<T_> const&) const {
    return true;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      is_comparable_with<E, E_>,
      meta::is_less_comparable_with<E, E_>>,
  bool>
  operator<=(failure<E_> const& rhs) const {
    return (*this == rhs) || (*this < rhs);
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    meta::is_less_comparable_with<E_, E>::value,
  bool>
  operator>(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_err() ? rhs.unwrap_err() < this->x : false;
  }

  template <class T_>
  constexpr bool
  operator>(success<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<E_, E>::value,
  bool>
  operator>(failure<E_> const& rhs) const {
    return rhs < *this;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      is_comparable_with<E_, E>,
      meta::is_less_comparable_with<E_, E>>,
  bool>
  operator>=(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_err() ? (rhs.unwrap_err() == this->x) || (rhs.unwrap_err() < this->x) : false;
  }

  template <class T_>
  constexpr bool
  operator>=(success<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      is_comparable_with<E_, E>,
      meta::is_less_comparable_with<E_, E>>,
  bool>
  operator>=(failure<E_> const& rhs) const {
    return rhs <= *this;
  }

  E& get() & { return x; }
  E const& get() const& { return x; }
  E&& get() && { return std::move(x); }

};


template <class E>
class [[nodiscard]] failure<E&>
{
  template <class>
  friend class failure;
  std::reference_wrapper<E> x;

  template <class... Requires>
  using where = std::enable_if_t<std::conjunction_v<Requires...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<failure, U>>;
public:
  using err_type = E&;

  failure() = delete;
  explicit constexpr failure(E& err) : x(err) {}
  explicit constexpr failure(std::in_place_t, E& err) : x(err) {}
  explicit constexpr failure(failure &&) = default;
  explicit constexpr failure(failure const&) = default;
  constexpr failure& operator=(failure &&) = default;
  constexpr failure& operator=(failure const&) = default;

  template <mutability _mut, class T_, class E_>
  std::enable_if_t<
      is_comparable_with<E, E_>::value,
      bool>
  constexpr operator==(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_err() ? rhs.unwrap_err() == this->x : false;
  }

  template <class T_>
  constexpr bool
  operator==(success<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    is_comparable_with<E, E_>::value,
  bool>
  operator==(failure<E_> const& rhs) const {
    return this->x == rhs.x;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    is_comparable_with<E, E_>::value,
  bool>
  operator!=(basic_result<_mut, T_, E_> const& rhs) const {
    return !(*this == rhs);
  }

  template <class T_>
  constexpr bool
  operator!=(success<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    is_comparable_with<E, E_>::value,
  bool>
  operator!=(failure<E_> const& rhs) const {
    return !(this->x == rhs.x);
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    is_comparable_with<E, E_>::value,
  bool>
  operator<(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_err() ? this->x < rhs.unwrap_err() : true;
  }

  template <class T_>
  constexpr bool
  operator<(success<T_> const&) const {
    return true;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    meta::is_less_comparable_with<E, E_>::value,
  bool>
  operator<(failure<E_> const& rhs) const {
    return this->x < rhs.x;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      is_comparable_with<E, E_>,
      meta::is_less_comparable_with<E, E_>>,
  bool>
  operator<=(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_err() ? (this->x == rhs.unwrap_err()) || (this->x < rhs.unwrap_err()) : true;
  }

  template <class T_>
  constexpr bool
  operator<=(success<T_> const&) const {
    return true;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      is_comparable_with<E, E_>,
      meta::is_less_comparable_with<E, E_>>,
  bool>
  operator<=(failure<E_> const& rhs) const {
    return (*this == rhs) || (*this < rhs);
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    meta::is_less_comparable_with<E_, E>::value,
  bool>
  operator>(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_err() ? rhs.unwrap_err() < this->x : false;
  }

  template <class T_>
  constexpr bool
  operator>(success<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<E_, E>::value,
  bool>
  operator>(failure<E_> const& rhs) const {
    return rhs < *this;
  }

  template <mutability _mut, class T_, class E_>
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      is_comparable_with<E_, E>,
      meta::is_less_comparable_with<E_, E>>,
  bool>
  operator>=(basic_result<_mut, T_, E_> const& rhs) const
  {
    return rhs.is_err() ? (rhs.unwrap_err() == this->x) || (rhs.unwrap_err() < this->x) : false;
  }

  template <class T_>
  constexpr bool
  operator>=(success<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      is_comparable_with<E_, E>,
      meta::is_less_comparable_with<E_, E>>,
  bool>
  operator>=(failure<E_> const& rhs) const {
    return rhs <= *this;
  }

  E& get() & { return x.get(); }
  E const& get() const& { return x.get(); }
  E&& get() && { return std::move(x.get()); }

};

  /// @brief
  ///   ostream output operator
  ///
  /// @requires
  ///   Format<T>;
  ///   Format<E>
  ///
  /// @note
  ///   Output its contained value with pretty format, and is used by `operator<<` found by ADL.
  template <class E>
  inline std::enable_if_t<trait::formattable<E>::value, std::ostream&>
  operator<<(std::ostream& os, failure<E> const& err) {
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
    return os << boost::format("failure(%1%)") % inner_format( err.get() );
  }


}

#endif
