#pragma once

#include <concepts>

namespace maybe_ext::utils {
    template<class To>
    struct Convertor {
        template<class From>
        requires std::convertible_to<From, To>
        To operator()(From &&from) const {
            return static_cast<To>(std::forward<From>(from));
        }
    };

    //region mut_fn, cosnt_fn, rref_fn, const_rref_fn
    template<class Class, class Ret>
    auto mut_fn(Ret (Class::*method)()) { return method; }
    template<class Class, class Ret>
    auto mut_fn(Ret (Class::*method)() noexcept) { return method; }
    template<class Class, class Ret>
    auto mut_fn(Ret (Class::*method)() &) { return method; }
    template<class Class, class Ret>
    auto mut_fn(Ret (Class::*method)() & noexcept) { return method; }

    template<class Class, class Ret>
    auto const_fn(Ret (Class::*method)() const) { return method; }
    template<class Class, class Ret>
    auto const_fn(Ret (Class::*method)() const noexcept) { return method; }
    template<class Class, class Ret>
    auto const_fn(Ret (Class::*method)() const &) { return method; }
    template<class Class, class Ret>
    auto const_fn(Ret (Class::*method)() const & noexcept) { return method; }

    template<class Class, class Ret>
    auto rref_fn(Ret (Class::*method)() &&) { return method; }
    template<class Class, class Ret>
    auto rref_fn(Ret (Class::*method)() && noexcept) { return method; }

    template<class Class, class Ret>
    auto const_rref_fn(Ret (Class::*method)() const &&) { return method; }
    template<class Class, class Ret>
    auto const_rref_fn(Ret (Class::*method)() const && noexcept) { return method; }
    //endregion
}
