#pragma once
#include "detail/type_check.hpp"
#include "detail/make_none.hpp"

namespace maybe_ext::detail {
    template<detail::Nullable Maybe>
    struct AndPartial {
        Maybe b;
    };
}

namespace maybe_ext {
    template<detail::Nullable Maybe>
    detail::AndPartial<Maybe> and_(Maybe b) {
        return {std::move(b)};
    }

    /** Returns nullopt or nullptr (according to the type of b) if a is empty, otherwise returns b. */
    template<detail::Nullable MaybeA, detail::Nullable MaybeB>
    MaybeB and_(const MaybeA &a, MaybeB b) {
        if (!a) { return detail::make_none<MaybeB>(); }
        else { return b; }
    }

    template<detail::Nullable MaybeA, detail::Nullable MaybeB>
    MaybeB operator|(const MaybeA &a, detail::AndPartial<MaybeB> &&partial) {
        if (!a) { return detail::make_none<MaybeB>(); }
        else { return std::move(partial.b); }
    }
}
