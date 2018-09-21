#include "TestClass.hpp"
#include <type_traits>

static_assert(std::is_copy_constructible_v<Ok<TestClass<true, true, true, true>>> == true);
static_assert(std::is_copy_assignable_v<Ok<TestClass<true, true, true, true>>> == true);
static_assert(std::is_move_constructible_v<Ok<TestClass<true, true, true, true>>> == true);
static_assert(std::is_move_assignable_v<Ok<TestClass<true, true, true, true>>> == true);

static_assert(std::is_copy_constructible_v<Ok<TestClass<true, true, true, false>>> == true);
static_assert(std::is_copy_assignable_v<Ok<TestClass<true, true, true, false>>> == true);
static_assert(std::is_move_constructible_v<Ok<TestClass<true, true, true, false>>> == true);
static_assert(std::is_move_assignable_v<Ok<TestClass<true, true, true, false>>> == true);

static_assert(std::is_copy_constructible_v<Ok<TestClass<true, false, true, true>>> == true);
static_assert(std::is_copy_assignable_v<Ok<TestClass<true, false, true, true>>> == false);
static_assert(std::is_move_constructible_v<Ok<TestClass<true, false, true, true>>> == true);
static_assert(std::is_move_assignable_v<Ok<TestClass<true, false, true, true>>> == true);

static_assert(std::is_copy_constructible_v<Ok<TestClass<true, false, true, false>>> == true);
static_assert(std::is_copy_assignable_v<Ok<TestClass<true, false, true, false>>> == false);
static_assert(std::is_move_constructible_v<Ok<TestClass<true, false, true, false>>> == true);
static_assert(std::is_move_assignable_v<Ok<TestClass<true, false, true, false>>> == false);

static_assert(std::is_copy_constructible_v<Ok<TestClass<true, true, false, true>>> == true);
static_assert(std::is_copy_assignable_v<Ok<TestClass<true, true, false, true>>> == true);
static_assert(std::is_move_constructible_v<Ok<TestClass<true, true, false, true>>> == true);
static_assert(std::is_move_assignable_v<Ok<TestClass<true, true, false, true>>> == true);

static_assert(std::is_copy_constructible_v<Ok<TestClass<true, true, false, false>>> == true);
static_assert(std::is_copy_assignable_v<Ok<TestClass<true, true, false, false>>> == true);
static_assert(std::is_move_constructible_v<Ok<TestClass<true, true, false, false>>> == true);
static_assert(std::is_move_assignable_v<Ok<TestClass<true, true, false, false>>> == true);

static_assert(std::is_copy_constructible_v<Ok<TestClass<true, false, false, true>>> == true);
static_assert(std::is_copy_assignable_v<Ok<TestClass<true, false, false, true>>> == false);
static_assert(std::is_move_constructible_v<Ok<TestClass<true, false, false, true>>> == true);
static_assert(std::is_move_assignable_v<Ok<TestClass<true, false, false, true>>> == false);

static_assert(std::is_copy_constructible_v<Ok<TestClass<true, false, false, false>>> == true);
static_assert(std::is_copy_assignable_v<Ok<TestClass<true, false, false, false>>> == false);
static_assert(std::is_move_constructible_v<Ok<TestClass<true, false, false, false>>> == true);
static_assert(std::is_move_assignable_v<Ok<TestClass<true, false, false, false>>> == false);

static_assert(std::is_copy_constructible_v<Ok<TestClass<false, true, true, true>>> == false);
static_assert(std::is_copy_assignable_v<Ok<TestClass<false, true, true, true>>> == false);
static_assert(std::is_move_constructible_v<Ok<TestClass<false, true, true, true>>> == true);
static_assert(std::is_move_assignable_v<Ok<TestClass<false, true, true, true>>> == true);

