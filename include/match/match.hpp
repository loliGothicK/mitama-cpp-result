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

struct conjunction
{
  template <class Actual, class... Preds>
  static constexpr bool apply(Actual const &act, Preds &&... preds)
  {
    return (std::forward<Preds>(preds)(act) && ...);
  }
};

struct disjunction
{
  template <class Actual, class... Preds>
  static constexpr bool apply(Actual const &act, Preds &&... preds)
  {
    return (std::forward<Preds>(preds)(act) || ...);
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
  constexpr bool operator()(Expected const &actual) const
  {
    return std::apply([actual](auto &&... preds) { return Supplier::apply(actual, std::forward<decltype(preds)>(preds)...); }, constraints);
  }
};

template <class Constraint, class CallBack>
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
template <class Co, class Ca>
struct is_match_sequential<MatchSequence<Co, Ca>> : std::true_type
{
};

template <class Constraint, class CallBack>
class MatchExpression
{
  Constraint constraint;
  mutable CallBack call_back;

public:
  using result_type = std::invoke_result_t<CallBack>;

  constexpr MatchExpression(Constraint cons, CallBack cb) : constraint{cons}, call_back{cb} {}

  MatchExpression(MatchExpression const &) = default;
  MatchExpression &operator=(MatchExpression const &) = default;

  template <class Tuple>
  constexpr bool operator()(Tuple const &tup) const
  {
    return std::apply(constraint, tup);
  }

  constexpr result_type result() const
  {
    return call_back();
  }
};

template <class Constraint, class CallBack>
MatchExpression(Constraint constraint, CallBack call_back)->MatchExpression<Constraint, CallBack>;

template <class Tuple>
class MatchProxy
{
  Tuple args_;

public:
  template <class... Args>
  constexpr MatchProxy(Args... args) : args_{args...} {}

  template <class... MatchSequence>
  constexpr auto operator()(MatchSequence &&... seq) const
  {
    return [impl = [this](auto f, auto first, auto... rest)
                -> std::common_type_t<typename decltype(first)::result_type, typename decltype(rest)::result_type...> {
      if (first(args_))
      {
        return first.result();
      }
      if constexpr (sizeof...(rest) > 0)
      {
        return f(f, rest...);
      }
      else
      {
        if constexpr (!std::is_void_v<std::common_type_t<typename decltype(first)::result_type, typename decltype(rest)::result_type...>>)
        {
          static_assert(dependent_bool::always_false_v<std::common_type_t<typename decltype(first)::result_type, typename decltype(rest)::result_type...>>,
              "Error: non-void match expression without default case");
        }
      }
    }](auto... args) {
      return impl(impl, args...);
    }(std::forward<MatchSequence>(seq)...);
  }
};

template <class... Args>
MatchProxy(Args... args)->MatchProxy<std::tuple<Args...>>;

template <class Constraints, class CallBack>
constexpr std::enable_if_t<std::conjunction_v<
                               std::is_base_of<is_constraints, std::decay_t<Constraints>>,
                               std::is_invocable<std::decay_t<CallBack>>>,
                           MatchExpression<Constraints, CallBack>>
operator>>=(Constraints &&co, CallBack &&ca)
{
  return {std::forward<Constraints>(co), std::forward<CallBack>(ca)};
}

} // namespace mitamagic

template <class>
class Case;

template <class... Expected>
class Case<std::tuple<Expected...>> : mitamagic::is_constraints
{
  std::tuple<Expected...> expected;

public:
  constexpr Case(Expected... values) : expected{values...} {}

  Case(Case const &) = default;
  Case &operator=(Case const &) = default;

  template <class... Actuals>
  constexpr bool operator()(Actuals const &... actuals) const
  {
    return std::tie(actuals...) == expected;
  }
};

template <class... Expected>
Case(Expected... expected)->Case<std::tuple<Expected...>>;

template <class Pred>
class Guard : private mitamagic::is_constraints
{
  Pred constraint;

public:
  constexpr Guard(Pred pred) : constraint{pred} {}

  template <class Actual>
  constexpr bool operator()(Actual &&actual) const
  {
    return constraint(std::forward<Actual>(actual));
  }
};

template <class Pred>
Guard(Pred pred)->Guard<Pred>;

struct MatchDefault : private mitamagic::is_constraints
{
  template <class... Dummy>
  constexpr bool operator()(Dummy &&...) const { return true; }
};

constexpr MatchDefault Default{};

template <class... Args>
constexpr mitamagic::MatchProxy<std::tuple<Args...>> match(Args... args) { return {args...}; }

} // namespace mitama::match

#endif