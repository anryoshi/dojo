cmake_minimum_required(VERSION 3.0)

project(
    clap
    VERSION 1.0
    DESCRIPTION "Command Line Argument Parser"
    LANGUAGES C
    )

add_library(
    clap
    STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/clap.c
    )

target_include_directories(
    clap
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

set_target_properties(
    clap
    PROPERTIES
        C_STANDARD 90
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
    )

target_compile_options(
    clap
    PUBLIC
        $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
           -Wall
           -Wextra
           -Werror
           -pedantic-errors
           -Wconversion>
        $<$<C_COMPILER_ID:MSVC>:
            /Wall>
    )

add_executable(
    clap_test
        ${CMAKE_CURRENT_SOURCE_DIR}/clap_test.c
    )

target_link_libraries(
    clap_test
    PRIVATE
        clap
        nanotest
    )

set_target_properties(
    clap_test
    PROPERTIES
        C_STANDARD 90
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
    )

add_test(
    NAME clap_test
    COMMAND clap_test
    )
