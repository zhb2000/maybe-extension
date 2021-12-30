#pragma once

#include "detail/type_check.hpp"
#include "detail/make_none.hpp"

namespace maybe_ext::detail {
    template<detail::Nullable Maybe>
    struct XorPartial {
        Maybe b;
    };
}

namespace maybe_ext {
    template<detail::Nullable Maybe>
    detail::XorPartial<Maybe> xor_(Maybe b) {
        return {std::move(b)};
    }

    template<detail::Nullable Maybe>
    Maybe xor_(Maybe a, Maybe b) {
        if (a && !b) { return a; }
        else if (!a && b) { return b; }
        else { return detail::make_none<Maybe>(); }
    }

    template<detail::Nullable Maybe>
    Maybe operator|(Maybe a, detail::XorPartial<Maybe> &&partial) {
        if (a && !partial.b) { return a; }
        else if (!a && partial.b) { return std::move(partial.b); }
        else { return detail::make_none<Maybe>(); }
    }
}
