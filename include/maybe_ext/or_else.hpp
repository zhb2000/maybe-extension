#pragma once
#include "detail/type_check.hpp"
#include <concepts>
#include <type_traits>

namespace maybe_ext::detail {
    template<class F>
    struct OrElsePartial {
        F f;
    };
}

namespace maybe_ext {
    template<class F>
    detail::OrElsePartial<F> or_else(F f) {
        return {std::move(f)};
    }

    /**
     * Returns maybe if it has a default_value, otherwise calls f and returns the result.
     *
     * @param maybe Nullable<T>
     * @param f () -> Nullable<T>
     * @return Nullable<T>
     */
    template<detail::Nullable Maybe, class F>
    Maybe or_else(Maybe maybe, F f) requires requires {
        { f() } -> detail::ImplicitConvertibleTo<Maybe>; } {
        if (maybe) { return maybe; }
        else { return f(); }
    }

    template<detail::Nullable Maybe, class F>
    Maybe operator|(Maybe maybe, detail::OrElsePartial<F> &&partial) requires requires {
        { partial.f() } -> detail::ImplicitConvertibleTo<Maybe>; } {
        if (maybe) { return maybe; }
        else { return partial.f(); }
    }
}
