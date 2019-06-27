#include "TestClass.hpp"
#include <type_traits>

static_assert(std::is_copy_constructible_v<success<TestClass<true, true, true, true>>> == true);
static_assert(std::is_copy_assignable_v<success<TestClass<true, true, true, true>>> == true);
static_assert(std::is_move_constructible_v<success<TestClass<true, true, true, true>>> == true);
static_assert(std::is_move_assignable_v<success<TestClass<true, true, true, true>>> == true);

static_assert(std::is_copy_constructible_v<success<TestClass<true, true, true, false>>> == true);
static_assert(std::is_copy_assignable_v<success<TestClass<true, true, true, false>>> == true);
static_assert(std::is_move_constructible_v<success<TestClass<true, true, true, false>>> == true);
static_assert(std::is_move_assignable_v<success<TestClass<true, true, true, false>>> == true);

static_assert(std::is_copy_constructible_v<success<TestClass<true, false, true, true>>> == true);
static_assert(std::is_copy_assignable_v<success<TestClass<true, false, true, true>>> == false);
static_assert(std::is_move_constructible_v<success<TestClass<true, false, true, true>>> == true);
static_assert(std::is_move_assignable_v<success<TestClass<true, false, true, true>>> == true);

static_assert(std::is_copy_constructible_v<success<TestClass<true, false, true, false>>> == true);
static_assert(std::is_copy_assignable_v<success<TestClass<true, false, true, false>>> == false);
static_assert(std::is_move_constructible_v<success<TestClass<true, false, true, false>>> == true);
static_assert(std::is_move_assignable_v<success<TestClass<true, false, true, false>>> == false);

static_assert(std::is_copy_constructible_v<success<TestClass<true, true, false, true>>> == true);
static_assert(std::is_copy_assignable_v<success<TestClass<true, true, false, true>>> == true);
static_assert(std::is_move_constructible_v<success<TestClass<true, true, false, true>>> == true);
static_assert(std::is_move_assignable_v<success<TestClass<true, true, false, true>>> == true);

static_assert(std::is_copy_constructible_v<success<TestClass<true, true, false, false>>> == true);
static_assert(std::is_copy_assignable_v<success<TestClass<true, true, false, false>>> == true);
static_assert(std::is_move_constructible_v<success<TestClass<true, true, false, false>>> == true);
static_assert(std::is_move_assignable_v<success<TestClass<true, true, false, false>>> == true);

static_assert(std::is_copy_constructible_v<success<TestClass<true, false, false, true>>> == true);
static_assert(std::is_copy_assignable_v<success<TestClass<true, false, false, true>>> == false);
static_assert(std::is_move_constructible_v<success<TestClass<true, false, false, true>>> == true);
static_assert(std::is_move_assignable_v<success<TestClass<true, false, false, true>>> == false);

static_assert(std::is_copy_constructible_v<success<TestClass<true, false, false, false>>> == true);
static_assert(std::is_copy_assignable_v<success<TestClass<true, false, false, false>>> == false);
static_assert(std::is_move_constructible_v<success<TestClass<true, false, false, false>>> == true);
static_assert(std::is_move_assignable_v<success<TestClass<true, false, false, false>>> == false);

static_assert(std::is_copy_constructible_v<success<TestClass<false, true, true, true>>> == false);
static_assert(std::is_copy_assignable_v<success<TestClass<false, true, true, true>>> == false);
static_assert(std::is_move_constructible_v<success<TestClass<false, true, true, true>>> == true);
static_assert(std::is_move_assignable_v<success<TestClass<false, true, true, true>>> == true);

static_assert(std::is_copy_constructible_v<success<TestClass<false, true, true, false>>> == false);
static_assert(std::is_copy_assignable_v<success<TestClass<false, true, true, false>>> == false);
static_assert(std::is_move_constructible_v<success<TestClass<false, true, true, false>>> == true);
static_assert(std::is_move_assignable_v<success<TestClass<false, true, true, false>>> == false);

static_assert(std::is_copy_constructible_v<success<TestClass<false, false, true, true>>> == false);
static_assert(std::is_copy_assignable_v<success<TestClass<false, false, true, true>>> == false);
static_assert(std::is_move_constructible_v<success<TestClass<false, false, true, true>>> == true);
static_assert(std::is_move_assignable_v<success<TestClass<false, false, true, true>>> == true);

