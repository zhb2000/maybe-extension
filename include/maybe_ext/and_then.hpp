#pragma once
#include "detail/type_check.hpp"
#include "detail/make_none.hpp"
#include "detail/member_ptr_adaptor.hpp"
#include "maybe_ext/as_unowned.hpp"

namespace maybe_ext::detail {
    template<class F>
    struct AndThenPartial {
        F f;
    };

    template<class F>
    AndThenPartial(F) -> AndThenPartial<F>;
}

namespace maybe_ext {
    template<class F>
    detail::AndThenPartial<F> and_then(F f) {
        return {std::move(f)};
    }

//    template<detail::MethodPtr MethodPtr>
//    auto and_then(MethodPtr method) {
//        return detail::AndThenPartial{
//            detail::method_ptr_adaptor(method)
//        };
//    }

    template<detail::FieldPtr FieldPtr>
    auto and_then(FieldPtr field) {
        return detail::AndThenPartial{
            detail::field_ptr_and_then_adaptor(field)
        };
    }

    /**
     * Returns nullopt or nullptr (according to the type of maybe) if maybe is empty,
     * otherwise calls f with the wrapped default_value and returns the result.
     * <p/>
     * This operation is also known as flatmap in other languages.
     *
     * @param maybe Maybe<T>
     * @param f (T) -> Maybe<U> or (T&) -> Maybe<U>
     * @return Maybe<U>
     */
    template<detail::Nullable Maybe, class F>
    auto and_then(Maybe &&maybe, F f) requires requires {
        { f(*std::forward<Maybe>(maybe)) } -> detail::Nullable;
        { f(*std::forward<Maybe>(maybe)) } -> detail::NotLValueRef; } {
        using Ret = decltype(f(*std::forward<Maybe>(maybe)));
        return maybe ? f(*std::forward<Maybe>(maybe))
                     : detail::make_none<Ret>();
    }

    template<detail::Nullable Maybe, class F>
    auto operator|(Maybe &&maybe, detail::AndThenPartial<F> &&partial) requires requires {
        { partial.f(*std::forward<Maybe>(maybe)) } -> detail::Nullable;
        { partial.f(*std::forward<Maybe>(maybe)) } -> detail::NotLValueRef; } {
        using Ret = decltype(partial.f(*std::forward<Maybe>(maybe)));
        return maybe ? partial.f(*std::forward<Maybe>(maybe))
                     : detail::make_none<Ret>();
    }

    template<detail::Nullable Maybe, class F>
    auto and_then(Maybe &&maybe, F f) requires requires {
        { f(*std::forward<Maybe>(maybe)) } -> detail::Nullable;
        { f(*std::forward<Maybe>(maybe)) } -> detail::LValueRef; } {
        return maybe ? as_unowned(f(*std::forward<Maybe>(maybe)))
                     : nullptr;
    }

    template<detail::Nullable Maybe, class F>
    auto operator|(Maybe &&maybe, detail::AndThenPartial<F> &&partial) requires requires {
        { partial.f(*std::forward<Maybe>(maybe)) } -> detail::Nullable;
        { partial.f(*std::forward<Maybe>(maybe)) } -> detail::LValueRef; } {
        return maybe ? as_unowned(partial.f(*std::forward<Maybe>(maybe)))
                     : nullptr;
    }
}

namespace maybe_ext {
    template<class Class, class Ret>
    auto and_then(Ret (Class::*method)()) {
        return detail::AndThenPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto and_then(Ret (Class::*method)() noexcept) {
        return detail::AndThenPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto and_then(Ret (Class::*method)() &) {
        return detail::AndThenPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto and_then(Ret (Class::*method)() & noexcept) {
        return detail::AndThenPartial{
            detail::method_ptr_adaptor(method)
        };
    }

    template<class Class, class Ret>
    auto and_then(Ret (Class::*method)() const) {
        return detail::AndThenPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto and_then(Ret (Class::*method)() const &) {
        return detail::AndThenPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto and_then(Ret (Class::*method)() const noexcept) {
        return detail::AndThenPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto and_then(Ret (Class::*method)() const & noexcept) {
        return detail::AndThenPartial{
            detail::method_ptr_adaptor(method)
        };
    }

    template<class Class, class Ret>
    auto and_then(Ret (Class::*method)() &&) {
        return detail::AndThenPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto and_then(Ret (Class::*method)() && noexcept) {
        return detail::AndThenPartial{
            detail::method_ptr_adaptor(method)
        };
    }

    template<class Class, class Ret>
    auto and_then(Ret (Class::*method)() const &&) {
        return detail::AndThenPartial{
            detail::method_ptr_adaptor(method)
        };
    }
    template<class Class, class Ret>
    auto and_then(Ret (Class::*method)() const && noexcept) {
        return detail::AndThenPartial{
            detail::method_ptr_adaptor(method)
        };
    }
}
