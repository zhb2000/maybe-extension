#pragma once
#include "detail/type_check.hpp"

namespace maybe_ext::detail {
    template<class U>
    struct ContainsPartial {
        const U &value;
    };
}

namespace maybe_ext {
    template<class U>
    detail::ContainsPartial<U> contains(const U &value) {
        return {value};
    }

    /** Returns true if maybe is not empty and contains the given value. */
    template<detail::Nullable Maybe, class U>
    bool contains(const Maybe &maybe, const U &value) requires requires {
        { *maybe == value } -> std::same_as<bool>; } {
        return maybe && (*maybe == value);
    }

    template<detail::Nullable Maybe, class U>
    bool operator|(const Maybe &maybe, detail::ContainsPartial<U> &&partial) requires requires {
        { *maybe == partial.value } -> std::same_as<bool>; } {
        return maybe && (*maybe == partial.value);
    }
}
