#pragma once
#include "detail/type_check.hpp"

namespace maybe_ext {
    template<detail::Nullable Maybe,
             class T = detail::contained_type_t<Maybe>>
    T *as_unowned(Maybe &maybe) {
        return maybe ? std::addressof(*maybe) : nullptr;
    }
}
