#pragma once

#include "detail/type_check.hpp"

namespace maybe_ext::detail {
    template<class F>
    struct ValueOrElsePartial {
        F f;
    };
}

namespace maybe_ext {
    template<class F>
    detail::ValueOrElsePartial<F> value_or_else(F f) {
        return {std::move(f)};
    }

    template<detail::Nullable Maybe, class F,
             class T = std::remove_cv_t<detail::contained_type_t<Maybe>>>
    T value_or_else(Maybe &&maybe, F f) requires requires {
        { f() } -> std::convertible_to<T>; } {
        return maybe ? *std::forward<Maybe>(maybe)
                     : static_cast<T>(f());
    }

    template<detail::Nullable Maybe, class F,
             class T = std::remove_cv_t<detail::contained_type_t<Maybe>>>
    T operator|(Maybe &&maybe, detail::ValueOrElsePartial<F> &&partial) requires requires {
        { partial.f() } -> std::convertible_to<T>; } {
        return maybe ? *std::forward<Maybe>(maybe)
                     : static_cast<T>(partial.f());
    }
}
