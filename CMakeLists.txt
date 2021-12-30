cmake_minimum_required(VERSION 3.20)
project(maybe VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options(/utf-8 /W3 /permissive-)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

# force colored output
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
endif ()

include_directories(include/)

add_library(maybe-extension INTERFACE)
target_link_directories(maybe-extension INTERFACE include/)

add_subdirectory(examples)

add_subdirectory(tests)
