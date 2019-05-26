#ifndef MITAMA_RESULT_MATCH
#define MITAMA_RESULT_MATCH

#include <tuple>
#include <type_traits>
#include <utility>
#include <functional>
#include <result/detail/fwd.hpp>
#include <result/detail/detail.hpp>

namespace mitama::match
{

struct suppress_non_exhaustive {};

inline constexpr auto _ = std::ignore;

using ignore_t = decltype(std::ignore);

namespace mitamagic
{

template < class, class, class = void >
struct is_comparable_with: std::false_type {};

template < class T, class U >
struct is_comparable_with<T, U, std::void_t<decltype(std::declval<T>() == std::declval<U>())>>: std::true_type {};

template < class T, class U >
inline constexpr bool is_comparable_with_v = is_comparable_with<T, U>::value;

namespace _detail {
  template < class, class, class >
  struct is_comparable_case;

  template < class... E, class... A, std::size_t... I >
  struct is_comparable_case<std::tuple<E...>, std::tuple<A...>, std::index_sequence<I...>>
    : std::conjunction<
        std::disjunction<
          is_comparable_with<
            std::tuple_element_t<I, std::tuple<E...>>,
            std::tuple_element_t<I, std::tuple<A...>>
          >,
          std::is_same<
            std::decay_t<ignore_t>,
            std::decay_t<std::tuple_element_t<I, std::tuple<E...>>>
          >
        >...
      >
  {};
}

template < class, class >
struct is_comparable_case {};

template < class... E, class... A >
struct is_comparable_case<std::tuple<E...>, std::tuple<A...>>
  : _detail::is_comparable_case<std::tuple<E...>, std::tuple<A...>, std::index_sequence_for<E...>> {};

namespace _detail {
template <typename AlwaysVoid, typename, typename...>
struct is_invocable_constrait: std::false_type {};

template <typename F, typename...Args>
struct is_invocable_constrait<
    std::enable_if_t<
      std::is_same_v<bool, 
      decltype(std::declval<const F&>()[std::declval<std::tuple<Args...>>()])>>,
    F, Args...>
  : std::true_type
{};

}

template <class F, class... ArgTypes>
struct is_invocable_constrait : _detail::is_invocable_constrait<void, F, ArgTypes...> {};

template <class F, class... ArgTypes>
inline constexpr bool is_invocable_constrait_v = is_invocable_constrait<F, ArgTypes...>::value;

template < class, class, class = void >
struct is_tuple_like_impl;

template < class T, std::size_t... Indices >
struct is_tuple_like_impl<T, std::index_sequence<Indices...>, std::void_t<std::tuple_element_t<Indices, T>...>>: std::true_type {};

template < class, class = void >
struct is_tuple_like: std::false_type {};

template < class T >
struct is_tuple_like<T, std::void_t<decltype(std::tuple_size_v<T>)>> : is_tuple_like_impl<T, std::make_index_sequence<std::tuple_size_v<T>>> {};

struct conjunction
{
  template <class Actual, class... Preds>
  static constexpr bool apply(Actual const &act, Preds &&... preds)
  {
    return (std::forward<Preds>(preds)[act] && ...);
  }
};

struct disjunction
{
  template <class Actual, class... Preds>
  static constexpr bool apply(Actual const &act, Preds &&... preds)
  {
    return (std::forward<Preds>(preds)[act] || ...);
  }
};

struct is_constraints {};

template <class, class...>
class Constraints;

template <class C1, class C2,
    std::enable_if_t<std::conjunction_v<
      std::is_base_of<is_constraints, std::decay_t<C1>>,
      std::is_base_of<is_constraints, std::decay_t<C2>>>,
    bool> = false>
constexpr auto operator&(C1 &&c1, C2 &&c2)
{
  return Constraints<conjunction, C1, C2>{std::forward<C1>(c1), std::forward<C2>(c2)};
}
template <class C1, class C2,
    std::enable_if_t<std::conjunction_v<
      std::is_base_of<is_constraints, std::decay_t<C1>>,
      std::is_base_of<is_constraints, std::decay_t<C2>>>,
    bool> = false>
constexpr auto operator|(C1 &&c1, C2 &&c2)
{
  return Constraints<disjunction, C1, C2>{std::forward<C1>(c1), std::forward<C2>(c2)};
}

template <class Supplier, class... Preds>
class Constraints : is_constraints
{
  std::tuple<Preds...> constraints;

public:
  constexpr Constraints(Preds... preds): constraints{preds...} {}

