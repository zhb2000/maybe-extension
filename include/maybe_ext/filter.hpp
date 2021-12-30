#pragma once

#include "detail/type_check.hpp"
#include "detail/make_none.hpp"

namespace maybe_ext::detail {
    template<class P>
    struct FilterPartial {
        P predicate;
    };
}

namespace maybe_ext {
    template<class P>
    detail::FilterPartial<P> filter(P f) {
        return {std::move(f)};
    }

    /**
     * Returns maybe itself if maybe has value and the predicate returns true,
     * otherwise returns nullopt or nullptr (according to the type of maybe).
     *
     * @param maybe Maybe<T>
     * @param predicate (&T) -> bool
     * @return Maybe<T>
     */
    template<detail::Nullable Maybe, class P>
    Maybe filter(Maybe maybe, P predicate) requires requires {
        { predicate(*maybe) } -> std::same_as<bool>; } {
        if (maybe && predicate(*maybe)) { return maybe; }
        else { return detail::make_none<Maybe>();}
    }

    template<detail::Nullable Maybe, class P>
    auto operator|(Maybe maybe, detail::FilterPartial<P> &&partial) requires requires {
        { partial.predicate(*maybe) } -> std::same_as<bool>; } {
        if (maybe && partial.predicate(*maybe)) { return maybe; }
        else { return detail::make_none<Maybe>(); }
    }
}