static_assert(std::is_copy_constructible_v<success<TestClass<false, false, true, false>>> == false);
static_assert(std::is_copy_assignable_v<success<TestClass<false, false, true, false>>> == false);
static_assert(std::is_move_constructible_v<success<TestClass<false, false, true, false>>> == true);
static_assert(std::is_move_assignable_v<success<TestClass<false, false, true, false>>> == false);

static_assert(std::is_copy_constructible_v<success<TestClass<false, true, false, true>>> == false);
static_assert(std::is_copy_assignable_v<success<TestClass<false, true, false, true>>> == false);
static_assert(std::is_move_constructible_v<success<TestClass<false, true, false, true>>> == false);
static_assert(std::is_move_assignable_v<success<TestClass<false, true, false, true>>> == false);

static_assert(std::is_copy_constructible_v<success<TestClass<false, true, false, false>>> == false);
static_assert(std::is_copy_assignable_v<success<TestClass<false, true, false, false>>> == false);
static_assert(std::is_move_constructible_v<success<TestClass<false, true, false, false>>> == false);
static_assert(std::is_move_assignable_v<success<TestClass<false, true, false, false>>> == false);

static_assert(std::is_copy_constructible_v<success<TestClass<false, false, false, true>>> == false);
static_assert(std::is_copy_assignable_v<success<TestClass<false, false, false, true>>> == false);
static_assert(std::is_move_constructible_v<success<TestClass<false, false, false, true>>> == false);
static_assert(std::is_move_assignable_v<success<TestClass<false, false, false, true>>> == false);

static_assert(std::is_copy_constructible_v<success<TestClass<false, false, false, false>>> == false);
static_assert(std::is_copy_assignable_v<success<TestClass<false, false, false, false>>> == false);
static_assert(std::is_move_constructible_v<success<TestClass<false, false, false, false>>> == false);
static_assert(std::is_move_assignable_v<success<TestClass<false, false, false, false>>> == false);

static_assert(std::is_copy_constructible_v<failure<TestClass<true, true, true, true>>> == true);
static_assert(std::is_copy_assignable_v<failure<TestClass<true, true, true, true>>> == true);
static_assert(std::is_move_constructible_v<failure<TestClass<true, true, true, true>>> == true);
static_assert(std::is_move_assignable_v<failure<TestClass<true, true, true, true>>> == true);

static_assert(std::is_copy_constructible_v<failure<TestClass<true, true, true, false>>> == true);
static_assert(std::is_copy_assignable_v<failure<TestClass<true, true, true, false>>> == true);
static_assert(std::is_move_constructible_v<failure<TestClass<true, true, true, false>>> == true);
static_assert(std::is_move_assignable_v<failure<TestClass<true, true, true, false>>> == true);

static_assert(std::is_copy_constructible_v<failure<TestClass<true, false, true, true>>> == true);
static_assert(std::is_copy_assignable_v<failure<TestClass<true, false, true, true>>> == false);
static_assert(std::is_move_constructible_v<failure<TestClass<true, false, true, true>>> == true);
static_assert(std::is_move_assignable_v<failure<TestClass<true, false, true, true>>> == true);

static_assert(std::is_copy_constructible_v<failure<TestClass<true, false, true, false>>> == true);
static_assert(std::is_copy_assignable_v<failure<TestClass<true, false, true, false>>> == false);
static_assert(std::is_move_constructible_v<failure<TestClass<true, false, true, false>>> == true);
static_assert(std::is_move_assignable_v<failure<TestClass<true, false, true, false>>> == false);

static_assert(std::is_copy_constructible_v<failure<TestClass<true, true, false, true>>> == true);
static_assert(std::is_copy_assignable_v<failure<TestClass<true, true, false, true>>> == true);
static_assert(std::is_move_constructible_v<failure<TestClass<true, true, false, true>>> == true);
static_assert(std::is_move_assignable_v<failure<TestClass<true, true, false, true>>> == true);

