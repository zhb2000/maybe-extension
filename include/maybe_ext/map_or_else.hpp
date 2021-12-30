#pragma once

#include "detail/type_check.hpp"
#include "detail/member_ptr_adaptor.hpp"

namespace maybe_ext::detail {
    template<class D, class F>
    struct MapOrElsePartial {
        D default_provider;
        F f;
    };

    template<class D, class F>
    MapOrElsePartial(D, F) -> MapOrElsePartial<D, F>;
}

namespace maybe_ext {
    template<class D, class F>
    detail::MapOrElsePartial<D, F> map_or_else(D default_provider, F f) {
        return {std::move(default_provider), std::move(f)};
    }

//    template<class D, detail::MethodPtr MethodPtr>
//    auto map_or_else(D default_provider, MethodPtr method) {
//        return detail::MapOrElsePartial{
//            std::move(default_provider),
//            detail::method_ptr_adaptor(method)
//        };
//    }

    template<class D, detail::FieldPtr FieldPtr>
    auto map_or_else(D default_provider, FieldPtr field) {
        return detail::MapOrElsePartial{
            std::move(default_provider),
            detail::field_ptr_map_adaptor(field)
        };
    }

    template<detail::Nullable Maybe, class D, class F,
             class U = std::remove_cvref_t<std::invoke_result_t<D>>>
    U map_or_else(Maybe &&maybe, D default_provider, F f)requires requires {
        { default_provider() } -> std::same_as<D>;
        { f(*std::forward<Maybe>(maybe)) } -> std::convertible_to<U>; } {
        return maybe ? static_cast<U>(f(*std::forward<Maybe>(maybe)))
                     : default_provider();
    }

    template<detail::Nullable Maybe, class D, class F,
             class U = std::remove_cvref_t<std::invoke_result_t<D>>>
    U operator|(Maybe &&maybe, detail::MapOrElsePartial<D, F> &&partial) requires requires {
        { partial.default_provider() } -> std::same_as<U>;
        { partial.f(*std::forward<Maybe>(maybe)) } -> std::convertible_to<U>; } {
        return maybe ? static_cast<U>(partial.f(*std::forward<Maybe>(maybe)))
                     : partial.default_provider();
    }
}

namespace maybe_ext {
    template<class U, class Class, class Ret>
    auto map_or_else(U default_provider, Ret (Class::*method)()) {
        return detail::MapOrElsePartial{
            std::move(default_provider),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or_else(U default_provider, Ret (Class::*method)() noexcept) {
        return detail::MapOrElsePartial{
            std::move(default_provider),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or_else(U default_provider, Ret (Class::*method)() &) {
        return detail::MapOrElsePartial{
            std::move(default_provider),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or_else(U default_provider, Ret (Class::*method)() & noexcept) {
        return detail::MapOrElsePartial{
            std::move(default_provider),
            detail::method_ptr_adaptor(method)
        };
    }

    template<class U, class Class, class Ret>
    auto map_or_else(U default_provider, Ret (Class::*method)() const) {
        return detail::MapOrElsePartial{
            std::move(default_provider),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or_else(U default_provider, Ret (Class::*method)() const &) {
        return detail::MapOrElsePartial{
            std::move(default_provider),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or_else(U default_provider, Ret (Class::*method)() const noexcept) {
        return detail::MapOrElsePartial{
            std::move(default_provider),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or_else(U default_provider, Ret (Class::*method)() const & noexcept) {
        return detail::MapOrElsePartial{
            std::move(default_provider),
            detail::method_ptr_adaptor(method)
        };
    }

    template<class U, class Class, class Ret>
    auto map_or_else(U default_provider, Ret (Class::*method)() &&) {
        return detail::MapOrElsePartial{
            std::move(default_provider),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or_else(U default_provider, Ret (Class::*method)() && noexcept) {
        return detail::MapOrElsePartial{
            std::move(default_provider),
            detail::method_ptr_adaptor(method)
        };
    }

    template<class U, class Class, class Ret>
    auto map_or_else(U default_provider, Ret (Class::*method)() const &&) {
        return detail::MapOrElsePartial{
            std::move(default_provider),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or_else(U default_provider, Ret (Class::*method)() const && noexcept) {
        return detail::MapOrElsePartial{
            std::move(default_provider),
            detail::method_ptr_adaptor(method)
        };
    }
}
