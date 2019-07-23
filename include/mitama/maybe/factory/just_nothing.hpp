#ifndef MITAMA_MAYBE_FACTORY_JUST_HPP
#define MITAMA_MAYBE_FACTORY_JUST_HPP
#include <mitama/maybe/fwd/maybe_fwd.hpp>
#include <mitama/result/detail/meta.hpp>
#include <mitama/result/traits/perfect_traits_special_members.hpp>
#include <type_traits>
#include <utility>

namespace mitama {

struct [[nodiscard]] nothing_t {};

inline constexpr nothing_t nothing{};

std::ostream& operator<<(std::ostream& os, nothing_t) {
    return os << "nothing";
}


/// class just:
/// The main use of this class is to propagate some value to the constructor of the maybe class.
template <class T>
class [[nodiscard]] just_t
    : private ::mitamagic::perfect_trait_copy_move<
          std::is_copy_constructible_v<std::decay_t<T>>,
          std::conjunction_v<std::is_copy_constructible<std::decay_t<T>>, std::is_copy_assignable<std::decay_t<T>>>,
          std::is_move_constructible_v<std::decay_t<T>>,
          std::conjunction_v<std::is_move_constructible<std::decay_t<T>>, std::is_move_assignable<std::decay_t<T>>>,
          just_t<T>>
{
    template <class>
    friend class just_t;
    template <class>
    friend class maybe;
    T x;

    template <class... Requires>
    using where = std::enable_if_t<std::conjunction_v<Requires...>, std::nullptr_t>;

    static constexpr std::nullptr_t required = nullptr;

    template <class U>
    using not_self = std::negation<std::is_same<just_t, U>>;
public:
  using type = T;

    template <class U = T>
    constexpr just_t(std::enable_if_t<std::is_same_v<std::monostate, U>, std::nullptr_t> = nullptr)
    { /* whatever */ }

    template <class U,
        where<not_self<std::decay_t<U>>,
                std::is_constructible<T, U>,
                std::is_convertible<U, T>> = required>
    constexpr just_t(U&& u) noexcept(std::is_nothrow_constructible_v<T, U>)
        : x(std::forward<U>(u)) {}

    template <class U,
        where<not_self<std::decay_t<U>>,
                std::is_constructible<T, U>,
                std::negation<std::is_convertible<U, T>>> = required>
    explicit constexpr just_t(U&& u) noexcept(std::is_nothrow_constructible_v<T, U>)
        : x(std::forward<U>(u)) {}

    template <typename U,
        where<std::negation<std::is_same<T, U>>,
                std::is_constructible<T, const U &>,
                std::is_convertible<const U &, T>> = required>
    constexpr just_t(const just_t<U> &t) noexcept(std::is_nothrow_constructible_v<T, U>)
        : x(t.x) {}

    template <typename U,
        where<std::negation<std::is_same<T, U>>,
                std::is_constructible<T, const U &>,
                std::negation<std::is_convertible<const U &, T>>> = required>
    explicit constexpr just_t(const just_t<U> &t) noexcept(std::is_nothrow_constructible_v<T, U>)
        : x(t.x) {}

    template <typename U,
        where<std::negation<std::is_same<T, U>>,
                std::is_constructible<T, U&&>,
                std::is_convertible<U&&, T>> = required>
    constexpr just_t(just_t<U> && t) noexcept(std::is_nothrow_constructible_v<T, U>)
        : x(std::move(t.x)) {}

    template <typename U,
        where<std::negation<std::is_same<T, U>>,
                std::is_constructible<T, U&&>,
                std::negation<std::is_convertible<U&&, T>>> = required>
    explicit constexpr just_t(just_t<U> && t) noexcept(std::is_nothrow_constructible_v<T, U>)
        : x(std::move(t.x)) {}

    template <class... Args,
            where<std::is_constructible<T, Args...>> = required>
    explicit constexpr just_t(std::in_place_t, Args && ... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
        : x(std::forward<Args>(args)...) {}

    template <class U>
    friend
    std::enable_if_t<meta::is_comparable_with<T, U>::value,
    bool>
    operator==(maybe<U> const& lhs, just_t const& rhs) {
        return lhs && (lhs.unwrap() == rhs.x);
    }

    template <class U>
    friend
    std::enable_if_t<meta::is_comparable_with<T, U>::value,
    bool>
    operator==(just_t const& lhs, maybe<U> const& rhs) {
        return rhs && (lhs.x == rhs.unwrap());
    }

    template <class U>
    friend
    std::enable_if_t<meta::is_comparable_with<T, U>::value,
    bool>
    operator!=(maybe<U> const& lhs, just_t const& rhs) {
        return !(lhs == rhs);
    }

    template <class U>
    friend
    std::enable_if_t<meta::is_comparable_with<T, U>::value,
    bool>
    operator!=(just_t const& lhs, maybe<U> const& rhs) {
        return !(lhs == rhs);
    }

    friend std::ostream& operator<<(std::ostream& os, just_t const& j) {
        return os << boost::hana::overload_linearly(
          [](std::monostate) { return boost::format("just()"); },
          [](std::string_view x) { return boost::format("just(\"%1%\")") % x; },
          [](auto const& x) { return boost::format("just(%1%)") % x; })
        (j.x);
    }
};

template <class T>
auto just(T&& v) { return just_t<T>{std::forward<T>(v)}; }

}

#endif