static_assert(std::is_copy_constructible_v<failure<TestClass<true, true, false, false>>> == true);
static_assert(std::is_copy_assignable_v<failure<TestClass<true, true, false, false>>> == true);
static_assert(std::is_move_constructible_v<failure<TestClass<true, true, false, false>>> == true);
static_assert(std::is_move_assignable_v<failure<TestClass<true, true, false, false>>> == true);

static_assert(std::is_copy_constructible_v<failure<TestClass<true, false, false, true>>> == true);
static_assert(std::is_copy_assignable_v<failure<TestClass<true, false, false, true>>> == false);
static_assert(std::is_move_constructible_v<failure<TestClass<true, false, false, true>>> == true);
static_assert(std::is_move_assignable_v<failure<TestClass<true, false, false, true>>> == false);

static_assert(std::is_copy_constructible_v<failure<TestClass<true, false, false, false>>> == true);
static_assert(std::is_copy_assignable_v<failure<TestClass<true, false, false, false>>> == false);
static_assert(std::is_move_constructible_v<failure<TestClass<true, false, false, false>>> == true);
static_assert(std::is_move_assignable_v<failure<TestClass<true, false, false, false>>> == false);

static_assert(std::is_copy_constructible_v<failure<TestClass<false, true, true, true>>> == false);
static_assert(std::is_copy_assignable_v<failure<TestClass<false, true, true, true>>> == false);
static_assert(std::is_move_constructible_v<failure<TestClass<false, true, true, true>>> == true);
static_assert(std::is_move_assignable_v<failure<TestClass<false, true, true, true>>> == true);

static_assert(std::is_copy_constructible_v<failure<TestClass<false, true, true, false>>> == false);
static_assert(std::is_copy_assignable_v<failure<TestClass<false, true, true, false>>> == false);
static_assert(std::is_move_constructible_v<failure<TestClass<false, true, true, false>>> == true);
static_assert(std::is_move_assignable_v<failure<TestClass<false, true, true, false>>> == false);

static_assert(std::is_copy_constructible_v<failure<TestClass<false, false, true, true>>> == false);
static_assert(std::is_copy_assignable_v<failure<TestClass<false, false, true, true>>> == false);
static_assert(std::is_move_constructible_v<failure<TestClass<false, false, true, true>>> == true);
static_assert(std::is_move_assignable_v<failure<TestClass<false, false, true, true>>> == true);

static_assert(std::is_copy_constructible_v<failure<TestClass<false, false, true, false>>> == false);
static_assert(std::is_copy_assignable_v<failure<TestClass<false, false, true, false>>> == false);
static_assert(std::is_move_constructible_v<failure<TestClass<false, false, true, false>>> == true);
static_assert(std::is_move_assignable_v<failure<TestClass<false, false, true, false>>> == false);

static_assert(std::is_copy_constructible_v<failure<TestClass<false, true, false, true>>> == false);
static_assert(std::is_copy_assignable_v<failure<TestClass<false, true, false, true>>> == false);
static_assert(std::is_move_constructible_v<failure<TestClass<false, true, false, true>>> == false);
static_assert(std::is_move_assignable_v<failure<TestClass<false, true, false, true>>> == false);

static_assert(std::is_copy_constructible_v<failure<TestClass<false, true, false, false>>> == false);
static_assert(std::is_copy_assignable_v<failure<TestClass<false, true, false, false>>> == false);
static_assert(std::is_move_constructible_v<failure<TestClass<false, true, false, false>>> == false);
static_assert(std::is_move_assignable_v<failure<TestClass<false, true, false, false>>> == false);

static_assert(std::is_copy_constructible_v<failure<TestClass<false, false, false, true>>> == false);
static_assert(std::is_copy_assignable_v<failure<TestClass<false, false, false, true>>> == false);
static_assert(std::is_move_constructible_v<failure<TestClass<false, false, false, true>>> == false);
static_assert(std::is_move_assignable_v<failure<TestClass<false, false, false, true>>> == false);

static_assert(std::is_copy_constructible_v<failure<TestClass<false, false, false, false>>> == false);
static_assert(std::is_copy_assignable_v<failure<TestClass<false, false, false, false>>> == false);
static_assert(std::is_move_constructible_v<failure<TestClass<false, false, false, false>>> == false);
static_assert(std::is_move_assignable_v<failure<TestClass<false, false, false, false>>> == false);


