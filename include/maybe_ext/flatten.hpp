#pragma once
#include "detail/type_check.hpp"
#include "detail/make_none.hpp"

namespace maybe_ext::detail {
    struct FlattenPartial {};
}

namespace maybe_ext {
    inline detail::FlattenPartial flatten() {
        return {};
    }

    template<detail::Nullable Maybe,
             class Inner = detail::contained_type_t<Maybe>>
             requires detail::Nullable<Inner>
    Inner flatten(Maybe &&maybe) {
        return maybe ? *std::forward<Maybe>(maybe)
                     : detail::make_none<Inner>();
    }

    template<detail::Nullable Maybe,
             class Inner = detail::contained_type_t<Maybe>>
             requires detail::Nullable<Inner>
    Inner operator|(Maybe &&maybe, detail::FlattenPartial &&) {
        return maybe ? *std::forward<Maybe>(maybe)
                     : detail::make_none<Inner>();
    }
}
