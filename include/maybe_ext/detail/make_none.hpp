#pragma once
#include "type_check.hpp"
#include <optional>

namespace maybe_ext::detail {
    template<detail::Option Opt>
    std::remove_cvref_t<Opt> make_none() {
        return std::nullopt;
    }

    template<detail::Pointer Ptr>
    std::remove_cvref_t<Ptr> make_none() {
        return nullptr;
    }
}