  Constraints(Constraints const &) = default;
  Constraints &operator=(Constraints const &) = default;

  template <class... Expected>
  constexpr
  std::enable_if_t<
    std::conjunction_v<is_invocable_constrait<Preds, Expected...>...>,
  bool>
  operator[](std::tuple<Expected...> const &actual) const
  {
    return std::apply([&actual](auto&&... preds) {
      return Supplier::apply(actual, std::forward<decltype(preds)>(preds)...);
    }, constraints);
  }
};

template <class Constraint, class Fallback>
class MatchExpression;

template <class Second, class First>
class MatchSequence;

template <class>
struct is_match_expr: std::false_type {};

template <class C, class F>
struct is_match_expr<MatchExpression<C, F>>: std::true_type {};

template <class T>
inline constexpr bool is_match_expr_v = is_match_expr<T>::value;

template <class Constraint, class Fallback>
class MatchExpression
{
  Constraint constraint;
  mutable std::decay_t<Fallback> fallback;

public:
  constexpr MatchExpression(Constraint cons, Fallback cb): constraint{cons}, fallback{cb} {}

  MatchExpression(MatchExpression const &) = default;
  MatchExpression &operator=(MatchExpression const &) = default;

  template <class... Args>
  constexpr
  std::enable_if_t<
    is_invocable_constrait_v<Constraint, Args...>,
  bool>
  operator[](std::tuple<Args...> const& args) const
  {
    return constraint[args];
  }

  template <class... Args>
  constexpr decltype(auto) operator()(Args&&... args) const
  {
    if constexpr (std::is_invocable_v<Fallback, Args&&...>) {
      return std::invoke(fallback, std::forward<Args>(args)...);
    }
    else if constexpr (std::is_invocable_v<Fallback>) {
      return std::invoke(fallback);
    }
    else {
      static_assert([]{ return false; }(), "not invocable");
    }
  }
};

template <class Constraint, class Fallback>
MatchExpression(Constraint, Fallback) -> MatchExpression<Constraint, Fallback>;

} // ! namespace mitamagic

struct MatchDefault : private mitamagic::is_constraints
{
  template <class... Dummy>
  constexpr bool operator[](Dummy &&...) const { return true; }
};

constexpr MatchDefault Default{};

namespace mitamagic {

template <class, class>
class MatchProxy;

template <class SuppressNonExhaustive, class... MatchSequence>
class MatchProxy<SuppressNonExhaustive, std::tuple<MatchSequence...>>
{
  std::tuple<MatchSequence...> seq_;

public:
  template <class... Sequence>
  constexpr MatchProxy(Sequence... seq): seq_{seq...} {}

  template <class... Args>
  constexpr auto operator()(Args&&... args) const
  {
    auto arg_pack = std::tuple<Args...>{ std::forward<Args>(args)... };
    return std::apply(
      [impl = [&](auto f, auto first, auto... rest)
          -> std::common_type_t<decltype(std::apply(first, arg_pack)), decltype(std::apply(rest, arg_pack))...>
    {
      if constexpr (is_invocable_constrait_v<decltype(first), Args...>) {
        if (first[arg_pack]) {
          return std::apply(first, arg_pack);
        }
      }
      if constexpr (sizeof...(rest) > 0) {
        return f(f, rest...);
      }
      else {
        if constexpr (!std::is_same_v<SuppressNonExhaustive, suppress_non_exhaustive>) {
          static_assert(
            std::disjunction_v<
              std::disjunction<std::is_same<MatchSequence, MatchDefault>...>,
              std::negation<std::is_void<std::common_type_t<decltype(std::apply(first, arg_pack)), decltype(std::apply(rest, arg_pack))...>>>>,
            "Error: non-exhaustive pattern"
          );
          throw std::runtime_error("reached to non-exhaustive path");
        }
        else {
          if constexpr (std::is_void_v<std::common_type_t<decltype(std::apply(first, arg_pack)), decltype(std::apply(rest, arg_pack))...>>) {
            return;
          }
          else {
            throw std::runtime_error("reached to non-exhaustive path");
          }
        }
      }
    }](auto... cases) {
      return impl(impl, cases...);
    }, seq_);
  }
};

template < class, class >
class ImmediateMatchProxy;

template < class SuppressNonExhaustive, class... Args >
class ImmediateMatchProxy<SuppressNonExhaustive, std::tuple<Args...>> {
  std::tuple<Args...> args_;
public:
  template < class... Args_ >
  constexpr ImmediateMatchProxy(Args_&&... args) noexcept : args_( std::forward<Args_>(args)... ) {}

