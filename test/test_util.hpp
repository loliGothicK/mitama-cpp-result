#ifndef MITAMA_DIMENSIONAL_TEST_UTIL_HPP
#define MITAMA_DIMENSIONAL_TEST_UTIL_HPP

#include <type_traits>
#include <utility>
#include <tuple>
#include <random>
#include <catch2/catch.hpp>

#define IS_INVALID_EXPR(...)                                     \
  IS_INVALID_EXPR_IMPL1(__VA_ARGS__)                             \
  IS_INVALID_EXPR_IMPL2

#define IS_INVALID_EXPR_IMPL1(...) bool(false ? ::mitama::make_overload([](auto x)->decltype(__VA_ARGS__, std::false_type{}){return{};},[](...)->std::true_type{return{};})
#define IS_INVALID_EXPR_IMPL2(...) (::mitama::type_transfer<__VA_ARGS__>{}): ::mitama::protean_bool{})
#define DECLVAL(N) std::declval<std::tuple_element_t<N,typename decltype(x)::type>>()
#define DECLTYPE(N) std::tuple_element_t<N,typename decltype(x)::type>

namespace test_util {
  template < class ValueType >
  class RandomGenerator {
    ValueType lower, upper;
    mutable std::size_t limit;

    template < class Pred, class Dist, class Engine, std::size_t... Indices >
    static auto invoke(Pred&& pred, Dist& dist, Engine& engine, std::index_sequence<Indices...>) -> bool {
      return std::forward<Pred>(pred)((void(Indices), dist(engine))...);
    }

  public:
    RandomGenerator(ValueType a, ValueType b): lower(a), upper(b) {}

    static constexpr auto uniform(ValueType a, ValueType b) -> RandomGenerator {
      return RandomGenerator{a, b};
    }

    auto& take(std::size_t lim) {
      limit = lim;
      return *this;
    }

    template < int N = 1, class Pred >
    auto required(Pred&& pred) const {
      bool res = true;
      std::mt19937_64 mt(std::random_device{}());
      if constexpr (std::is_integral_v<ValueType>){
        std::uniform_int_distribution<ValueType> dist(lower, upper);
        while (bool(limit--) && res) {
          res = invoke(std::forward<Pred>(pred), dist, mt, std::make_index_sequence<N>{});
        }
        return res;
      }
      else if constexpr (std::is_floating_point_v<ValueType>){
        std::uniform_real_distribution<ValueType> dist(lower, upper);
        while (bool(limit--) && res) {
          res = invoke(std::forward<Pred>(pred), dist, mt, std::make_index_sequence<N>{});
        }
        return res;
      }
      else {
        return false;
      }
    }

  };
}

#endif
