#pragma once

#include <utility>
#include <co_async/task.hpp>
#include <co_async/concepts.hpp>

namespace co_async {

template <Awaitable A> A &&make_awaitable(A &&a) {
    return std::forward<A>(a);
}

template <class A>
    requires(!Awaitable<A>)
Task<A> make_awaitable(A &&a) {
    co_return std::forward<A>(a);
}

} // namespace co_async
