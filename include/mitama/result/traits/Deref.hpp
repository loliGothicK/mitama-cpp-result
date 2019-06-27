#ifndef MITAMA_RESULT_TRAITS_DEREF_HPP
#define MITAMA_RESULT_TRAITS_DEREF_HPP
#include <mitama/result/detail/dangling.hpp>
#include <type_traits>

namespace mitama::traits {

    template < class T, class = void >
    struct is_dereferencable: std::false_type {};

    template < class T >
    struct is_dereferencable<T, std::void_t<decltype(*std::declval<T>())>>: std::true_type {};

    template < class For, class = void >
    struct Deref;

    template < class For >
    struct Deref<For, std::enable_if_t<is_dereferencable<For>::value>> {
        using Target = decltype(*std::declval<For>());
    };

}

#endif
