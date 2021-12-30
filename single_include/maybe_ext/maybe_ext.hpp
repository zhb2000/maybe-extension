#pragma once
#ifndef MAYBE_EXT_HPP
#define MAYBE_EXT_HPP

// region type_check.hpp

#include <concepts>
#include <optional>
#include <type_traits>

namespace maybe_ext::detail {
    template<class T>
    concept LValueRef = std::is_lvalue_reference_v<T>;

    template<class T>
    concept NotLValueRef = !std::is_lvalue_reference_v<T>;

    template<class T>
    concept Option =
    std::is_constructible_v<std::remove_reference_t<T>, std::nullopt_t> &&
    std::is_constructible_v<bool, T> && requires(T maybe) {
        { *maybe } -> LValueRef;
        { maybe == std::nullopt } -> std::same_as<bool>;
        { maybe != std::nullopt } -> std::same_as<bool>;
    };

    template<class T>
    concept Pointer =
    std::is_constructible_v<std::remove_reference_t<T>, std::nullptr_t> &&
    std::is_constructible_v<bool, T> && requires(T maybe) {
        { *maybe } -> LValueRef;
        { maybe == nullptr } -> std::same_as<bool>;
        { maybe != nullptr } -> std::same_as<bool>;
    };

    template<class T>
    concept Nullable = Option<T> || Pointer<T>;

    template<Nullable Maybe>
    using contained_type_t = std::remove_reference_t<decltype(*std::declval<Maybe>())>;

    template<class From, class To>
    concept ImplicitConvertibleTo = std::is_convertible_v<From, To>;

    template<class T>
    concept MethodPtr = std::is_member_function_pointer_v<T>;

    template<class T>
    concept FieldPtr = std::is_member_object_pointer_v<T>;
}

// endregion

// region make_none.hpp

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

// endregion

// region xor.hpp


namespace maybe_ext::detail {
    template<detail::Nullable Maybe>
    struct XorPartial {
        Maybe b;
    };
}

namespace maybe_ext {
    template<detail::Nullable Maybe>
    detail::XorPartial<Maybe> xor_(Maybe b) {
        return {std::move(b)};
    }

    template<detail::Nullable Maybe>
    Maybe xor_(Maybe a, Maybe b) {
        if (a && !b) { return a; }
        else if (!a && b) { return b; }
        else { return detail::make_none<Maybe>(); }
    }

    template<detail::Nullable Maybe>
    Maybe operator|(Maybe a, detail::XorPartial<Maybe> &&partial) {
        if (a && !partial.b) { return a; }
        else if (!a && partial.b) { return std::move(partial.b); }
        else { return detail::make_none<Maybe>(); }
    }
}

// endregion

// region value_or_else.hpp


namespace maybe_ext::detail {
    template<class F>
    struct ValueOrElsePartial {
        F f;
    };
}

namespace maybe_ext {
    template<class F>
    detail::ValueOrElsePartial<F> value_or_else(F f) {
        return {std::move(f)};
    }

    template<detail::Nullable Maybe, class F,
             class T = std::remove_cv_t<detail::contained_type_t<Maybe>>>
    T value_or_else(Maybe &&maybe, F f) requires requires {
        { f() } -> std::convertible_to<T>; } {
        return maybe ? *std::forward<Maybe>(maybe)
                     : static_cast<T>(f());
    }

    template<detail::Nullable Maybe, class F,
             class T = std::remove_cv_t<detail::contained_type_t<Maybe>>>
    T operator|(Maybe &&maybe, detail::ValueOrElsePartial<F> &&partial) requires requires {
        { partial.f() } -> std::convertible_to<T>; } {
        return maybe ? *std::forward<Maybe>(maybe)
                     : static_cast<T>(partial.f());
    }
}

// endregion

// region value_or.hpp


namespace maybe_ext::detail {
    template<class U>
    struct ValueOrPartial {
        U default_value;
    };
}