template < class A, class B >
struct cc_expected{
  static constexpr bool value = std::conjunction_v<
    std::is_copy_constructible<A>, std::is_copy_constructible<B>
  >;
};

template < class A, class B >
struct mc_expected{
  static constexpr bool value = std::disjunction_v<
    std::conjunction<
      std::is_move_constructible<A>, std::is_move_constructible<B>
    >,
    std::conjunction<
      std::is_copy_constructible<A>, std::is_copy_constructible<B>
    >
  >;
};

template < class A, class B >
struct ca_expected{
  static constexpr bool value = std::conjunction_v<
    std::is_copy_constructible<A>, std::is_copy_constructible<B>,
    std::is_copy_assignable<A>, std::is_copy_assignable<B>
  >;
};

template < class A, class B >
struct ma_expected{
  static constexpr bool value = std::disjunction_v<
    std::conjunction<
      std::is_move_constructible<A>, std::is_move_constructible<B>,
      std::is_move_assignable<A>, std::is_move_assignable<B>
    >,
    std::conjunction<
      std::is_copy_constructible<A>, std::is_copy_constructible<B>,
      std::is_copy_assignable<A>, std::is_copy_assignable<B>
    >
  >;
};

#define SP_MEM_TEST(_1, _2, _3, _4, _5, _6, _7, _8)                                                                                                                            \
  static_assert(                                                                                                                                                               \
      cc_expected<TestClass<_1, _2, _3, _4>, TestClass<_5, _6, _7, _8>>::value == std::is_copy_constructible_v<Result<TestClass<_1, _2, _3, _4>, TestClass<_5, _6, _7, _8>>>); \
  static_assert(                                                                                                                                                               \
      ca_expected<TestClass<_1, _2, _3, _4>, TestClass<_5, _6, _7, _8>>::value == std::is_copy_assignable_v<Result<TestClass<_1, _2, _3, _4>, TestClass<_5, _6, _7, _8>>>);    \
  static_assert(                                                                                                                                                               \
      mc_expected<TestClass<_1, _2, _3, _4>, TestClass<_5, _6, _7, _8>>::value == std::is_move_constructible_v<Result<TestClass<_1, _2, _3, _4>, TestClass<_5, _6, _7, _8>>>); \
  static_assert(                                                                                                                                                               \
      ma_expected<TestClass<_1, _2, _3, _4>, TestClass<_5, _6, _7, _8>>::value == std::is_move_assignable_v<Result<TestClass<_1, _2, _3, _4>, TestClass<_5, _6, _7, _8>>>)

