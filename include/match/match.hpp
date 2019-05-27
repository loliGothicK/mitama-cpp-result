#ifndef MITAMA_RESULT_MATCH
#define MITAMA_RESULT_MATCH

#include <tuple>
#include <type_traits>
#include <utility>
#include <functional>
#include <result/detail/fwd.hpp>

namespace mitama::match
{

struct suppress_non_exhaustive {};

inline constexpr auto _ = std::ignore;

using ignore_t = decltype(std::ignore);

namespace mitamagic
{

struct result_match_tag{};

template < class T >
inline constexpr bool is_result_match_v = std::is_base_of_v<result_match_tag, std::decay_t<T>>;

template < class, class, class = void >
struct is_comparable_with: std::false_type {};

template < class T, class U >
struct is_comparable_with<T, U, std::void_t<decltype(std::declval<T>() == std::declval<U>())>>: std::true_type {};

template < class T, class U >
inline constexpr bool is_comparable_with_v = is_comparable_with<T, U>::value;

namespace _detail {
  template < class E, class A >
  struct is_compatible_type
    : std::disjunction<
      is_comparable_with<E, A>,
      std::is_same<std::decay_t<E>, std::decay_t<ignore_t>>,
      std::conjunction<
        std::is_same<std::decay_t<E>, mitama::Ok<std::decay_t<ignore_t>>>,
        mitama::is_result<std::decay_t<A>>
      >,
      std::conjunction<
        std::is_same<std::decay_t<E>, mitama::Err<std::decay_t<ignore_t>>>,
        mitama::is_result<std::decay_t<A>>
      >
    >
  {};

  template < class, class, class >
  struct is_comparable_case;

  template < class... E, class... A, std::size_t... I >
  struct is_comparable_case<std::tuple<E...>, std::tuple<A...>, std::index_sequence<I...>>
    : std::conjunction<
        std::disjunction<
          is_compatible_type<
            std::tuple_element_t<I, std::tuple<E...>>,
            std::tuple_element_t<I, std::tuple<A...>>
          >...
        >
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
  template < class F, class G >
  static
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      std::is_invocable_r<bool, F>,
      std::is_invocable_r<bool, G>>,
  bool>
  Apply(F f, G g) {
    return f() && g();
  }
};

struct disjunction
{
  template < class F, class G >
  static
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      std::is_invocable_r<bool, F>,
      std::is_invocable_r<bool, G>>,
  bool>
  Apply(F f, G g) {
    return f() || g();
  }
};

struct is_constraints {};

template <class, class, class>
class Constraints;

template <class C1, class C2,
    std::enable_if_t<std::conjunction_v<
      std::is_base_of<is_constraints, std::decay_t<C1>>,
      std::is_base_of<is_constraints, std::decay_t<C2>>>,
    bool> = false>
constexpr auto operator&(C1 c1, C2 c2)
{
  return Constraints<conjunction, C1, C2>{c1, c2};
}
template <class C1, class C2,
    std::enable_if_t<std::conjunction_v<
      std::is_base_of<is_constraints, std::decay_t<C1>>,
      std::is_base_of<is_constraints, std::decay_t<C2>>>,
    bool> = false>
constexpr auto operator|(C1 c1, C2 c2)
{
  return Constraints<disjunction, C1, C2>{c1, c2};
}

template <class Supplier, class Pre, class Post>
class Constraints : is_constraints
{
  Pre pre_fn;
  Post post_fn;
public:
  constexpr Constraints(Pre a, Post b): pre_fn(a), post_fn(b) {}

  Constraints(Constraints const &) = default;
  Constraints &operator=(Constraints const &) = default;