namespace maybe_ext {
    template<class U>
    detail::ValueOrPartial<U> value_or(U default_value) {
        return {std::move(default_value)};
    }

    template<detail::Nullable Maybe, class U,
             class T = std::remove_cv_t<detail::contained_type_t<Maybe>>>
             requires std::convertible_to<U, T>
    T value_or(Maybe &&maybe, U &&default_value) {
        return maybe ? *std::forward<Maybe>(maybe)
                     : static_cast<T>(std::forward<U>(default_value));
    }

    template<detail::Nullable Maybe, class U,
             class T = std::remove_cv_t<detail::contained_type_t<Maybe>>>
             requires std::convertible_to<U, T>
    T operator|(Maybe &&maybe, detail::ValueOrPartial<U> &&partial) {
        return maybe ? *std::forward<Maybe>(maybe)
                     : static_cast<T>(std::move(partial.default_value));
    }
}

// endregion

// region utils.hpp

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

// endregion

// region or_else.hpp

#include <concepts>
#include <type_traits>

namespace maybe_ext::detail {
    template<class F>
    struct OrElsePartial {
        F f;
    };
}

namespace maybe_ext {
    template<class F>
    detail::OrElsePartial<F> or_else(F f) {
        return {std::move(f)};
    }

    /**
     * Returns maybe if it has a default_value, otherwise calls f and returns the result.
     *
     * @param maybe Nullable<T>
     * @param f () -> Nullable<T>
     * @return Nullable<T>
     */
    template<detail::Nullable Maybe, class F>
    Maybe or_else(Maybe maybe, F f) requires requires {
        { f() } -> detail::ImplicitConvertibleTo<Maybe>; } {
        if (maybe) { return maybe; }
        else { return f(); }
    }

    template<detail::Nullable Maybe, class F>
    Maybe operator|(Maybe maybe, detail::OrElsePartial<F> &&partial) requires requires {
        { partial.f() } -> detail::ImplicitConvertibleTo<Maybe>; } {
        if (maybe) { return maybe; }
        else { return partial.f(); }
    }
}

// endregion

// region or.hpp


namespace maybe_ext::detail {
    template<detail::Nullable Maybe>
    struct OrPartial {
        const Maybe &b;
    };
}

namespace maybe_ext {
    template<detail::Nullable Maybe>
    detail::OrPartial<Maybe> or_(const Maybe &b) {
        return {b};
    }

    /** Returns a if it has a value, otherwise returns b. */
    template<detail::Nullable Maybe>
    const Maybe &or_(const Maybe &a, const Maybe &b) {
        return a ? a : b;
    }

    template<detail::Nullable Maybe>
    const Maybe &operator|(const Maybe &a, detail::OrPartial<Maybe> &&partial) {
        return a ? a : partial.b;
    }
}

// endregion

// region flatten.hpp


namespace maybe_ext::detail {
    struct FlattenPartial {};
}

namespace maybe_ext {
    inline detail::FlattenPartial flatten() {
        return {};
    }

    template<detail::Nullable Maybe,
             class Inner = detail::contained_type_t<Maybe>>
             requires detail::Nullable<Inner>
    Inner flatten(Maybe &&maybe) {
        return maybe ? *std::forward<Maybe>(maybe)
                     : detail::make_none<Inner>();
    }

    template<detail::Nullable Maybe,
             class Inner = detail::contained_type_t<Maybe>>
             requires detail::Nullable<Inner>
    Inner operator|(Maybe &&maybe, detail::FlattenPartial &&) {
        return maybe ? *std::forward<Maybe>(maybe)
                     : detail::make_none<Inner>();
    }
}

// endregion

// region filter.hpp


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

// endregion

// region deref.hpp


namespace maybe_ext::detail {
    struct DerefPartial {};
}

namespace maybe_ext {
    inline detail::DerefPartial deref() {
        return {};
    }