static_assert(std::is_copy_constructible_v<Ok<TestClass<false, true, true, false>>> == false);
static_assert(std::is_copy_assignable_v<Ok<TestClass<false, true, true, false>>> == false);
static_assert(std::is_move_constructible_v<Ok<TestClass<false, true, true, false>>> == true);
static_assert(std::is_move_assignable_v<Ok<TestClass<false, true, true, false>>> == false);

static_assert(std::is_copy_constructible_v<Ok<TestClass<false, false, true, true>>> == false);
static_assert(std::is_copy_assignable_v<Ok<TestClass<false, false, true, true>>> == false);
static_assert(std::is_move_constructible_v<Ok<TestClass<false, false, true, true>>> == true);
static_assert(std::is_move_assignable_v<Ok<TestClass<false, false, true, true>>> == true);

static_assert(std::is_copy_constructible_v<Ok<TestClass<false, false, true, false>>> == false);
static_assert(std::is_copy_assignable_v<Ok<TestClass<false, false, true, false>>> == false);
static_assert(std::is_move_constructible_v<Ok<TestClass<false, false, true, false>>> == true);
static_assert(std::is_move_assignable_v<Ok<TestClass<false, false, true, false>>> == false);

static_assert(std::is_copy_constructible_v<Ok<TestClass<false, true, false, true>>> == false);
static_assert(std::is_copy_assignable_v<Ok<TestClass<false, true, false, true>>> == false);
static_assert(std::is_move_constructible_v<Ok<TestClass<false, true, false, true>>> == false);
static_assert(std::is_move_assignable_v<Ok<TestClass<false, true, false, true>>> == false);

static_assert(std::is_copy_constructible_v<Ok<TestClass<false, true, false, false>>> == false);
static_assert(std::is_copy_assignable_v<Ok<TestClass<false, true, false, false>>> == false);
static_assert(std::is_move_constructible_v<Ok<TestClass<false, true, false, false>>> == false);
static_assert(std::is_move_assignable_v<Ok<TestClass<false, true, false, false>>> == false);

static_assert(std::is_copy_constructible_v<Ok<TestClass<false, false, false, true>>> == false);
static_assert(std::is_copy_assignable_v<Ok<TestClass<false, false, false, true>>> == false);
static_assert(std::is_move_constructible_v<Ok<TestClass<false, false, false, true>>> == false);
static_assert(std::is_move_assignable_v<Ok<TestClass<false, false, false, true>>> == false);

static_assert(std::is_copy_constructible_v<Ok<TestClass<false, false, false, false>>> == false);
static_assert(std::is_copy_assignable_v<Ok<TestClass<false, false, false, false>>> == false);
static_assert(std::is_move_constructible_v<Ok<TestClass<false, false, false, false>>> == false);
static_assert(std::is_move_assignable_v<Ok<TestClass<false, false, false, false>>> == false);

static_assert(std::is_copy_constructible_v<Err<TestClass<true, true, true, true>>> == true);
static_assert(std::is_copy_assignable_v<Err<TestClass<true, true, true, true>>> == true);
static_assert(std::is_move_constructible_v<Err<TestClass<true, true, true, true>>> == true);
static_assert(std::is_move_assignable_v<Err<TestClass<true, true, true, true>>> == true);

static_assert(std::is_copy_constructible_v<Err<TestClass<true, true, true, false>>> == true);
static_assert(std::is_copy_assignable_v<Err<TestClass<true, true, true, false>>> == true);
static_assert(std::is_move_constructible_v<Err<TestClass<true, true, true, false>>> == true);
static_assert(std::is_move_assignable_v<Err<TestClass<true, true, true, false>>> == true);

static_assert(std::is_copy_constructible_v<Err<TestClass<true, false, true, true>>> == true);
static_assert(std::is_copy_assignable_v<Err<TestClass<true, false, true, true>>> == false);
static_assert(std::is_move_constructible_v<Err<TestClass<true, false, true, true>>> == true);
static_assert(std::is_move_assignable_v<Err<TestClass<true, false, true, true>>> == true);

