#pragma once/*{export module co_async:awaiter.details.make_awaitable;}*/

#include <cmake/clang_std_modules_source/std.hpp>/*{import std;}*/
#include <co_async/awaiter/task.hpp>/*{import :awaiter.task;}*/
#include <co_async/awaiter/concepts.hpp>/*{import :awaiter.concepts;}*/

namespace co_async {

template <Awaitable A>
A &&ensureAwaitable(A &&a) {
    return std::forward<A>(a);
}

template <class A>
    requires(!Awaitable<A>)
Task<A> ensureAwaitable(A &&a) {
    co_return std::forward<A>(a);
}

template <Awaitable A>
Task<typename AwaitableTraits<A>::RetType> ensureTask(A a) {
    co_return co_await std::move(a);
}

template <class T>
Task<T> ensureTask(Task<T> &&t) {
    return std::move(t);
}

template <class A>
    requires(!Awaitable<A> && std::is_invocable_v<A> &&
             Awaitable<std::invoke_result_t<A>>)
Task<typename AwaitableTraits<std::invoke_result_t<A>>::RetType>
ensureTask(A a) {
    return ensureTask(std::invoke(std::move(a)));
}

} // namespace co_async