  template <class... Expected>
  constexpr
  std::enable_if_t<
    std::conjunction_v<
      is_invocable_constrait<Pre, Expected...>,
      is_invocable_constrait<Post, Expected...>>,
  bool>
  operator[](std::tuple<Expected...> const &actual) const
  {
    return Supplier::Apply(
      [&actual, this]{ return std::apply(pre_fn, actual); },
      [&actual, this]{ return std::apply(post_fn, actual); }
    );
  }
};

template <class Second, class First>
class MatchSequence;

template <class Constraint, class Fallback, class = void>
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
  constexpr auto operator()(Args... args) const
  {
    if constexpr (std::is_invocable_v<Fallback, Args...>) {
      return std::invoke(fallback, args...);
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
class MatchExpression<Constraint, Fallback, std::enable_if_t<is_result_match_v<Constraint>>>
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

  template <class Res>
  constexpr auto operator()(Res const& res) const
  {
    if constexpr (Constraint::ok) {
      if constexpr (std::is_invocable_v<Fallback, typename Res::ok_type>) {
        return std::invoke(fallback, res.unwrap());
      }
      else if constexpr (std::is_invocable_v<Fallback>) {
        return std::invoke(fallback);
      }
      else {
        static_assert([]{ return false; }(), "not invocable");
      }
    }
    else {
      if constexpr (std::is_invocable_v<Fallback, typename Res::err_type>) {
        return std::invoke(fallback, res.unwrap_err());
      }
      else if constexpr (std::is_invocable_v<Fallback>) {
        return std::invoke(fallback);
      }
      else {
        static_assert([]{ return false; }(), "not invocable");
      }
    }
  }
};

template <class Constraint, class Fallback>
MatchExpression(Constraint, Fallback) -> MatchExpression<std::decay_t<Constraint>, std::decay_t<Fallback>>;

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
  constexpr MatchProxy(Sequence... seq): seq_{seq...} {}

  template <class... Args>
  constexpr auto operator()(Args const&... args) const
  {
    static_assert(
      std::conjunction_v<is_invocable_constrait<MatchSequence, Args...>...>,
      "Error: non-invocable match arm(s) exist.");
    return std::apply(
      [impl = [&, args...](auto f, auto first, auto... rest)
          -> std::common_type_t<decltype(std::apply(first, std::declval<std::tuple<Args...>>())), decltype(std::apply(rest, std::declval<std::tuple<Args...>>()))...>
    {
      if (first[std::tuple(args...)]) {
        return std::apply(first, std::tuple(args...));
      }
      if constexpr (sizeof...(rest) > 0) {
        return f(f, rest...);
      }
      else {
        if constexpr (std::is_void_v<std::common_type_t<decltype(std::apply(first, std::tuple(args...))), decltype(std::apply(rest, std::tuple(args...)))...>>) {
          return;
        }
        else {
          throw std::runtime_error("reached to non-exhaustive path");
        }
      }
    }](auto... cases) {
      return impl(impl, cases...);
    }, seq_);
  }
};

template < class >
class ImmediateMatchProxy;

template < class... Args >
class ImmediateMatchProxy<std::tuple<Args...>> {
  std::tuple<Args...> args_;
public:
  template < class... Args_ >
  constexpr ImmediateMatchProxy(Args_... args) noexcept : args_( args... ) {}

  template < class... Cases >
  auto operator()(Cases... cases) const {
    return std::apply(MatchProxy<std::tuple<Cases...>>{ cases... }, args_);
  }
};

template <class Constraints, class Fallback>
constexpr std::enable_if_t<std::is_base_of_v<is_constraints, std::decay_t<Constraints>>,
                           MatchExpression<std::decay_t<Constraints>, std::decay_t<Fallback>>>
operator>>=(Constraints co, Fallback fb)
{
  return {co, fb};
}

template <class Constraints, class FallbackValue,
    std::enable_if_t<
      std::is_base_of_v<is_constraints, std::decay_t<Constraints>>,
    bool> = false>
constexpr auto
operator<<=(Constraints co, FallbackValue fv)
{
  return MatchExpression{co, [value = fv]{ return value; }};
}


class match_builder {
public:
  template <class Arg>
  constexpr auto
  operator[](Arg arg) const {
    if constexpr (mitamagic::is_tuple_like<std::decay_t<Arg>>::value) {
      return ImmediateMatchProxy<std::decay_t<Arg>>{arg};
    }
    else {
      return ImmediateMatchProxy<std::tuple<std::decay_t<Arg>>>{arg};
    }
  }

