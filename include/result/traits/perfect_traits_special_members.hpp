#ifndef MITAMA_RESULT_PERFECT_TRAIT
#define MITAMA_RESULT_PERFECT_TRAIT
#include <type_traits>

namespace mitamagic
{

template <bool Copy, bool CopyAssignment,
          bool Move, bool MoveAssignment,
          class Tag = void>
struct perfect_trait_copy_move
{
};

template < class T >
struct is_copyable: std::conjunction<std::is_copy_constructible<T>, std::is_copy_assignable<T>> {};
template < class T >
struct is_movable: std::conjunction<std::is_move_constructible<T>, std::is_move_assignable<T>> {};

template <class Tag, class... Targets>
using perfect_trait_copy_move_for = perfect_trait_copy_move<
    std::conjunction_v<std::is_copy_constructible<Targets>...>,
    std::conjunction_v<is_copyable<Targets>...>,
    std::conjunction_v<std::is_move_constructible<Targets>...>,
    std::conjunction_v<is_movable<Targets>...>,
    Tag>;

struct perfect_trait_default_ctor_receptor
{
  explicit constexpr perfect_trait_default_ctor_receptor() = default;
};

template <bool B, class Tag = void>
struct perfect_trait_default_ctor
{
  constexpr perfect_trait_default_ctor() noexcept = default;
  constexpr perfect_trait_default_ctor(perfect_trait_default_ctor const &) noexcept = default;
  constexpr perfect_trait_default_ctor(perfect_trait_default_ctor &&) noexcept = default;
  perfect_trait_default_ctor &
  operator=(perfect_trait_default_ctor const &) noexcept = default;
  perfect_trait_default_ctor &
  operator=(perfect_trait_default_ctor &&) noexcept = default;

  constexpr explicit perfect_trait_default_ctor(perfect_trait_default_ctor_receptor) {}
};

template <class Tag, class ... Targets>
using perfect_trait_default_ctor_for
    = perfect_trait_default_ctor<std::conjunction_v<std::is_default_constructible<Targets>...>, Tag>;

template <class Tag>
struct perfect_trait_default_ctor<false, Tag>
{
  constexpr perfect_trait_default_ctor() noexcept = delete;
  constexpr perfect_trait_default_ctor(perfect_trait_default_ctor const &) noexcept = default;
  constexpr perfect_trait_default_ctor(perfect_trait_default_ctor &&) noexcept = default;
  perfect_trait_default_ctor &
  operator=(perfect_trait_default_ctor const &) noexcept = default;
  perfect_trait_default_ctor &
  operator=(perfect_trait_default_ctor &&) noexcept = default;

  constexpr explicit perfect_trait_default_ctor(perfect_trait_default_ctor_receptor) {}
};

template <bool B, class Tag = void>
struct perfect_trait_dtor
{
};

template <class Tag, class ... Targets>
using perfect_trait_dtor_for
    = perfect_trait_dtor<std::conjunction_v<std::is_destructible<Targets>...>, Tag>;

template <class Tag>
struct perfect_trait_dtor<false, Tag>
{
  ~perfect_trait_dtor() noexcept = delete;
};

template <class Tag>
struct perfect_trait_copy_move<false, true, true, true, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = delete;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = default;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = default;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = default;
};

template <class Tag>
struct perfect_trait_copy_move<true, false, true, true, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = default;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = default;
};

template <class Tag>
struct perfect_trait_copy_move<false, false, true, true, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = delete;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = default;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = default;
};

template <class Tag>
struct perfect_trait_copy_move<true, true, false, true, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = default;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = default;
};

template <class Tag>
struct perfect_trait_copy_move<false, true, false, true, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = delete;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = default;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = default;
};

template <class Tag>
struct perfect_trait_copy_move<true, false, false, true, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = default;
};

template <class Tag>
struct perfect_trait_copy_move<false, false, false, true, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = delete;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = default;
};

template <class Tag>
struct perfect_trait_copy_move<true, true, true, false, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = default;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = default;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = delete;
};

template <class Tag>
struct perfect_trait_copy_move<false, true, true, false, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = delete;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = default;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = default;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = delete;
};

template <class Tag>
struct perfect_trait_copy_move<true, false, true, false, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = default;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = delete;
};

template <class Tag>
struct perfect_trait_copy_move<false, false, true, false, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = delete;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = default;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = delete;
};

template <class Tag>
struct perfect_trait_copy_move<true, true, false, false, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = default;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = delete;
};

template <class Tag>
struct perfect_trait_copy_move<false, true, false, false, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = delete;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = default;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = delete;
};

template <class Tag>
struct perfect_trait_copy_move<true, false, false, false, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = delete;
};

template <class Tag>
struct perfect_trait_copy_move<false, false, false, false, Tag>
{
  constexpr perfect_trait_copy_move() noexcept = default;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move const &) noexcept = delete;
  constexpr perfect_trait_copy_move(perfect_trait_copy_move &&) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move const &) noexcept = delete;
  perfect_trait_copy_move &
  operator=(perfect_trait_copy_move &&) noexcept = delete;
};

} // namespace _detail
#endif
