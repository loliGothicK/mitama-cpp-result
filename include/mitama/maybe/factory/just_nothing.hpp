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

namespace mitama {

struct [[nodiscard]] nothing_t {};

inline constexpr nothing_t nothing{};

constexpr bool operator==(const nothing_t, const nothing_t) { return true; }
constexpr bool operator!=(const nothing_t, const nothing_t) { return false; }
constexpr bool operator< (const nothing_t, const nothing_t) { return false; }
constexpr bool operator> (const nothing_t, const nothing_t) { return false; }
constexpr bool operator<=(const nothing_t, const nothing_t) { return true; }
constexpr bool operator>=(const nothing_t, const nothing_t) { return true; }

std::ostream& operator<<(std::ostream& os, nothing_t) { return os << "nothing"; }

template <class, class>
class display {};

template <class T>
class display<just_t<T>, std::enable_if_t<trait::formattable<T>::value> > {
    friend std::ostream& operator<<(std::ostream& os, just_t<T> const& j) {
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
class [[nodiscard]] just_t
    : public display<just_t<T>>
    , private ::mitamagic::perfect_trait_copy_move<
          std::is_copy_constructible_v<std::decay_t<T>>,
          std::conjunction_v<std::is_copy_constructible<std::decay_t<T>>, std::is_copy_assignable<std::decay_t<T>>>,
          std::is_move_constructible_v<std::decay_t<T>>,
          std::conjunction_v<std::is_move_constructible<std::decay_t<T>>, std::is_move_assignable<std::decay_t<T>>>,
          just_t<T>>
{
    T x;

    template <class> friend class just_t;
    template <class,class> friend class display;
    template <class> friend class maybe;

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
};

template <class T>
auto just(T&& v) { return just_t<T>{std::forward<T>(v)}; }


}

#endif
