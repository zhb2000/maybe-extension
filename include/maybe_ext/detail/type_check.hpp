#pragma once

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
