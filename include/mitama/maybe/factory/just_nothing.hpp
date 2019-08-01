#ifndef MITAMA_MAYBE_FACTORY_JUST_HPP
#define MITAMA_MAYBE_FACTORY_JUST_HPP
#include <mitama/maybe/fwd/maybe_fwd.hpp>
#include <mitama/result/detail/meta.hpp>
#include <mitama/result/traits/perfect_traits_special_members.hpp>
#include <mitama/result/traits/impl_traits.hpp>
#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/overload.hpp>
#include <boost/hana/functional/overload_linearly.hpp>

#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace mitama::_just_detail {
    template<class T=void>
    struct forward_mode {};
}

namespace mitama {

struct [[nodiscard]] nothing_t {};

inline constexpr nothing_t nothing{};

constexpr bool operator==(const nothing_t, const nothing_t) { return true; }
constexpr bool operator!=(const nothing_t, const nothing_t) { return false; }
constexpr bool operator< (const nothing_t, const nothing_t) { return false; }
constexpr bool operator> (const nothing_t, const nothing_t) { return false; }
constexpr bool operator<=(const nothing_t, const nothing_t) { return true; }
constexpr bool operator>=(const nothing_t, const nothing_t) { return true; }

inline std::ostream& operator<<(std::ostream& os, nothing_t) { return os << "nothing"; }

template <class>
struct is_just: std::false_type {};

template <class T>
struct is_just<just_t<T>>: std::true_type {};

template <class, class>
struct is_just_with: std::false_type {};

template <class T>
struct is_just_with<just_t<T>, T>: std::true_type {};

/// class just:
/// The main use of this class is to propagate some value to the constructor of the maybe class.
template <class T>
class [[nodiscard]] just_t<T>
    : private ::mitamagic::perfect_trait_copy_move<
          std::is_copy_constructible_v<std::decay_t<T>>,
          std::conjunction_v<std::is_copy_constructible<std::decay_t<T>>, std::is_copy_assignable<std::decay_t<T>>>,
          std::is_move_constructible_v<std::decay_t<T>>,
          std::conjunction_v<std::is_move_constructible<std::decay_t<T>>, std::is_move_assignable<std::decay_t<T>>>,
          just_t<T>>
{
    template <class...> friend class just_t;
    template <class> friend class maybe;

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

    friend constexpr bool
    operator==(const nothing_t, just_t const&) {
        return false;
    }

    friend constexpr bool
    operator==(just_t const&, const nothing_t) {
        return false;
    }

    template <class U>
    constexpr
    std::enable_if_t<
        meta::is_comparable_with<T, U>::value,
    bool>
    operator==(just_t<U> const& rhs) const {
        return this->x == rhs.x;
    }

    template <class U>
    friend
    std::enable_if_t<
        meta::is_comparable_with<T, U>::value,
    bool>
    operator==(maybe<U> const& lhs, just_t const& rhs) {
        return lhs && (lhs.unwrap() == rhs.x);
    }

    template <class U>
    friend
    std::enable_if_t<
        meta::is_comparable_with<T, U>::value,
    bool>
    operator==(just_t const& lhs, maybe<U> const& rhs) {
        return rhs && (lhs.x == rhs.unwrap());
    }

    friend constexpr bool
    operator!=(const nothing_t , just_t const&) {
        return true;
    }

    friend constexpr bool
    operator!=(just_t const&, const nothing_t) {
        return true;
    }

    template <class U>
    constexpr
    std::enable_if_t<
        meta::is_comparable_with<T, U>::value,
    bool>
    operator!=(just_t<U> const& rhs) const{
        return !(this->x == rhs.x);
    }

    template <class U>
    friend
    std::enable_if_t<
        meta::is_comparable_with<T, U>::value,
    bool>
    operator!=(maybe<U> const& lhs, just_t const& rhs) {
        return lhs && !(lhs.unwrap() == rhs);
    }

    template <class U>
    friend
    std::enable_if_t<
        meta::is_comparable_with<T, U>::value,
    bool>
    operator!=(just_t const& lhs, maybe<U> const& rhs) {
        return rhs && !(lhs == rhs.unwrap());
    }

    friend constexpr bool
    operator<(const nothing_t , just_t const&) {
        return true;
    }

    friend constexpr bool
    operator<(just_t const&, const nothing_t) {
        return false;
    }

    template <class U>
    constexpr
    std::enable_if_t<
        meta::is_less_comparable_with<T, U>::value,
    bool>
    operator<(just_t<U> const& rhs) const{
        return this->x < rhs.x;
    }

    template <class U>
    friend
    std::enable_if_t<
        meta::is_less_comparable_with<T, U>::value,
    bool>
    operator<(just_t const& lhs, maybe<U> const& rhs) {
        return rhs ? lhs.x < rhs.unwrap() : false;
    }

    template <class U>
    friend
    std::enable_if_t<
        meta::is_less_comparable_with<T, U>::value,
    bool>
    operator<(maybe<U> const& lhs, just_t const& rhs) {
        return lhs ? lhs.unwrap() < rhs.x : true;
    }

    friend constexpr bool
    operator<=(const nothing_t , just_t const&) {
        return true;
    }

    friend constexpr bool
    operator<=(just_t const&, const nothing_t) {
        return false;
    }

    template <class U>
    constexpr 
    std::enable_if_t<
        std::conjunction_v<
            meta::is_less_comparable_with<T, U>,
            meta::is_comparable_with<T, U>>,
    bool>
    operator<=(just_t<U> const& rhs) const{
        return this->x < rhs.x || this->x == rhs.x ;
    }

    template <class U>
    friend
    std::enable_if_t<
        std::conjunction_v<
            meta::is_less_comparable_with<T, U>,
            meta::is_comparable_with<T, U>>,
    bool>
    operator<=(just_t const& lhs, maybe<U> const& rhs) {
        return rhs ? (lhs.x < rhs.unwrap() || lhs.x == rhs.unwrap()) : false;
    }

    template <class U>
    friend
    std::enable_if_t<
        std::conjunction_v<
            meta::is_less_comparable_with<T, U>,
            meta::is_comparable_with<T, U>>,
    bool>
    operator<=(maybe<U> const& lhs, just_t const& rhs) {
        return lhs ? (lhs.unwrap() < rhs.x || lhs.unwrap() == rhs.x) : true;
    }

    friend constexpr bool
    operator>(const nothing_t , just_t const&) {
        return false;
    }

    friend constexpr bool
    operator>(just_t const&, const nothing_t) {
        return true;
    }

    template <class U>
    std::enable_if_t<
        meta::is_comparable_with<T, U>::value,
    bool>
    operator>(just_t<U> const& rhs) const{
        return rhs < *this;
    }

    template <class U>
    friend
    std::enable_if_t<
        meta::is_less_comparable_with<T, U>::value,
    bool>
    operator>(just_t const& lhs, maybe<U> const& rhs) {
        return rhs < lhs;
    }

    template <class U>
    friend
    std::enable_if_t<
        meta::is_less_comparable_with<T, U>::value,
    bool>
    operator>(maybe<U> const& lhs, just_t const& rhs) {
        return rhs < lhs;
    }

    friend constexpr bool
    operator>=(const nothing_t , just_t const&) {
        return false;
    }

    friend constexpr bool
    operator>=(just_t const&, const nothing_t) {
        return true;
    }

    template <class U>
    std::enable_if_t<
        std::conjunction_v<
            meta::is_less_comparable_with<T, U>,
            meta::is_comparable_with<T, U>>,
    bool>
    operator>=(just_t<U> const& rhs) const{
        return rhs <= *this;
    }

    template <class U>
    friend
    std::enable_if_t<
        std::conjunction_v<
            meta::is_less_comparable_with<T, U>,
            meta::is_comparable_with<T, U>>,
    bool>
    operator>=(just_t const& lhs, maybe<U> const& rhs) {
        return rhs <= lhs;
    }

    template <class U>
    friend
    std::enable_if_t<
        std::conjunction_v<
            meta::is_less_comparable_with<T, U>,
            meta::is_comparable_with<T, U>>,
    bool>
    operator>=(maybe<U> const& lhs, just_t const& rhs) {
        return rhs <= lhs;
    }

    friend std::ostream& operator<<(std::ostream& os, just_t const& j) {
        static_assert(trait::formattable<T>::value,
            "Error: trait `formattable<T>` was not satisfied.\n"
            "Hint: Did you forgot some include?\n"
            "Otherwise, please define `operator<<(std::ostream&, T)`.");

        using namespace std::string_literals;
        using namespace std::string_view_literals;
        auto inner_format = boost::hana::fix(boost::hana::overload_linearly(
            [](auto, auto const& x) -> std::enable_if_t<trait::formattable_element<std::decay_t<decltype(x)>>::value, std::string> {
                return boost::hana::overload_linearly(
                [](std::monostate) { return "()"s; },
                [](std::string_view x) { return (boost::format("\"%1%\"") % x).str(); },
                [](auto const& x) { return (boost::format("%1%") % x).str(); })
                (x);
            },
            [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_dictionary<std::decay_t<decltype(x)>>::value, std::string> {
                if (x.empty()) return "{}"s;
                using std::begin, std::end;
                auto iter = begin(x);
                std::string str = "{"s + (boost::format("%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
                while (++iter != end(x)) {
                str += (boost::format(",%1%: %2%") % _fmt(std::get<0>(*iter)) % _fmt(std::get<1>(*iter))).str();
                }
                return str += "}";
            },
            [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_range<std::decay_t<decltype(x)>>::value, std::string> {
                if (x.empty()) return "[]"s;
                using std::begin, std::end;
                auto iter = begin(x);
                std::string str = "["s + _fmt(*iter);
                while (++iter != end(x)) {
                str += (boost::format(",%1%") % _fmt(*iter)).str();
                }
                return str += "]";
            },
            [](auto _fmt, auto const& x) -> std::enable_if_t<trait::formattable_tuple<std::decay_t<decltype(x)>>::value, std::string> {
                if constexpr (std::tuple_size_v<std::decay_t<decltype(x)>> == 0) {
                return "()"s;
                }
                else {
                return std::apply(
                    [_fmt](auto const& head, auto const&... tail) {
                    return "("s + _fmt(head) + ((("," + _fmt(tail))) + ...) + ")"s;
                    }, x);
                }
            }));
        return os << boost::format("just(%1%)") % inner_format(j.x);
    }

};

template <class Target = void, class... Types>
auto just(Types&&... v) {
    if constexpr (sizeof...(Types) > 1) {
        return just_t<_just_detail::forward_mode<Target>, Types&&...>{std::forward<Types>(v)...};
    }
    else {
        if constexpr (!std::is_void_v<Target>)
            return just_t<_just_detail::forward_mode<Target>, Types&&...>{std::forward<Types>(v)...};
        else
            return just_t<Types...>{std::forward<Types>(v)...};
    }
}

template <class Target = void, class T, class... Types>
auto just(std::initializer_list<T> il, Types&&... v) {
    return just_t<_just_detail::forward_mode<Target>, std::initializer_list<T>, Types&&...>{il, std::forward<Types>(v)...};
}

template <class T, class... Args>
class [[nodiscard]] just_t<_just_detail::forward_mode<T>, Args...>
    : private ::mitamagic::perfect_trait_copy_move<false, false, false, false,
        just_t<_just_detail::forward_mode<T>, Args...>>
{
    std::tuple<Args...> args;
public:
    constexpr explicit just_t(Args... args): args(std::forward<Args>(args)...) {}

    auto operator()() && {
        return std::apply([](auto&&... fwd){ return std::forward_as_tuple(std::forward<decltype(fwd)>(fwd)...); }, args);
    }
};
}

#endif
