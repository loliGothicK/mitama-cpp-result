#ifndef MITAMA_RESULT_DETAIL_DANGLING_HPP
#define MITAMA_RESULT_DETAIL_DANGLING_HPP

namespace mitama {

    template < class T >
    class dangling {
        T value_;
    public:
        template < class U >
        constexpr dangling(U&& u): value_(std::forward<U>(u)) {}

        T transmute() const {
            return value_;
        }
    };

}

#endif
