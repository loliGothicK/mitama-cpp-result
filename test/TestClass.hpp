#pragma once
#include <type_traits>

template <bool CopyConstructible,
          bool CopyAssignable,
          bool MoveConstructible,
          bool MoveAssignable>
struct TestClass
{
  // special member definitions ...
};

struct Receptor
{
};

template <>
struct TestClass<true, true, true, true>
{
  TestClass() = default;
  TestClass(const TestClass &) = default;
  TestClass &operator=(const TestClass &) = default;
  TestClass(TestClass &&) = default;
  TestClass &operator=(TestClass &&) = default;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<true, true, true, false>
{
  TestClass() = default;
  TestClass(const TestClass &) = default;
  TestClass &operator=(const TestClass &) = default;
  TestClass(TestClass &&) = default;
  TestClass &operator=(TestClass &&) = delete;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<true, true, false, true>
{
  TestClass() = default;
  TestClass(const TestClass &) = default;
  TestClass &operator=(const TestClass &) = default;
  TestClass(TestClass &&) = delete;
  TestClass &operator=(TestClass &&) = default;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<true, true, false, false>
{
  TestClass() = default;
  TestClass(const TestClass &) = default;
  TestClass &operator=(const TestClass &) = default;
  TestClass(TestClass &&) = delete;
  TestClass &operator=(TestClass &&) = delete;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<true, false, true, true>
{
  TestClass() = default;
  TestClass(const TestClass &) = default;
  TestClass &operator=(const TestClass &) = delete;
  TestClass(TestClass &&) = default;
  TestClass &operator=(TestClass &&) = default;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<true, false, true, false>
{
  TestClass() = default;
  TestClass(const TestClass &) = default;
  TestClass &operator=(const TestClass &) = delete;
  TestClass(TestClass &&) = default;
  TestClass &operator=(TestClass &&) = delete;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<true, false, false, true>
{
  TestClass() = default;
  TestClass(const TestClass &) = default;
  TestClass &operator=(const TestClass &) = delete;
  TestClass(TestClass &&) = delete;
  TestClass &operator=(TestClass &&) = default;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<true, false, false, false>
{
  TestClass() = default;
  TestClass(const TestClass &) = default;
  TestClass &operator=(const TestClass &) = delete;
  TestClass(TestClass &&) = delete;
  TestClass &operator=(TestClass &&) = delete;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<false, true, true, true>
{
  TestClass() = default;
  TestClass(const TestClass &) = delete;
  TestClass &operator=(const TestClass &) = default;
  TestClass(TestClass &&) = default;
  TestClass &operator=(TestClass &&) = default;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<false, true, true, false>
{
  TestClass() = default;
  TestClass(const TestClass &) = delete;
  TestClass &operator=(const TestClass &) = default;
  TestClass(TestClass &&) = default;
  TestClass &operator=(TestClass &&) = delete;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<false, true, false, true>
{
  TestClass() = default;
  TestClass(const TestClass &) = delete;
  TestClass &operator=(const TestClass &) = default;
  TestClass(TestClass &&) = delete;
  TestClass &operator=(TestClass &&) = default;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<false, true, false, false>
{
  TestClass() = default;
  TestClass(const TestClass &) = delete;
  TestClass &operator=(const TestClass &) = default;
  TestClass(TestClass &&) = delete;
  TestClass &operator=(TestClass &&) = delete;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<false, false, true, true>
{
  TestClass() = default;
  TestClass(const TestClass &) = delete;
  TestClass &operator=(const TestClass &) = delete;
  TestClass(TestClass &&) = default;
  TestClass &operator=(TestClass &&) = default;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<false, false, true, false>
{
  TestClass() = default;
  TestClass(const TestClass &) = delete;
  TestClass &operator=(const TestClass &) = delete;
  TestClass(TestClass &&) = default;
  TestClass &operator=(TestClass &&) = delete;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<false, false, false, true>
{
  TestClass() = default;
  TestClass(const TestClass &) = delete;
  TestClass &operator=(const TestClass &) = delete;
  TestClass(TestClass &&) = delete;
  TestClass &operator=(TestClass &&) = default;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};

template <>
struct TestClass<false, false, false, false>
{
  TestClass() = default;
  TestClass(const TestClass &) = delete;
  TestClass &operator=(const TestClass &) = delete;
  TestClass(TestClass &&) = delete;
  TestClass &operator=(TestClass &&) = delete;
  ~TestClass() = default;
  TestClass(const Receptor &) {}
};
static_assert(std::is_copy_constructible_v<TestClass<true, true, true, true>> == true);
static_assert(std::is_copy_assignable_v<TestClass<true, true, true, true>> == true);
static_assert(std::is_move_constructible_v<TestClass<true, true, true, true>> == true);
static_assert(std::is_move_assignable_v<TestClass<true, true, true, true>> == true);

static_assert(std::is_copy_constructible_v<TestClass<true, true, true, false>> == true);
static_assert(std::is_copy_assignable_v<TestClass<true, true, true, false>> == true);
static_assert(std::is_move_constructible_v<TestClass<true, true, true, false>> == true);
static_assert(std::is_move_assignable_v<TestClass<true, true, true, false>> == false);

static_assert(std::is_copy_constructible_v<TestClass<true, false, true, true>> == true);
static_assert(std::is_copy_assignable_v<TestClass<true, false, true, true>> == false);
static_assert(std::is_move_constructible_v<TestClass<true, false, true, true>> == true);
static_assert(std::is_move_assignable_v<TestClass<true, false, true, true>> == true);

static_assert(std::is_copy_constructible_v<TestClass<true, false, true, false>> == true);
static_assert(std::is_copy_assignable_v<TestClass<true, false, true, false>> == false);
static_assert(std::is_move_constructible_v<TestClass<true, false, true, false>> == true);
static_assert(std::is_move_assignable_v<TestClass<true, false, true, false>> == false);

static_assert(std::is_copy_constructible_v<TestClass<true, true, false, true>> == true);
static_assert(std::is_copy_assignable_v<TestClass<true, true, false, true>> == true);
static_assert(std::is_move_constructible_v<TestClass<true, true, false, true>> == false);
static_assert(std::is_move_assignable_v<TestClass<true, true, false, true>> == true);

static_assert(std::is_copy_constructible_v<TestClass<true, true, false, false>> == true);
static_assert(std::is_copy_assignable_v<TestClass<true, true, false, false>> == true);
static_assert(std::is_move_constructible_v<TestClass<true, true, false, false>> == false);
static_assert(std::is_move_assignable_v<TestClass<true, true, false, false>> == false);

static_assert(std::is_copy_constructible_v<TestClass<true, false, false, true>> == true);
static_assert(std::is_copy_assignable_v<TestClass<true, false, false, true>> == false);
static_assert(std::is_move_constructible_v<TestClass<true, false, false, true>> == false);
static_assert(std::is_move_assignable_v<TestClass<true, false, false, true>> == true);

static_assert(std::is_copy_constructible_v<TestClass<true, false, false, false>> == true);
static_assert(std::is_copy_assignable_v<TestClass<true, false, false, false>> == false);
static_assert(std::is_move_constructible_v<TestClass<true, false, false, false>> == false);
static_assert(std::is_move_assignable_v<TestClass<true, false, false, false>> == false);

static_assert(std::is_copy_constructible_v<TestClass<false, true, true, true>> == false);
static_assert(std::is_copy_assignable_v<TestClass<false, true, true, true>> == true);
static_assert(std::is_move_constructible_v<TestClass<false, true, true, true>> == true);
static_assert(std::is_move_assignable_v<TestClass<false, true, true, true>> == true);

static_assert(std::is_copy_constructible_v<TestClass<false, true, true, false>> == false);
static_assert(std::is_copy_assignable_v<TestClass<false, true, true, false>> == true);
static_assert(std::is_move_constructible_v<TestClass<false, true, true, false>> == true);
static_assert(std::is_move_assignable_v<TestClass<false, true, true, false>> == false);

static_assert(std::is_copy_constructible_v<TestClass<false, false, true, true>> == false);
static_assert(std::is_copy_assignable_v<TestClass<false, false, true, true>> == false);
static_assert(std::is_move_constructible_v<TestClass<false, false, true, true>> == true);
static_assert(std::is_move_assignable_v<TestClass<false, false, true, true>> == true);

static_assert(std::is_copy_constructible_v<TestClass<false, false, true, false>> == false);
static_assert(std::is_copy_assignable_v<TestClass<false, false, true, false>> == false);
static_assert(std::is_move_constructible_v<TestClass<false, false, true, false>> == true);
static_assert(std::is_move_assignable_v<TestClass<false, false, true, false>> == false);

static_assert(std::is_copy_constructible_v<TestClass<false, true, false, true>> == false);
static_assert(std::is_copy_assignable_v<TestClass<false, true, false, true>> == true);
static_assert(std::is_move_constructible_v<TestClass<false, true, false, true>> == false);
static_assert(std::is_move_assignable_v<TestClass<false, true, false, true>> == true);

static_assert(std::is_copy_constructible_v<TestClass<false, true, false, false>> == false);
static_assert(std::is_copy_assignable_v<TestClass<false, true, false, false>> == true);
static_assert(std::is_move_constructible_v<TestClass<false, true, false, false>> == false);
static_assert(std::is_move_assignable_v<TestClass<false, true, false, false>> == false);

static_assert(std::is_copy_constructible_v<TestClass<false, false, false, true>> == false);
static_assert(std::is_copy_assignable_v<TestClass<false, false, false, true>> == false);
static_assert(std::is_move_constructible_v<TestClass<false, false, false, true>> == false);
static_assert(std::is_move_assignable_v<TestClass<false, false, false, true>> == true);

static_assert(std::is_copy_constructible_v<TestClass<false, false, false, false>> == false);
static_assert(std::is_copy_assignable_v<TestClass<false, false, false, false>> == false);
static_assert(std::is_move_constructible_v<TestClass<false, false, false, false>> == false);
static_assert(std::is_move_assignable_v<TestClass<false, false, false, false>> == false);