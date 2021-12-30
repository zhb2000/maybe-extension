#pragma once
#include "detail/type_check.hpp"
#include "detail/member_ptr_adaptor.hpp"

namespace maybe_ext::detail {
    template<class F>
    struct MapPartial {
        F f;
    };

    template<class F>
    MapPartial(F) -> MapPartial<F>;
}

namespace maybe_ext {
    template<class F>
    detail::MapPartial<F> map(F f) {
        return {std::move(f)};
    }

//    template<detail::MethodPtr MethodPtr>
//    auto map(MethodPtr method) {
//        return detail::MapPartial{detail::method_ptr_adaptor(method)};
//    }

    template<detail::FieldPtr FieldPtr>
    auto map(FieldPtr field) {
        return detail::MapPartial{detail::field_ptr_map_adaptor(field)};
    }

    /**
     * @param maybe Maybe<T>
     * @param f (T) -> U or (&T) -> U, where U is not a lvalue reference.
     * That means the result of f is value semantic. We use std::optional to represent nullable values.
     * @return std::optional<U>
     */
    template<detail::Nullable Maybe, class F>
    auto map(Maybe &&maybe, F f) requires requires {
        { f(*std::forward<Maybe>(maybe)) } -> detail::NotLValueRef; } {
        return maybe ? std::optional(f(*std::forward<Maybe>(maybe)))
                     : std::nullopt;
    }

    template<detail::Nullable Maybe, class F>
    auto operator|(Maybe &&maybe, detail::MapPartial<F> &&partial) requires requires {
        { partial.f(*std::forward<Maybe>(maybe)) } -> detail::NotLValueRef; } {
        return maybe ? std::optional(partial.f(*std::forward<Maybe>(maybe)))
                     : std::nullopt;
    }

    /**
     * @param maybe Maybe<T>
     * @param f (T) -> U& or (&T) -> U&, where U& is a lvalue reference.
     * That means the result of f is reference semantic. We use pointers to represent nullable references.
     * @return U*
     */
    template<detail::Nullable Maybe, class F>
    auto map(Maybe &&maybe, F f) requires requires {
        { f(*std::forward<Maybe>(maybe)) } -> detail::LValueRef; } {
        return maybe ? std::addressof(f(*std::forward<Maybe>(maybe)))
                     : nullptr;
    }

    template<detail::Nullable Maybe, class F>
    auto operator|(Maybe &&maybe, detail::MapPartial<F> &&partial) requires requires {
        { partial.f(*std::forward<Maybe>(maybe)) } -> detail::LValueRef; } {
        return maybe ? std::addressof(partial.f(*std::forward<Maybe>(maybe)))
                     : nullptr;
    }
}

namespace maybe_ext {
    template<class Class, class Ret>
    auto map(Ret (Class::*method)()) {
        return detail::MapPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto map(Ret (Class::*method)() noexcept) {
        return detail::MapPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto map(Ret (Class::*method)() &) {
        return detail::MapPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto map(Ret (Class::*method)() & noexcept) {
        return detail::MapPartial{
            detail::method_ptr_adaptor(method)
        };
    }

    template<class Class, class Ret>
    auto map(Ret (Class::*method)() const) {
        return detail::MapPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto map(Ret (Class::*method)() const &) {
        return detail::MapPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto map(Ret (Class::*method)() const noexcept) {
        return detail::MapPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto map(Ret (Class::*method)() const & noexcept) {
        return detail::MapPartial{
            detail::method_ptr_adaptor(method)
        };
    }

    template<class Class, class Ret>
    auto map(Ret (Class::*method)() &&) {
        return detail::MapPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto map(Ret (Class::*method)() && noexcept) {
        return detail::MapPartial{
            detail::method_ptr_adaptor(method)
        };
    }

    template<class Class, class Ret>
    auto map(Ret (Class::*method)() const &&) {
        return detail::MapPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto map(Ret (Class::*method)() const && noexcept) {
        return detail::MapPartial{
            detail::method_ptr_adaptor(method)
        };
    }
}