static_assert(std::is_copy_constructible_v<Err<TestClass<true, false, true, false>>> == true);
static_assert(std::is_copy_assignable_v<Err<TestClass<true, false, true, false>>> == false);
static_assert(std::is_move_constructible_v<Err<TestClass<true, false, true, false>>> == true);
static_assert(std::is_move_assignable_v<Err<TestClass<true, false, true, false>>> == false);

static_assert(std::is_copy_constructible_v<Err<TestClass<true, true, false, true>>> == true);
static_assert(std::is_copy_assignable_v<Err<TestClass<true, true, false, true>>> == true);
static_assert(std::is_move_constructible_v<Err<TestClass<true, true, false, true>>> == true);
static_assert(std::is_move_assignable_v<Err<TestClass<true, true, false, true>>> == true);

static_assert(std::is_copy_constructible_v<Err<TestClass<true, true, false, false>>> == true);
static_assert(std::is_copy_assignable_v<Err<TestClass<true, true, false, false>>> == true);
static_assert(std::is_move_constructible_v<Err<TestClass<true, true, false, false>>> == true);
static_assert(std::is_move_assignable_v<Err<TestClass<true, true, false, false>>> == true);

static_assert(std::is_copy_constructible_v<Err<TestClass<true, false, false, true>>> == true);
static_assert(std::is_copy_assignable_v<Err<TestClass<true, false, false, true>>> == false);
static_assert(std::is_move_constructible_v<Err<TestClass<true, false, false, true>>> == true);
static_assert(std::is_move_assignable_v<Err<TestClass<true, false, false, true>>> == false);

static_assert(std::is_copy_constructible_v<Err<TestClass<true, false, false, false>>> == true);
static_assert(std::is_copy_assignable_v<Err<TestClass<true, false, false, false>>> == false);
static_assert(std::is_move_constructible_v<Err<TestClass<true, false, false, false>>> == true);
static_assert(std::is_move_assignable_v<Err<TestClass<true, false, false, false>>> == false);

static_assert(std::is_copy_constructible_v<Err<TestClass<false, true, true, true>>> == false);
static_assert(std::is_copy_assignable_v<Err<TestClass<false, true, true, true>>> == false);
static_assert(std::is_move_constructible_v<Err<TestClass<false, true, true, true>>> == true);
static_assert(std::is_move_assignable_v<Err<TestClass<false, true, true, true>>> == true);

static_assert(std::is_copy_constructible_v<Err<TestClass<false, true, true, false>>> == false);
static_assert(std::is_copy_assignable_v<Err<TestClass<false, true, true, false>>> == false);
static_assert(std::is_move_constructible_v<Err<TestClass<false, true, true, false>>> == true);
static_assert(std::is_move_assignable_v<Err<TestClass<false, true, true, false>>> == false);

static_assert(std::is_copy_constructible_v<Err<TestClass<false, false, true, true>>> == false);
static_assert(std::is_copy_assignable_v<Err<TestClass<false, false, true, true>>> == false);
static_assert(std::is_move_constructible_v<Err<TestClass<false, false, true, true>>> == true);
static_assert(std::is_move_assignable_v<Err<TestClass<false, false, true, true>>> == true);

static_assert(std::is_copy_constructible_v<Err<TestClass<false, false, true, false>>> == false);
static_assert(std::is_copy_assignable_v<Err<TestClass<false, false, true, false>>> == false);
static_assert(std::is_move_constructible_v<Err<TestClass<false, false, true, false>>> == true);
static_assert(std::is_move_assignable_v<Err<TestClass<false, false, true, false>>> == false);

static_assert(std::is_copy_constructible_v<Err<TestClass<false, true, false, true>>> == false);
static_assert(std::is_copy_assignable_v<Err<TestClass<false, true, false, true>>> == false);
static_assert(std::is_move_constructible_v<Err<TestClass<false, true, false, true>>> == false);
static_assert(std::is_move_assignable_v<Err<TestClass<false, true, false, true>>> == false);