  template <class... Args>
  constexpr MatchProxy<std::tuple<Args...>>
  operator()(Args... args) const {
    return {args...};
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
      else if constexpr (std::conjunction_v<std::is_same<mitama::Ok<std::decay_t<ignore_t>>, std::decay_t<decltype(lhs)>>, 
                                            mitama::result::is_result<std::decay_t<decltype(rhs)>>>) {
        return rhs.is_ok();
      }
      else if constexpr (std::conjunction_v<std::is_same<mitama::Err<std::decay_t<ignore_t>>, std::decay_t<decltype(lhs)>>, 
                                            mitama::result::is_result<std::decay_t<decltype(rhs)>>>) {
        return rhs.is_err();
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

template <class T>
class Case<Ok<T>>
  : mitamagic::is_constraints
  , mitamagic::result_match_tag
{
  Ok<T> expected;

public:
  explicit constexpr Case(Ok<T> expected) : expected{expected} {}
  static constexpr bool ok = true;

  Case(Case const &) = default;
  Case &operator=(Case const &) = default;

  template < class U, class E >
  constexpr 
  std::enable_if_t<
    std::is_same_v<std::decay_t<ignore_t>, std::decay_t<T>> ||
    mitamagic::is_comparable_with<T, U>::value,
  bool>
  operator[](std::tuple<mitama::Result<U, E>> const& res) const {
    if constexpr (std::is_same_v<std::decay_t<ignore_t>, std::decay_t<T>>) {
      return std::get<0>(res).is_ok();
    }
    else {
      return expected == std::get<0>(res);
    }
  }
};

template <class E>
class Case<Err<E>>
  : mitamagic::is_constraints
  , mitamagic::result_match_tag
{
  Err<E> expected;

public:
  explicit constexpr Case(Err<E> expected) : expected{expected} {}
  static constexpr bool ok = false;

  Case(Case const &) = default;
  Case &operator=(Case const &) = default;

  template < class T, class F >
  constexpr 
  std::enable_if_t<
    std::is_same_v<std::decay_t<ignore_t>, std::decay_t<E>> ||
    mitamagic::is_comparable_with<E, F>::value,
  bool>
  operator[](std::tuple<mitama::Result<T, F>> const& res) const {
    if constexpr (std::is_same_v<std::decay_t<ignore_t>, std::decay_t<E>>) {
      return std::get<0>(res).is_err();
    }
    else {
      return expected == std::get<0>(res);
    }
  }
};

namespace mitamagic {
template <class Supplier, class T, class Post>
class Constraints<Supplier, Case<mitama::Ok<T>>, Post>
  : is_constraints
  , result_match_tag
{
  Case<mitama::Ok<T>> ok_case;
  Post post_fn;
public:
  constexpr Constraints(Case<mitama::Ok<T>> a, Post b): ok_case(a), post_fn(b) {}
  static constexpr bool ok = true;

  Constraints(Constraints const &) = default;
  Constraints &operator=(Constraints const &) = default;

  template < class U, class E >
  constexpr 
  std::enable_if_t<
    std::is_same_v<std::decay_t<ignore_t>, std::decay_t<T>> ||
    mitamagic::is_comparable_with<T, U>::value,
  bool>
  operator[](std::tuple<mitama::Result<U, E>> const &actual) const
  {
    Supplier::Apply(
      [&actual, this]{ return ok_case[std::get<0>(actual)]; },
      [&actual, this]{ return std::apply(post_fn, std::get<0>(actual).unwrap()); }
    );
  }
};
template <class Supplier, class E, class Post>
class Constraints<Supplier, Case<mitama::Err<E>>, Post>
  : is_constraints
  , result_match_tag
{
  Case<mitama::Err<E>> err_case;
  Post post_fn;
public:
  constexpr Constraints(Case<mitama::Err<E>> a, Post b): err_case(a), post_fn(b) {}
  static constexpr bool ok = false;

  Constraints(Constraints const &) = default;
  Constraints &operator=(Constraints const &) = default;

  template < class T, class F >
  constexpr 
  std::enable_if_t<
    std::is_same_v<std::decay_t<ignore_t>, std::decay_t<E>> ||
    mitamagic::is_comparable_with<E, F>::value,
  bool>
  operator[](std::tuple<mitama::Result<T, F>> const &actual) const
  {
    Supplier::Apply(
      [&actual, this]{ return err_case[std::get<0>(actual)]; },
      [&actual, this]{ return std::apply(post_fn, std::get<0>(actual).unwrap_err()); }
    );
  }
};
}

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
