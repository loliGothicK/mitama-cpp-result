#ifndef MITAMA_BOOLINATORS_HPP
#define MITAMA_BOOLINATORS_HPP
#include <mitama/maybe/maybe.hpp>
#include <mitama/result/result.hpp>
#include <type_traits>
#include <utility>

namespace mitama {
    inline maybe<std::monostate>
    as_maybe(bool b)
        { return b ? maybe<std::monostate>{std::in_place} : nothing; }

    template <class T>
    inline maybe<T>
    as_just(bool b, T&& some)
        { return b ? maybe<T>{std::forward<T>(some)} : nothing; }

    template <class F>
    inline maybe<std::invoke_result_t<F&&>>
    as_just_from(bool b, F&& some)
        { return b ? maybe{std::invoke(std::forward<F>(some))} : nothing; }

    template <class T>
    inline maybe<T>
    and_maybe(bool b, maybe<T> const &may)
        { return b ? may : nothing; }

    template <class F>
    inline auto
    and_maybe_from(bool b, F &&may)
        -> std::enable_if_t<
            std::conjunction_v<
                std::is_invocable<F&&>,
                is_maybe<std::invoke_result_t<F&&>>
            >,
            std::invoke_result_t<F&&>
        >
        { return b ? std::invoke(std::forward<F>(may)) : nothing; }

    template <class T>
    inline result<T>
    as_ok(bool b, T&& ok) {
        return b ? result<T>(success(std::forward<T>(ok))) : failure();
    }

    template <class T, class E>
    inline result<T, E>
    as_result(bool b, T&& ok, E&& err) {
        return b ? result<T, E>{success(std::forward<T>(ok))}
                 : result<T, E>{failure(std::forward<E>(err))};
    }

    template <class F, class G>
    inline result<std::invoke_result_t<F>, std::invoke_result_t<G&&>>
    as_result_from(bool b, F&& ok, G&& err) {
        using result_type = result<std::invoke_result_t<F>, std::invoke_result_t<G&&>>;
        return b ? result_type{success(std::invoke(std::forward<F>(ok)))}
                 : result_type{failure(std::invoke(std::forward<G>(err)))};
    }

    template <class E>
    inline result<std::monostate, E>
    ok_or(bool b, E&& err) {
        return b ? result<std::monostate, E>{success()}
                 : result<std::monostate, E>{failure(std::forward<E>(err))};
    }

    template <class G>
    inline result<std::monostate, std::invoke_result_t<G&&>>
    ok_or_else(bool b, G&& err) {
        return b ? result<std::monostate, std::invoke_result_t<G>>{success()}
                 : result<std::monostate, std::invoke_result_t<G>>{failure(std::invoke(std::forward<G>(err)))};
    }
}

#endif
