cmake_minimum_required(VERSION 3.0)

project(
    nanotest
    VERSION 1.0
    DESCRIPTION "test framework so small that named nano"
    LANGUAGES C
    )

add_library(
    nanotest
    INTERFACE
    )

target_include_directories(
    nanotest
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

add_executable(
    nanotest_example
        ${CMAKE_CURRENT_SOURCE_DIR}/nanotest_example.c
    )

target_link_libraries(
    nanotest_example
    PRIVATE
        nanotest
    )

set_target_properties(
    nanotest_example
    PROPERTIES
        C_STANDARD 90
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
    )

target_compile_options(
    nanotest_example
    PRIVATE
        $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
           -Wall
           -Wextra
           -Werror
           -pedantic-errors
           -Wconversion>
        $<$<C_COMPILER_ID:MSVC>:
            /Wall>
    )
