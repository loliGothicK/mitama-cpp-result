#ifndef MITAMA_RESULT_FACTORY_FAILURE_HPP
#define MITAMA_RESULT_FACTORY_FAILURE_HPP
#include <mitama/mitamagic/is_interface_of.hpp>
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
/// class failure_t:
/// The main use of this class is to propagate unsuccessful results to the constructor of the result class.
template <class E>
class [[nodiscard]] failure_t<E>
{
  template <class, class...>
  friend class failure_t;
  E x;

  template <class... Requires>
  using where = std::enable_if_t<std::conjunction_v<Requires...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<failure_t, U>>;
public:
  using err_type = E;

  template <class F = E>
  constexpr failure_t(std::enable_if_t<std::is_same_v<std::monostate, F>, std::nullptr_t> = nullptr)
  { /* whatever */ }

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<E, U>,
                  std::is_convertible<U, E>> = required>
  constexpr failure_t(U&& u) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::forward<U>(u)) {}

  template <class U,
            where<not_self<std::decay_t<U>>,
                  std::is_constructible<E, U>,
                  std::negation<std::is_convertible<U, E>>> = required>
  explicit constexpr failure_t(U&& u) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::forward<U>(u)) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, const U &>,
                  std::is_convertible<const U &, E>> = required>
  constexpr failure_t(const failure_t<U> &t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(t.get()) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, const U &>,
                  std::negation<std::is_convertible<const U &, E>>> = required>
  explicit constexpr failure_t(const failure_t<U> &t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(t.get()) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, U&&>,
                  std::is_convertible<U&&, E>> = required>
  constexpr failure_t(failure_t<U> && t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::move(t.get())) {}

  template <typename U,
            where<std::negation<std::is_same<E, U>>,
                  std::is_constructible<E, U&&>,
                  std::negation<std::is_convertible<U&&, E>>> = required>
  explicit constexpr failure_t(failure_t<U> && t) noexcept(std::is_nothrow_constructible_v<E, U>)
      : x(std::move(t.get())) {}

  template <class... Args,
            where<std::is_constructible<E, Args...>> = required>
  explicit constexpr failure_t(std::in_place_t, Args && ... args) noexcept(std::is_nothrow_constructible_v<E, Args...>)
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
  operator==(success_t<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    is_comparable_with<E, E_>::value,
  bool>
  operator==(failure_t<E_> const& rhs) const {
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
  operator!=(success_t<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    is_comparable_with<E, E_>::value,
  bool>
  operator!=(failure_t<E_> const& rhs) const {
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
  operator<(success_t<T_> const&) const {
    return true;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    meta::is_less_comparable_with<E, E_>::value,
  bool>
  operator<(failure_t<E_> const& rhs) const {
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
  operator<=(success_t<T_> const&) const {
    return true;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      is_comparable_with<E, E_>,
      meta::is_less_comparable_with<E, E_>>,
  bool>
  operator<=(failure_t<E_> const& rhs) const {
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
  operator>(success_t<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<E_, E>::value,
  bool>
  operator>(failure_t<E_> const& rhs) const {
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
  operator>=(success_t<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      is_comparable_with<E_, E>,
      meta::is_less_comparable_with<E_, E>>,
  bool>
  operator>=(failure_t<E_> const& rhs) const {
    return rhs <= *this;
  }

  E& get() & { return x; }
  E const& get() const& { return x; }
  E&& get() && { return std::move(x); }

};


template <class E>
class [[nodiscard]] failure_t<E&>
{
  template <class, class...>
  friend class failure_t;
  std::reference_wrapper<E> x;

  template <class... Requires>
  using where = std::enable_if_t<std::conjunction_v<Requires...>, std::nullptr_t>;

  static constexpr std::nullptr_t required = nullptr;

  template <class U>
  using not_self = std::negation<std::is_same<failure_t, U>>;
public:
  using err_type = E&;

  failure_t() = delete;
  explicit constexpr failure_t(E& err) : x(err) {}
  explicit constexpr failure_t(std::in_place_t, E& err) : x(err) {}

  template <class Derived, std::enable_if_t<mitamagic::is_interface_of_v<std::decay_t<E>, std::decay_t<Derived>>, bool> = false>
  explicit constexpr failure_t(Derived& derived) : x(derived) {}
  template <class Derived, std::enable_if_t<mitamagic::is_interface_of_v<std::decay_t<E>, std::decay_t<Derived>>, bool> = false>
  explicit constexpr failure_t(std::in_place_t, Derived& derived) : x(derived) {}

  explicit constexpr failure_t(failure_t &&) = default;
  explicit constexpr failure_t(failure_t const&) = default;
  constexpr failure_t& operator=(failure_t &&) = default;
  constexpr failure_t& operator=(failure_t const&) = default;

  template <mutability _mut, class T_, class E_>
  std::enable_if_t<
      is_comparable_with<E, E_>::value,
      bool>
  constexpr operator==(basic_result<_mut, T_, E_> const& rhs) const {
    return rhs.is_err() ? rhs.unwrap_err() == this->x : false;
  }

  template <class T_>
  constexpr bool
  operator==(success_t<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    is_comparable_with<E, E_>::value,
  bool>
  operator==(failure_t<E_> const& rhs) const {
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
  operator!=(success_t<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    is_comparable_with<E, E_>::value,
  bool>
  operator!=(failure_t<E_> const& rhs) const {
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
  operator<(success_t<T_> const&) const {
    return true;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    meta::is_less_comparable_with<E, E_>::value,
  bool>
  operator<(failure_t<E_> const& rhs) const {
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
  operator<=(success_t<T_> const&) const {
    return true;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      is_comparable_with<E, E_>,
      meta::is_less_comparable_with<E, E_>>,
  bool>
  operator<=(failure_t<E_> const& rhs) const {
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
  operator>(success_t<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    meta::is_comparable_with<E_, E>::value,
  bool>
  operator>(failure_t<E_> const& rhs) const {
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
  operator>=(success_t<T_> const&) const {
    return false;
  }

  template <class E_>
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      is_comparable_with<E_, E>,
      meta::is_less_comparable_with<E_, E>>,
  bool>
  operator>=(failure_t<E_> const& rhs) const {
    return rhs <= *this;
  }

  E& get() & { return x.get(); }
  E const& get() const& { return x.get(); }
  E& get() && { return x.get(); }

};

  template <class T, class... Args>
  class [[nodiscard]] failure_t<_result_detail::forward_mode<T>, Args...>
  {
    std::tuple<Args...> args;
  public:
    constexpr explicit failure_t(Args... args): args(std::forward<Args>(args)...) {}

    auto operator()() && {
      return std::apply([](auto&&... fwd){
        return std::forward_as_tuple(std::forward<decltype(fwd)>(fwd)...);
      }, args);
    }
  };

  template <class Target = void, class... Types>
  inline auto failure(Types&&... v) {
    if constexpr (!std::is_void_v<Target>) {
      if constexpr (sizeof...(Types) < 2)
        return failure_t<Target>{std::forward<Types>(v)...};
      else
        return failure_t<_result_detail::forward_mode<Target>, Types&&...>{std::forward<Types>(v)...};
    }
    else {
      if constexpr (sizeof...(Types) == 0)
        return failure_t<>{};
      else if constexpr (sizeof...(Types) == 1)
        return failure_t<Types...>{std::forward<Types>(v)...};
      else
        return failure_t<_result_detail::forward_mode<>, Types&&...>{std::forward<Types>(v)...};
    }
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
  template <class E>
  inline std::enable_if_t<trait::formattable<E>::value, std::ostream&>
  operator<<(std::ostream& os, failure_t<E> const& err) {
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
