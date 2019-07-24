#ifndef MITAMA_RESULT_DETAIL_DANGLING_HPP
#define MITAMA_RESULT_DETAIL_DANGLING_HPP
#include <utility>
#include <functional>

namespace mitama {

    template < class T >
    class dangling {
        T value_;
    public:
        template < class U >
        constexpr dangling(U&& u): value_(std::forward<U>(u)) {}

        decltype(auto) transmute() const {
            return value_;
        }
    };

    template < class T >
    class dangling<std::reference_wrapper<T>> {
        std::reference_wrapper<T> ref_;
    public:
        constexpr dangling(std::reference_wrapper<T> ref): ref_(ref) {}

        T& transmute() const&  {
            return ref_.get();
        }
    };

    template < class T >
    using dangle_ref = dangling<std::reference_wrapper<std::remove_reference_t<T>>>;
}

#endif
