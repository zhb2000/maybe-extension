#pragma once

#include "detail/type_check.hpp"
#include "detail/make_none.hpp"
#include "detail/member_ptr_adaptor.hpp"
#include "as_unowned.hpp"

namespace maybe_ext::utils {
    template<detail::Nullable Maybe, class F>
    auto operator>>(Maybe &&maybe, F f) requires requires {
        { f(*std::forward<Maybe>(maybe)) } -> detail::Nullable;
        { f(*std::forward<Maybe>(maybe)) } -> detail::NotLValueRef; } {
        using Ret = decltype(f(*std::forward<Maybe>(maybe)));
        return maybe ? f(*std::forward<Maybe>(maybe))
                     : detail::make_none<Ret>();
    }

    template<detail::Nullable Maybe, class F>
    auto operator>>(Maybe &&maybe, F f) requires requires {
        { f(*std::forward<Maybe>(maybe)) } -> detail::Nullable;
        { f(*std::forward<Maybe>(maybe)) } -> detail::LValueRef; } {
        return maybe ? as_unowned(f(*std::forward<Maybe>(maybe)))
                     : nullptr;
    }

    template<detail::Nullable Maybe, detail::FieldPtr FieldPtr>
    auto operator>>(Maybe &&maybe, FieldPtr field) {
        return std::forward<Maybe>(maybe)
               >> detail::field_ptr_and_then_adaptor(field);
    }
}

namespace maybe_ext::utils {
    template<detail::Nullable Maybe, class Class, class Ret>
    auto operator>>(Maybe &&maybe, Ret (Class::*method)()) {
        return std::forward<Maybe>(maybe)
               >> detail::method_ptr_adaptor(method);
    }
    template<detail::Nullable Maybe, class Class, class Ret>
    auto operator>>(Maybe &&maybe, Ret (Class::*method)() noexcept) {
        return std::forward<Maybe>(maybe)
               >> detail::method_ptr_adaptor(method);
    }
    template<detail::Nullable Maybe, class Class, class Ret>
    auto operator>>(Maybe &&maybe, Ret (Class::*method)() &) {
        return std::forward<Maybe>(maybe)
               >> detail::method_ptr_adaptor(method);
    }
    template<detail::Nullable Maybe, class Class, class Ret>
    auto operator>>(Maybe &&maybe, Ret (Class::*method)() & noexcept) {
        return std::forward<Maybe>(maybe)
               >> detail::method_ptr_adaptor(method);
    }

    template<detail::Nullable Maybe, class Class, class Ret>
    auto operator>>(Maybe &&maybe, Ret (Class::*method)() const) {
        return std::forward<Maybe>(maybe)
               >> detail::method_ptr_adaptor(method);
    }
    template<detail::Nullable Maybe, class Class, class Ret>
    auto operator>>(Maybe &&maybe, Ret (Class::*method)() const noexcept) {
        return std::forward<Maybe>(maybe)
               >> detail::method_ptr_adaptor(method);
    }
    template<detail::Nullable Maybe, class Class, class Ret>
    auto operator>>(Maybe &&maybe, Ret (Class::*method)() const &) {
        return std::forward<Maybe>(maybe)
               >> detail::method_ptr_adaptor(method);
    }
    template<detail::Nullable Maybe, class Class, class Ret>
    auto operator>>(Maybe &&maybe, Ret (Class::*method)() const & noexcept) {
        return std::forward<Maybe>(maybe)
               >> detail::method_ptr_adaptor(method);
    }

    template<detail::Nullable Maybe, class Class, class Ret>
    auto operator>>(Maybe &&maybe, Ret (Class::*method)() &&) {
        return std::forward<Maybe>(maybe)
               >> detail::method_ptr_adaptor(method);
    }
    template<detail::Nullable Maybe, class Class, class Ret>
    auto operator>>(Maybe &&maybe, Ret (Class::*method)() && noexcept) {
        return std::forward<Maybe>(maybe)
               >> detail::method_ptr_adaptor(method);
    }
    template<detail::Nullable Maybe, class Class, class Ret>
    auto operator>>(Maybe &&maybe, Ret (Class::*method)() const &&) {
        return std::forward<Maybe>(maybe)
               >> detail::method_ptr_adaptor(method);
    }
    template<detail::Nullable Maybe, class Class, class Ret>
    auto operator>>(Maybe &&maybe, Ret (Class::*method)() const && noexcept) {
        return std::forward<Maybe>(maybe)
               >> detail::method_ptr_adaptor(method);
    }
}
