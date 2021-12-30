#pragma once
#include "detail/type_check.hpp"

namespace maybe_ext::detail {
    template<detail::Nullable Maybe>
    struct OrPartial {
        const Maybe &b;
    };
}

namespace maybe_ext {
    template<detail::Nullable Maybe>
    detail::OrPartial<Maybe> or_(const Maybe &b) {
        return {b};
    }

    /** Returns a if it has a value, otherwise returns b. */
    template<detail::Nullable Maybe>
    const Maybe &or_(const Maybe &a, const Maybe &b) {
        return a ? a : b;
    }

    template<detail::Nullable Maybe>
    const Maybe &operator|(const Maybe &a, detail::OrPartial<Maybe> &&partial) {
        return a ? a : partial.b;
    }
}