  template < class... Cases >
  decltype(auto) operator()(Cases&&... cases) const {
    return std::apply(MatchProxy<SuppressNonExhaustive, std::tuple<Cases...>>{ std::forward<Cases>(cases)... }, std::move(args_));
  }
};

template <class Constraints, class Fallback>
constexpr std::enable_if_t<std::is_base_of_v<is_constraints, std::decay_t<Constraints>>,
                           MatchExpression<Constraints, Fallback>>
operator>>=(Constraints &&co, Fallback &&ca)
{
  return {std::forward<Constraints>(co), std::forward<Fallback>(ca)};
}

class match_builder {
public:
  template <class Arg>
  constexpr auto
  operator[](Arg&& arg) const {
    if constexpr (mitamagic::is_tuple_like<std::decay_t<Arg>>::value) {
      return ImmediateMatchProxy<void, std::decay_t<Arg>>{std::forward<Arg>(arg)};
    }
    else {
      return ImmediateMatchProxy<void, std::tuple<std::decay_t<Arg>>>{std::forward<Arg>(arg)};
    }
  }

  template <class... Args>
  constexpr MatchProxy<void, std::tuple<Args...>>
  operator()(Args&&... args) const {
    return {std::forward<Args>(args)...};
  }
};


} // namespace mitamagic

template <class... Expected>
class Case : mitamagic::is_constraints
{
  std::tuple<Expected...> expected;

  template < class E, class A, std::size_t... I >
  constexpr
  std::enable_if_t<
    mitamagic::is_comparable_case<E, A>::value,
  bool>
  compare(E const& e, A const& a, std::index_sequence<I...>) const {
    return ([&](auto const& lhs, auto const& rhs){
      if constexpr (std::is_same_v<ignore_t, std::remove_reference_t<decltype(lhs)>>) {
        return true;
      }
      else {
        return lhs == rhs;
      }
    }(std::get<I>(e), std::get<I>(a)) && ...);
  }
public:
  explicit constexpr Case(Expected... values) : expected{values...} {}

  Case(Case const &) = default;
  Case &operator=(Case const &) = default;

  template < class... Actual >
  constexpr
  std::enable_if_t<
    mitamagic::is_comparable_case<std::tuple<Expected...>, std::tuple<Actual...>>::value,
  bool>
  operator[](std::tuple<Actual...> const& actual) const
  {
    return compare(expected, actual, std::index_sequence_for<Expected...>{});
  }
};

template <class Pred>
class Guard : private mitamagic::is_constraints
{
  Pred constraint;

public:
  constexpr Guard(Pred pred) : constraint{pred} {}

  template <class... Actual>
  constexpr
  std::enable_if_t<
    std::is_invocable_r_v<bool, Pred, Actual...>,
  bool>
  operator[](std::tuple<Actual...> const& actual) const
  {
    return std::apply(constraint, actual);
  }
};

template <class Pred>
Guard(Pred pred) -> Guard<Pred>;

template < class T >
struct Range {
  T lower, upper;

  constexpr bool operator()(T value) const {
    return lower <= value && value < upper;
  }
};

template < class T > Range(T, T) -> Range<T>;

inline constexpr auto range = [](auto lower, auto upper){ return Range{lower, upper}; };

inline constexpr mitamagic::match_builder match{};

} // namespace mitama::match

#endif