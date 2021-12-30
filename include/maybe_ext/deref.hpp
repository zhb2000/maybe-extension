#pragma once
#include "detail/type_check.hpp"

namespace maybe_ext::detail {
    struct DerefPartial {};
}

namespace maybe_ext {
    inline detail::DerefPartial deref() {
        return {};
    }

    template<detail::Nullable Maybe>
    decltype(auto) deref(Maybe &&maybe) {
        return *std::forward<Maybe>(maybe);
    }

    template<detail::Nullable Maybe>
    decltype(auto) operator|(Maybe &&maybe, detail::DerefPartial &&) {
        return *std::forward<Maybe>(maybe);
    }
}
