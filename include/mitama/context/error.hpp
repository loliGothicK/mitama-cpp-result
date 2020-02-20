#ifndef MITAMA_RESULT_CONTEXT_HPP
#define MITAMA_RESULT_CONTEXT_HPP
#include <tuple>

namespace mitama {
    struct error {
        virtual std::string_view msg() const = 0;
    };
}

#endif