static_assert(std::is_copy_constructible_v<Err<TestClass<false, true, false, false>>> == false);
static_assert(std::is_copy_assignable_v<Err<TestClass<false, true, false, false>>> == false);
static_assert(std::is_move_constructible_v<Err<TestClass<false, true, false, false>>> == false);
static_assert(std::is_move_assignable_v<Err<TestClass<false, true, false, false>>> == false);

static_assert(std::is_copy_constructible_v<Err<TestClass<false, false, false, true>>> == false);
static_assert(std::is_copy_assignable_v<Err<TestClass<false, false, false, true>>> == false);
static_assert(std::is_move_constructible_v<Err<TestClass<false, false, false, true>>> == false);
static_assert(std::is_move_assignable_v<Err<TestClass<false, false, false, true>>> == false);

static_assert(std::is_copy_constructible_v<Err<TestClass<false, false, false, false>>> == false);
static_assert(std::is_copy_assignable_v<Err<TestClass<false, false, false, false>>> == false);
static_assert(std::is_move_constructible_v<Err<TestClass<false, false, false, false>>> == false);
static_assert(std::is_move_assignable_v<Err<TestClass<false, false, false, false>>> == false);


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

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,true>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,true>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,true>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,true,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,true,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,true,false>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,true,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,true,false>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,true,false>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,true>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,true>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,true>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,true,false,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,true,false,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,true,false,false>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,true,false,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,true,false,false>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,true,false,false>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,true>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,true>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,true>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,true,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,true,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,true,false>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,true,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,true,false>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,true,false>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,true>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,true>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,true>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<true,false,false,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<true,false,false,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<true,false,false,false>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<true,false,false,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<true,false,false,false>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<true,false,false,false>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,true>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,true>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,true>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,true,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,true,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,true,false>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,true,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,true,false>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,true,false>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,true>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,true>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,true>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,true,false,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,true,false,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,true,false,false>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,true,false,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,true,false,false>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,true,false,false>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,true>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,true>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,true>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,true,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,true,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,true,false>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,true,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,true,false>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,true,false>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,true>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,true>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,true>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,true>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,true>, TestClass<false,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<true,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<true,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<true,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<true,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<true,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<true,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<true,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<true,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<true,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<true,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<true,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<true,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<true,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<true,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<true,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<true,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<true,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<true,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<true,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<true,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<true,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<true,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<true,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<true,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<true,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<true,false,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<false,true,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,true,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<false,true,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,true,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<false,true,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,true,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<false,true,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,true,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<false,true,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,true,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<false,true,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,true,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<false,true,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,true,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<false,true,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,true,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<false,true,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,true,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<false,true,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,true,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<false,true,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,true,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<false,true,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,true,false,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<false,false,true,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,false,true,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<false,false,true,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,false,true,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<false,false,true,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,false,true,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<false,false,true,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,false,true,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<false,false,true,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,false,true,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<false,false,true,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,false,true,false>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<false,false,false,true>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,false,false,true>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<false,false,false,true>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,false,false,true>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<false,false,false,true>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,false,false,true>>>
);

static_assert(
  cc_expected<TestClass<false,false,false,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ca_expected<TestClass<false,false,false,false>, TestClass<false,false,false,false>>::value
  == std::is_copy_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,false,false,false>>>
);
static_assert(
  mc_expected<TestClass<false,false,false,false>, TestClass<false,false,false,false>>::value
  == std::is_move_constructible_v<Result<TestClass<false,false,false,false>, TestClass<false,false,false,false>>>
);
static_assert(
  ma_expected<TestClass<false,false,false,false>, TestClass<false,false,false,false>>::value
  == std::is_move_assignable_v<Result<TestClass<false,false,false,false>, TestClass<false,false,false,false>>>
);