    template<detail::Nullable Maybe>
    decltype(auto) deref(Maybe &&maybe) {
        return *std::forward<Maybe>(maybe);
    }

    template<detail::Nullable Maybe>
    decltype(auto) operator|(Maybe &&maybe, detail::DerefPartial &&) {
        return *std::forward<Maybe>(maybe);
    }
}

// endregion

// region contains.hpp


namespace maybe_ext::detail {
    template<class U>
    struct ContainsPartial {
        const U &value;
    };
}

namespace maybe_ext {
    template<class U>
    detail::ContainsPartial<U> contains(const U &value) {
        return {value};
    }

    /** Returns true if maybe is not empty and contains the given value. */
    template<detail::Nullable Maybe, class U>
    bool contains(const Maybe &maybe, const U &value) requires requires {
        { *maybe == value } -> std::same_as<bool>; } {
        return maybe && (*maybe == value);
    }

    template<detail::Nullable Maybe, class U>
    bool operator|(const Maybe &maybe, detail::ContainsPartial<U> &&partial) requires requires {
        { *maybe == partial.value } -> std::same_as<bool>; } {
        return maybe && (*maybe == partial.value);
    }
}

// endregion

// region cloned.hpp

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

// endregion

// region as_unowned.hpp


namespace maybe_ext {
    template<detail::Nullable Maybe,
             class T = detail::contained_type_t<Maybe>>
    T *as_unowned(Maybe & maybe) {
        return maybe ? &(*maybe) : nullptr;
    }
}

// endregion

// region member_ptr_adaptor.hpp

#include <type_traits>

namespace maybe_ext::detail {
    template<MethodPtr Method>
    auto method_ptr_adaptor(Method method) {
        return [method]<class T>(T &&obj) -> decltype(auto) {
            return (std::forward<T>(obj).*method)();
        };
    }

    template<FieldPtr Field>
    auto field_ptr_map_adaptor(Field field) {
        return [field](auto &&obj) -> auto & {
            return obj.*field;
        };
    }

    template<FieldPtr Field>
    auto field_ptr_and_then_adaptor(Field field) {
        return [field](auto &&obj) {
            static_assert(Nullable<decltype(obj.*field)>,
                          "This field is not a nullable type.");
            return as_unowned(obj.*field);
        };
    }
}

// endregion

// region map_or_else.hpp


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

// endregion

// region map_or.hpp


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

// endregion

// region map.hpp


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
        return maybe ? &(f(*std::forward<Maybe>(maybe)))
                     : nullptr;
    }

    template<detail::Nullable Maybe, class F>
    auto operator|(Maybe &&maybe, detail::MapPartial<F> &&partial) requires requires {
        { partial.f(*std::forward<Maybe>(maybe)) } -> detail::LValueRef; } {
        return maybe ? &(partial.f(*std::forward<Maybe>(maybe)))
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

// endregion

// region operator_bind.hpp


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

// endregion

// region and_then.hpp


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

// endregion

// region and.hpp


namespace maybe_ext::detail {
    template<detail::Nullable Maybe>
    struct AndPartial {
        Maybe b;
    };
}

namespace maybe_ext {
    template<detail::Nullable Maybe>
    detail::AndPartial<Maybe> and_(Maybe b) {
        return {std::move(b)};
    }

    /** Returns nullopt or nullptr (according to the type of b) if a is empty, otherwise returns b. */
    template<detail::Nullable MaybeA, detail::Nullable MaybeB>
    MaybeB and_(const MaybeA &a, MaybeB b) {
        if (!a) { return detail::make_none<MaybeB>(); }
        else { return b; }
    }

    template<detail::Nullable MaybeA, detail::Nullable MaybeB>
    MaybeB operator|(const MaybeA &a, detail::AndPartial<MaybeB> &&partial) {
        if (!a) { return detail::make_none<MaybeB>(); }
        else { return std::move(partial.b); }
    }
}

// endregion

#endif
