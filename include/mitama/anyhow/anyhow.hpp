#pragma once

#include <mitama/anyhow/error.hpp>
#include <mitama/result/detail/fwd.hpp>

#include <memory>

namespace mitama::anyhow
{
template <class T>
using result =
    result<void_to_monostate_t<T>, std::shared_ptr<::mitama::anyhow::error>>;
}
