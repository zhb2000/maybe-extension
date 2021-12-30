cmake_minimum_required(VERSION 3.20)
project(maybe)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options(/utf-8 /W3 /permissive-)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

include_directories(${CMAKE_SOURCE_DIR}/include)

SET(MAYBE_INCLUDE
        include/maybe_ext/maybe_ext.hpp
        include/maybe_ext/and.hpp
        include/maybe_ext/and_then.hpp
        include/maybe_ext/as_unowned.hpp
        include/maybe_ext/cloned.hpp
        include/maybe_ext/contains.hpp
        include/maybe_ext/deref.hpp
        include/maybe_ext/filter.hpp
        include/maybe_ext/flatten.hpp
        include/maybe_ext/map.hpp
        include/maybe_ext/map_or.hpp
        include/maybe_ext/map_or_else.hpp
        include/maybe_ext/or.hpp
        include/maybe_ext/or_else.hpp
        include/maybe_ext/value_or.hpp
        include/maybe_ext/value_or_else.hpp
        include/maybe_ext/xor.hpp
        include/maybe_ext/operator_bind.hpp
        include/maybe_ext/utils.hpp
        include/maybe_ext/detail/type_check.hpp
        include/maybe_ext/detail/make_none.hpp
        include/maybe_ext/detail/member_ptr_adaptor.hpp)

add_executable(maybe_example example.cpp ${MAYBE_INCLUDE})

add_subdirectory(tests)

add_subdirectory(examples)
