#pragma once

#include "detail/type_check.hpp"
#include <optional>

namespace maybe_ext::detail {
    struct ClonedPartial {};
}

namespace maybe_ext {
    inline detail::ClonedPartial cloned() {
        return {};
    }

    template<detail::Pointer Maybe,
             class T = std::remove_cv_t<detail::contained_type_t<Maybe>>>
             requires std::copy_constructible<T>
    std::optional<T> cloned(const Maybe &maybe) {
        return maybe ? std::optional(*maybe) : std::nullopt;
    }

    template<detail::Pointer Maybe,
             class T = std::remove_cv_t<detail::contained_type_t<Maybe>>>
             requires std::copy_constructible<T>
    std::optional<T> operator|(const Maybe &maybe, detail::ClonedPartial &&) {
        return maybe ? std::optional(*maybe) : std::nullopt;
    }
}