SP_MEM_TEST(true, true, true, true, true, true, true, true);
SP_MEM_TEST(true, true, true, true, true, true, true, false);
SP_MEM_TEST(true, true, true, true, true, true, false, true);
SP_MEM_TEST(true, true, true, true, true, true, false, false);
SP_MEM_TEST(true, true, true, true, true, false, true, true);
SP_MEM_TEST(true, true, true, true, true, false, true, false);
SP_MEM_TEST(true, true, true, true, true, false, false, true);
SP_MEM_TEST(true, true, true, true, true, false, false, false);
SP_MEM_TEST(true, true, true, true, false, true, true, true);
SP_MEM_TEST(true, true, true, true, false, true, true, false);
SP_MEM_TEST(true, true, true, true, false, true, false, true);
SP_MEM_TEST(true, true, true, true, false, true, false, false);
SP_MEM_TEST(true, true, true, true, false, false, true, true);
SP_MEM_TEST(true, true, true, true, false, false, true, false);
SP_MEM_TEST(true, true, true, true, false, false, false, true);
SP_MEM_TEST(true, true, true, true, false, false, false, false);
SP_MEM_TEST(true, true, true, false, true, true, true, true);
SP_MEM_TEST(true, true, true, false, true, true, true, false);
SP_MEM_TEST(true, true, true, false, true, true, false, true);
SP_MEM_TEST(true, true, true, false, true, true, false, false);
SP_MEM_TEST(true, true, true, false, true, false, true, true);
SP_MEM_TEST(true, true, true, false, true, false, true, false);
SP_MEM_TEST(true, true, true, false, true, false, false, true);
SP_MEM_TEST(true, true, true, false, true, false, false, false);
SP_MEM_TEST(true, true, true, false, false, true, true, true);
SP_MEM_TEST(true, true, true, false, false, true, true, false);
SP_MEM_TEST(true, true, true, false, false, true, false, true);
SP_MEM_TEST(true, true, true, false, false, true, false, false);
SP_MEM_TEST(true, true, true, false, false, false, true, true);
SP_MEM_TEST(true, true, true, false, false, false, true, false);
SP_MEM_TEST(true, true, true, false, false, false, false, true);
SP_MEM_TEST(true, true, true, false, false, false, false, false);
SP_MEM_TEST(true, true, false, true, true, true, true, true);
SP_MEM_TEST(true, true, false, true, true, true, true, false);
SP_MEM_TEST(true, true, false, true, true, true, false, true);
SP_MEM_TEST(true, true, false, true, true, true, false, false);
SP_MEM_TEST(true, true, false, true, true, false, true, true);
SP_MEM_TEST(true, true, false, true, true, false, true, false);
SP_MEM_TEST(true, true, false, true, true, false, false, true);
SP_MEM_TEST(true, true, false, true, true, false, false, false);
SP_MEM_TEST(true, true, false, true, false, true, true, true);
SP_MEM_TEST(true, true, false, true, false, true, true, false);
SP_MEM_TEST(true, true, false, true, false, true, false, true);
SP_MEM_TEST(true, true, false, true, false, true, false, false);
SP_MEM_TEST(true, true, false, true, false, false, true, true);
SP_MEM_TEST(true, true, false, true, false, false, true, false);
SP_MEM_TEST(true, true, false, true, false, false, false, true);
SP_MEM_TEST(true, true, false, true, false, false, false, false);
SP_MEM_TEST(true, true, false, false, true, true, true, true);
SP_MEM_TEST(true, true, false, false, true, true, true, false);
SP_MEM_TEST(true, true, false, false, true, true, false, true);
SP_MEM_TEST(true, true, false, false, true, true, false, false);
SP_MEM_TEST(true, true, false, false, true, false, true, true);
SP_MEM_TEST(true, true, false, false, true, false, true, false);
SP_MEM_TEST(true, true, false, false, true, false, false, true);
SP_MEM_TEST(true, true, false, false, true, false, false, false);
SP_MEM_TEST(true, true, false, false, false, true, true, true);
SP_MEM_TEST(true, true, false, false, false, true, true, false);
SP_MEM_TEST(true, true, false, false, false, true, false, true);
SP_MEM_TEST(true, true, false, false, false, true, false, false);
SP_MEM_TEST(true, true, false, false, false, false, true, true);
SP_MEM_TEST(true, true, false, false, false, false, true, false);
SP_MEM_TEST(true, true, false, false, false, false, false, true);
SP_MEM_TEST(true, true, false, false, false, false, false, false);
SP_MEM_TEST(true, false, true, true, true, true, true, true);
SP_MEM_TEST(true, false, true, true, true, true, true, false);
SP_MEM_TEST(true, false, true, true, true, true, false, true);
SP_MEM_TEST(true, false, true, true, true, true, false, false);
SP_MEM_TEST(true, false, true, true, true, false, true, true);
SP_MEM_TEST(true, false, true, true, true, false, true, false);
SP_MEM_TEST(true, false, true, true, true, false, false, true);
SP_MEM_TEST(true, false, true, true, true, false, false, false);
SP_MEM_TEST(true, false, true, true, false, true, true, true);
SP_MEM_TEST(true, false, true, true, false, true, true, false);
SP_MEM_TEST(true, false, true, true, false, true, false, true);
SP_MEM_TEST(true, false, true, true, false, true, false, false);
SP_MEM_TEST(true, false, true, true, false, false, true, true);
SP_MEM_TEST(true, false, true, true, false, false, true, false);
SP_MEM_TEST(true, false, true, true, false, false, false, true);
SP_MEM_TEST(true, false, true, true, false, false, false, false);
SP_MEM_TEST(true, false, true, false, true, true, true, true);
SP_MEM_TEST(true, false, true, false, true, true, true, false);
SP_MEM_TEST(true, false, true, false, true, true, false, true);
SP_MEM_TEST(true, false, true, false, true, true, false, false);
SP_MEM_TEST(true, false, true, false, true, false, true, true);
SP_MEM_TEST(true, false, true, false, true, false, true, false);
SP_MEM_TEST(true, false, true, false, true, false, false, true);
SP_MEM_TEST(true, false, true, false, true, false, false, false);
SP_MEM_TEST(true, false, true, false, false, true, true, true);
SP_MEM_TEST(true, false, true, false, false, true, true, false);
SP_MEM_TEST(true, false, true, false, false, true, false, true);
SP_MEM_TEST(true, false, true, false, false, true, false, false);
SP_MEM_TEST(true, false, true, false, false, false, true, true);
SP_MEM_TEST(true, false, true, false, false, false, true, false);
SP_MEM_TEST(true, false, true, false, false, false, false, true);
SP_MEM_TEST(true, false, true, false, false, false, false, false);
SP_MEM_TEST(true, false, false, true, true, true, true, true);
SP_MEM_TEST(true, false, false, true, true, true, true, false);
SP_MEM_TEST(true, false, false, true, true, true, false, true);
SP_MEM_TEST(true, false, false, true, true, true, false, false);
SP_MEM_TEST(true, false, false, true, true, false, true, true);
SP_MEM_TEST(true, false, false, true, true, false, true, false);
SP_MEM_TEST(true, false, false, true, true, false, false, true);
SP_MEM_TEST(true, false, false, true, true, false, false, false);
SP_MEM_TEST(true, false, false, true, false, true, true, true);
SP_MEM_TEST(true, false, false, true, false, true, true, false);
SP_MEM_TEST(true, false, false, true, false, true, false, true);
SP_MEM_TEST(true, false, false, true, false, true, false, false);
SP_MEM_TEST(true, false, false, true, false, false, true, true);
SP_MEM_TEST(true, false, false, true, false, false, true, false);
SP_MEM_TEST(true, false, false, true, false, false, false, true);
SP_MEM_TEST(true, false, false, true, false, false, false, false);
SP_MEM_TEST(true, false, false, false, true, true, true, true);
SP_MEM_TEST(true, false, false, false, true, true, true, false);
SP_MEM_TEST(true, false, false, false, true, true, false, true);
SP_MEM_TEST(true, false, false, false, true, true, false, false);
SP_MEM_TEST(true, false, false, false, true, false, true, true);
SP_MEM_TEST(true, false, false, false, true, false, true, false);
SP_MEM_TEST(true, false, false, false, true, false, false, true);
SP_MEM_TEST(true, false, false, false, true, false, false, false);
SP_MEM_TEST(true, false, false, false, false, true, true, true);
SP_MEM_TEST(true, false, false, false, false, true, true, false);
SP_MEM_TEST(true, false, false, false, false, true, false, true);
SP_MEM_TEST(true, false, false, false, false, true, false, false);
SP_MEM_TEST(true, false, false, false, false, false, true, true);
SP_MEM_TEST(true, false, false, false, false, false, true, false);
SP_MEM_TEST(true, false, false, false, false, false, false, true);
SP_MEM_TEST(true, false, false, false, false, false, false, false);
SP_MEM_TEST(false, true, true, true, true, true, true, true);
SP_MEM_TEST(false, true, true, true, true, true, true, false);
SP_MEM_TEST(false, true, true, true, true, true, false, true);
SP_MEM_TEST(false, true, true, true, true, true, false, false);
SP_MEM_TEST(false, true, true, true, true, false, true, true);
SP_MEM_TEST(false, true, true, true, true, false, true, false);
SP_MEM_TEST(false, true, true, true, true, false, false, true);
SP_MEM_TEST(false, true, true, true, true, false, false, false);
SP_MEM_TEST(false, true, true, true, false, true, true, true);
SP_MEM_TEST(false, true, true, true, false, true, true, false);
SP_MEM_TEST(false, true, true, true, false, true, false, true);
SP_MEM_TEST(false, true, true, true, false, true, false, false);
SP_MEM_TEST(false, true, true, true, false, false, true, true);
SP_MEM_TEST(false, true, true, true, false, false, true, false);
SP_MEM_TEST(false, true, true, true, false, false, false, true);
SP_MEM_TEST(false, true, true, true, false, false, false, false);
SP_MEM_TEST(false, true, true, false, true, true, true, true);
SP_MEM_TEST(false, true, true, false, true, true, true, false);
SP_MEM_TEST(false, true, true, false, true, true, false, true);
SP_MEM_TEST(false, true, true, false, true, true, false, false);
SP_MEM_TEST(false, true, true, false, true, false, true, true);
SP_MEM_TEST(false, true, true, false, true, false, true, false);
SP_MEM_TEST(false, true, true, false, true, false, false, true);
SP_MEM_TEST(false, true, true, false, true, false, false, false);
SP_MEM_TEST(false, true, true, false, false, true, true, true);
SP_MEM_TEST(false, true, true, false, false, true, true, false);
SP_MEM_TEST(false, true, true, false, false, true, false, true);
SP_MEM_TEST(false, true, true, false, false, true, false, false);
SP_MEM_TEST(false, true, true, false, false, false, true, true);
SP_MEM_TEST(false, true, true, false, false, false, true, false);
SP_MEM_TEST(false, true, true, false, false, false, false, true);
SP_MEM_TEST(false, true, true, false, false, false, false, false);
SP_MEM_TEST(false, true, false, true, true, true, true, true);
SP_MEM_TEST(false, true, false, true, true, true, true, false);
SP_MEM_TEST(false, true, false, true, true, true, false, true);
SP_MEM_TEST(false, true, false, true, true, true, false, false);
SP_MEM_TEST(false, true, false, true, true, false, true, true);
SP_MEM_TEST(false, true, false, true, true, false, true, false);
SP_MEM_TEST(false, true, false, true, true, false, false, true);
SP_MEM_TEST(false, true, false, true, true, false, false, false);
SP_MEM_TEST(false, true, false, true, false, true, true, true);
SP_MEM_TEST(false, true, false, true, false, true, true, false);
SP_MEM_TEST(false, true, false, true, false, true, false, true);
SP_MEM_TEST(false, true, false, true, false, true, false, false);
SP_MEM_TEST(false, true, false, true, false, false, true, true);
SP_MEM_TEST(false, true, false, true, false, false, true, false);
SP_MEM_TEST(false, true, false, true, false, false, false, true);
SP_MEM_TEST(false, true, false, true, false, false, false, false);
SP_MEM_TEST(false, true, false, false, true, true, true, true);
SP_MEM_TEST(false, true, false, false, true, true, true, false);
SP_MEM_TEST(false, true, false, false, true, true, false, true);
SP_MEM_TEST(false, true, false, false, true, true, false, false);
SP_MEM_TEST(false, true, false, false, true, false, true, true);
SP_MEM_TEST(false, true, false, false, true, false, true, false);
SP_MEM_TEST(false, true, false, false, true, false, false, true);
SP_MEM_TEST(false, true, false, false, true, false, false, false);
SP_MEM_TEST(false, true, false, false, false, true, true, true);
SP_MEM_TEST(false, true, false, false, false, true, true, false);
SP_MEM_TEST(false, true, false, false, false, true, false, true);
SP_MEM_TEST(false, true, false, false, false, true, false, false);
SP_MEM_TEST(false, true, false, false, false, false, true, true);
SP_MEM_TEST(false, true, false, false, false, false, true, false);
SP_MEM_TEST(false, true, false, false, false, false, false, true);
SP_MEM_TEST(false, true, false, false, false, false, false, false);
SP_MEM_TEST(false, false, true, true, true, true, true, true);
SP_MEM_TEST(false, false, true, true, true, true, true, false);
SP_MEM_TEST(false, false, true, true, true, true, false, true);
SP_MEM_TEST(false, false, true, true, true, true, false, false);
SP_MEM_TEST(false, false, true, true, true, false, true, true);
SP_MEM_TEST(false, false, true, true, true, false, true, false);
SP_MEM_TEST(false, false, true, true, true, false, false, true);
SP_MEM_TEST(false, false, true, true, true, false, false, false);
SP_MEM_TEST(false, false, true, true, false, true, true, true);
SP_MEM_TEST(false, false, true, true, false, true, true, false);
SP_MEM_TEST(false, false, true, true, false, true, false, true);
SP_MEM_TEST(false, false, true, true, false, true, false, false);
SP_MEM_TEST(false, false, true, true, false, false, true, true);
SP_MEM_TEST(false, false, true, true, false, false, true, false);
SP_MEM_TEST(false, false, true, true, false, false, false, true);
SP_MEM_TEST(false, false, true, true, false, false, false, false);
SP_MEM_TEST(false, false, true, false, true, true, true, true);
SP_MEM_TEST(false, false, true, false, true, true, true, false);
SP_MEM_TEST(false, false, true, false, true, true, false, true);
SP_MEM_TEST(false, false, true, false, true, true, false, false);
SP_MEM_TEST(false, false, true, false, true, false, true, true);
SP_MEM_TEST(false, false, true, false, true, false, true, false);
SP_MEM_TEST(false, false, true, false, true, false, false, true);
SP_MEM_TEST(false, false, true, false, true, false, false, false);
SP_MEM_TEST(false, false, true, false, false, true, true, true);
SP_MEM_TEST(false, false, true, false, false, true, true, false);
SP_MEM_TEST(false, false, true, false, false, true, false, true);
SP_MEM_TEST(false, false, true, false, false, true, false, false);
SP_MEM_TEST(false, false, true, false, false, false, true, true);
SP_MEM_TEST(false, false, true, false, false, false, true, false);
SP_MEM_TEST(false, false, true, false, false, false, false, true);
SP_MEM_TEST(false, false, true, false, false, false, false, false);
SP_MEM_TEST(false, false, false, true, true, true, true, true);
SP_MEM_TEST(false, false, false, true, true, true, true, false);
SP_MEM_TEST(false, false, false, true, true, true, false, true);
SP_MEM_TEST(false, false, false, true, true, true, false, false);
SP_MEM_TEST(false, false, false, true, true, false, true, true);
SP_MEM_TEST(false, false, false, true, true, false, true, false);
SP_MEM_TEST(false, false, false, true, true, false, false, true);
SP_MEM_TEST(false, false, false, true, true, false, false, false);
SP_MEM_TEST(false, false, false, true, false, true, true, true);
SP_MEM_TEST(false, false, false, true, false, true, true, false);
SP_MEM_TEST(false, false, false, true, false, true, false, true);
SP_MEM_TEST(false, false, false, true, false, true, false, false);
SP_MEM_TEST(false, false, false, true, false, false, true, true);
SP_MEM_TEST(false, false, false, true, false, false, true, false);
SP_MEM_TEST(false, false, false, true, false, false, false, true);
SP_MEM_TEST(false, false, false, true, false, false, false, false);
SP_MEM_TEST(false, false, false, false, true, true, true, true);
SP_MEM_TEST(false, false, false, false, true, true, true, false);
SP_MEM_TEST(false, false, false, false, true, true, false, true);
SP_MEM_TEST(false, false, false, false, true, true, false, false);
SP_MEM_TEST(false, false, false, false, true, false, true, true);
SP_MEM_TEST(false, false, false, false, true, false, true, false);
SP_MEM_TEST(false, false, false, false, true, false, false, true);
SP_MEM_TEST(false, false, false, false, true, false, false, false);
SP_MEM_TEST(false, false, false, false, false, true, true, true);
SP_MEM_TEST(false, false, false, false, false, true, true, false);
SP_MEM_TEST(false, false, false, false, false, true, false, true);
SP_MEM_TEST(false, false, false, false, false, true, false, false);
SP_MEM_TEST(false, false, false, false, false, false, true, true);
SP_MEM_TEST(false, false, false, false, false, false, true, false);
SP_MEM_TEST(false, false, false, false, false, false, false, true);
SP_MEM_TEST(false, false, false, false, false, false, false, false);

#undef SP_MEM_TEST
