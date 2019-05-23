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

namespace mitamagic
{

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

struct is_constraints
{
};

template <class, class...>
class Constraints;

template <class C1, class C2, std::enable_if_t<std::is_base_of_v<is_constraints, std::decay_t<C1>> && std::is_base_of_v<is_constraints, std::decay_t<C2>>> * = nullptr>
constexpr auto operator&(C1 &&c1, C2 &&c2)
{
  return Constraints<conjunction, C1, C2>{std::forward<C1>(c1), std::forward<C2>(c2)};
}
template <class C1, class C2, std::enable_if_t<std::is_base_of_v<is_constraints, std::decay_t<C1>> && std::is_base_of_v<is_constraints, std::decay_t<C2>>> * = nullptr>
constexpr auto operator|(C1 &&c1, C2 &&c2)
{
  return Constraints<disjunction, C1, C2>{std::forward<C1>(c1), std::forward<C2>(c2)};
}

template <class Supplier, class... Preds>
class Constraints : is_constraints
{
  std::tuple<Preds...> constraints;

public:
  constexpr Constraints(Preds... preds) : constraints{preds...} {}

  Constraints(Constraints const &) = default;
  Constraints &operator=(Constraints const &) = default;

  template <class Expected>
  constexpr bool operator[](Expected const &actual) const
  {
    return std::apply([actual](auto &&... preds) { return Supplier::apply(actual, std::forward<decltype(preds)>(preds)...); }, constraints);
  }
};

template <class Constraint, class Fallback>
class MatchExpression;

template <class Second, class First>
class MatchSequence;

template <class>
struct is_match_expr : std::false_type
{
};
template <class Co, class Ca>
struct is_match_expr<MatchExpression<Co, Ca>> : std::true_type
{
};

template <class T>
inline constexpr bool is_match_expr_v = is_match_expr<T>::value;

template <class>
struct is_match_sequential : std::false_type
{
};
template <class C, class F>
struct is_match_sequential<MatchSequence<C, F>> : std::true_type
{
};

template <class Constraint, class Fallback>
class MatchExpression
{
  Constraint constraint;
  mutable Fallback fallback;

public:
  constexpr MatchExpression(Constraint cons, Fallback cb) : constraint{cons}, fallback{cb} {}

  MatchExpression(MatchExpression const &) = default;
  MatchExpression &operator=(MatchExpression const &) = default;

  template <class... Args>
  constexpr bool operator[](Args&&... args) const
  {
    return constraint[std::tuple{std::forward<Args>(args)...}];
  }

  template <class... Args>
  constexpr decltype(auto) operator()(Args&&... args) const
  {
    if constexpr (std::is_invocable_v<Fallback>) {
      return std::invoke(fallback);
    }
    else if constexpr (std::is_invocable_v<Fallback, Args&&...>) {
      return std::invoke(fallback, std::forward<Args>(args)...);
    }
    else {
      static_assert([]{ return false; }(), "not invocable");
    }
  }
};

template <class Constraint, class Fallback>
MatchExpression(Constraint constraint, Fallback fallback)->MatchExpression<Constraint, Fallback>;

} // ! namespace mitamagic

struct MatchDefault : private mitamagic::is_constraints
{
  template <class... Dummy>
  constexpr bool operator[](Dummy &&...) const { return true; }
};

constexpr MatchDefault Default{};

namespace mitamagic {

template <class>
class MatchProxy;

template <class... MatchSequence>
class MatchProxy<std::tuple<MatchSequence...>>
{
  std::tuple<MatchSequence...> seq_;

public:
  template <class... Sequence>
  constexpr MatchProxy(Sequence... seq) : seq_{seq...} {}

  template <class... Args>
  constexpr auto operator()(Args&&... args) const
  {
    auto arg_pack = std::tuple(std::forward<Args>(args)...);
    return std::apply(
      [impl = [&](auto f, auto first, auto... rest)
                -> std::common_type_t<decltype(std::apply(first, arg_pack)), decltype(std::apply(rest, arg_pack))...>
    {
      if (first[arg_pack]) {
        return std::apply(first, arg_pack);
      }
      if constexpr (sizeof...(rest) > 0) {
        return f(f, rest...);
      }
      else {
        static_assert(
          std::disjunction_v<
            std::disjunction<std::is_same<MatchSequence, MatchDefault>...>,
            std::negation<std::is_void<std::common_type_t<decltype(std::apply(first, arg_pack)), decltype(std::apply(rest, arg_pack))...>>>>,
          "Error: non-exhaustive pattern"
        );
        if constexpr (std::is_void_v<std::common_type_t<decltype(std::apply(first, arg_pack)), decltype(std::apply(rest, arg_pack))...>>) {
          return;
        }
        else {
          throw std::runtime_error("reached to non-exhaustive path");
        }
      }
    }](auto... cases) mutable {
      return impl(impl, cases...);
    }, seq_);
  }
};

template <class... Args>
MatchProxy(Args... args)->MatchProxy<std::tuple<Args...>>;

template < class PackedArgs >
class ImmidiateMatchProxy {
  PackedArgs args_;
public:
  template < class... Args >
  constexpr ImmidiateMatchProxy(Args&&... args) noexcept : args_( std::forward<Args>(args)... ) {}

  template < class... Cases >
  decltype(auto) operator()(Cases&&... cases) const {
    return std::apply(MatchProxy<std::tuple<Cases...>>{ std::forward<Cases>(cases)... }, std::move(args_));
  }
};

template <class Constraints, class Fallback>
constexpr std::enable_if_t<std::conjunction_v<
                               std::is_base_of<is_constraints, std::decay_t<Constraints>>,
                               std::is_invocable<std::decay_t<Fallback>>>,
                           MatchExpression<Constraints, Fallback>>
operator>>=(Constraints &&co, Fallback &&ca)
{
  return {std::forward<Constraints>(co), std::forward<Fallback>(ca)};
}

struct match_builder {
  template <class Arg>
  constexpr auto
  operator[](Arg&& arg) const {
    if constexpr (mitamagic::is_tuple_like<std::decay_t<Arg>>::value) {
      return ImmidiateMatchProxy<std::decay_t<Arg>>{std::forward<Arg>(arg)};
    }
    else {
      return ImmidiateMatchProxy<std::tuple<Arg>>{std::forward<Arg>(arg)};
    }
  }

  template <class... Args>
  constexpr MatchProxy<std::tuple<Args...>>
  operator()(Args&&... args) const {
    return {std::forward<Args>(args)...};
  }

};


} // namespace mitamagic

template <class... Expected>
class Case : mitamagic::is_constraints
{
  std::tuple<Expected...> expected;

public:
  constexpr Case(Expected... values) : expected{values...} {}

  Case(Case const &) = default;
  Case &operator=(Case const &) = default;

  template <class Actual>
  constexpr bool operator[](Actual actual) const
  {
    return actual == expected;
  }
};

template <class Pred>
class Guard : private mitamagic::is_constraints
{
  Pred constraint;

public:
  constexpr Guard(Pred pred) : constraint{pred} {}

  template <class Actual>
  constexpr bool operator[](Actual&& actual) const
  {
    return std::apply(constraint, std::forward<Actual>(actual));
  }
};

template <class Pred>
Guard(Pred pred)->Guard<Pred>;

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