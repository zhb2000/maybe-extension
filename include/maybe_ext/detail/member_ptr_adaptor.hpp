#pragma once

#include <type_traits>
#include "type_check.hpp"
#include "maybe_ext/as_unowned.hpp"

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
