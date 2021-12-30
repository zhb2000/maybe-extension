project(maybe_test)

add_subdirectory(googletest)

include_directories(
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
)

set(MAYBE_TESTS
        prelude.hpp
        maybe_test.cpp
        and_then_test.cpp
        and_test.cpp
        or_test.cpp
        xor_test.cpp
        as_unowned_test.cpp
        cloned_test.cpp
        contains_test.cpp
        deref_test.cpp
        filter_test.cpp
        map_test.cpp
        map_or_test.cpp
        map_or_else_test.cpp
        or_else_test.cpp
        value_or_else_test.cpp
        value_or_test.cpp
        convertor_test.cpp
        method_ptr_overload_test.cpp
        field_ptr_test.cpp
        method_ptr_return_ref_test.cpp examples_test.cpp operator_bind_test.cpp flatten_test.cpp)

add_executable(maybe_test ${MAYBE_TESTS})

target_link_libraries(maybe_test gtest gtest_main)

if (MSVC)
    target_precompile_headers(maybe_test PRIVATE
            <iostream>
            <string>
            <optional>
            <memory>
            <gtest/gtest.h>)
endif ()
