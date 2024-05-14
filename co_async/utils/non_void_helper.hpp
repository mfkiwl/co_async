#pragma once
#include <co_async/std.hpp>
namespace co_async {
struct Void final {
    explicit Void() = default;
    template <class T>
    friend constexpr T &&operator,(T &&t, Void) {
        return std::forward<T>(t);
    }
    template <class T>
    friend constexpr T &&operator|(Void, T &&t) {
        return std::forward<T>(t);
    }
    friend constexpr void operator|(Void, Void) {}
    char const           *repr() const noexcept {
        return "Void";
    }
    /* static void *operator new(std::size_t) noexcept { */
    /*     static char unused[sizeof(Void)]; */
    /*     return &unused; */
    /* } */
    /*  */
    /* static void operator delete(void *) noexcept {} */
};
template <class T = void>
struct AvoidVoidTrait {
    using Type     = T;
    using RefType  = std::reference_wrapper<T>;
    using CRefType = std::reference_wrapper<T const>;
};
template <>
struct AvoidVoidTrait<void> final {
    using Type     = Void;
    using RefType  = Void;
    using CRefType = Void;
};
template <class T>
using Avoid = typename AvoidVoidTrait<T>::Type;
template <class T>
using AvoidRef = typename AvoidVoidTrait<T>::RefType;
template <class T>
using AvoidCRef = typename AvoidVoidTrait<T>::CRefType;
} // namespace co_async
