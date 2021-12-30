#pragma once
#include "detail/type_check.hpp"

namespace maybe_ext::detail {
    template<class U>
    struct ValueOrPartial {
        U default_value;
    };
}

namespace maybe_ext {
    template<class U>
    detail::ValueOrPartial<U> value_or(U default_value) {
        return {std::move(default_value)};
    }

    template<detail::Nullable Maybe, class U,
             class T = std::remove_cv_t<detail::contained_type_t<Maybe>>>
             requires std::convertible_to<U, T>
    T value_or(Maybe &&maybe, U &&default_value) {
        return maybe ? *std::forward<Maybe>(maybe)
                     : static_cast<T>(std::forward<U>(default_value));
    }

    template<detail::Nullable Maybe, class U,
             class T = std::remove_cv_t<detail::contained_type_t<Maybe>>>
             requires std::convertible_to<U, T>
    T operator|(Maybe &&maybe, detail::ValueOrPartial<U> &&partial) {
        return maybe ? *std::forward<Maybe>(maybe)
                     : static_cast<T>(std::move(partial.default_value));
    }
}
