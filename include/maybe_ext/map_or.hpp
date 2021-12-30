#pragma once
#include "detail/type_check.hpp"
#include "detail/member_ptr_adaptor.hpp"

namespace maybe_ext::detail {
    template<class U, class F>
    struct MapOrPartial {
        U default_value;
        F f;
    };

    template<class U, class F>
    MapOrPartial(U, F) -> MapOrPartial<U, F>;
}

namespace maybe_ext {
    template<class U, class F>
    detail::MapOrPartial<U, F> map_or(U default_value, F f) {
        return {std::move(default_value), std::move(f)};
    }

//    template<class U, detail::MethodPtr MethodPtr>
//    auto map_or(U default_value, MethodPtr method) {
//        return detail::MapOrPartial{
//            std::move(default_value),
//            detail::method_ptr_adaptor(method)
//        };
//    }

    template<class U, detail::FieldPtr FieldPtr>
    auto map_or(U default_value, FieldPtr field) {
        return detail::MapOrPartial{
            std::move(default_value),
            detail::field_ptr_map_adaptor(field)
        };
    }

    /**
     * Returns the provided default value if maybe is empty,
     * else applies function f to the contained value.
     *
     * @param maybe Nullable<T>
     * @param f (T) -> U or (&T) -> U
     * @return U
     */
    template<detail::Nullable Maybe, class U, class F>
    U map(Maybe &&maybe, U default_value, F f) requires requires {
        { f(*std::forward<Maybe>(maybe)) } -> std::convertible_to<U>; } {
        return maybe ? static_cast<U>(f(*std::forward<Maybe>(maybe)))
                     : std::move(default_value);
    }

    template<detail::Nullable Maybe, class U, class F>
    U operator|(Maybe &&maybe, detail::MapOrPartial<U, F> &&partial) requires requires {
        { partial.f(*std::forward<Maybe>(maybe)) } -> std::convertible_to<U>; } {
        return maybe ? static_cast<U>(partial.f(*std::forward<Maybe>(maybe)))
                     : std::move(partial.default_value);
    }
}

namespace maybe_ext {
    template<class U, class Class, class Ret>
    auto map_or(U default_value, Ret (Class::*method)()) {
        return detail::MapOrPartial{
            std::move(default_value),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or(U default_value, Ret (Class::*method)() noexcept) {
        return detail::MapOrPartial{
            std::move(default_value),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or(U default_value, Ret (Class::*method)() &) {
        return detail::MapOrPartial{
            std::move(default_value),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or(U default_value, Ret (Class::*method)() & noexcept) {
        return detail::MapOrPartial{
            std::move(default_value),
            detail::method_ptr_adaptor(method)
        };
    }

    template<class U, class Class, class Ret>
    auto map_or(U default_value, Ret (Class::*method)() const) {
        return detail::MapOrPartial{
            std::move(default_value),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or(U default_value, Ret (Class::*method)() const &) {
        return detail::MapOrPartial{
            std::move(default_value),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or(U default_value, Ret (Class::*method)() const noexcept) {
        return detail::MapOrPartial{
            std::move(default_value),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or(U default_value, Ret (Class::*method)() const & noexcept) {
        return detail::MapOrPartial{
            std::move(default_value),
            detail::method_ptr_adaptor(method)
        };
    }

    template<class U, class Class, class Ret>
    auto map_or(U default_value, Ret (Class::*method)() &&) {
        return detail::MapOrPartial{
            std::move(default_value),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or(U default_value, Ret (Class::*method)() && noexcept) {
        return detail::MapOrPartial{
            std::move(default_value),
            detail::method_ptr_adaptor(method)
        };
    }

    template<class U, class Class, class Ret>
    auto map_or(U default_value, Ret (Class::*method)() const &&) {
        return detail::MapOrPartial{
            std::move(default_value),
            detail::method_ptr_adaptor(method)
        };
    }
    template<class U, class Class, class Ret>
    auto map_or(U default_value, Ret (Class::*method)() const && noexcept) {
        return detail::MapOrPartial{
            std::move(default_value),
            detail::method_ptr_adaptor(method)
        };
    }
}
