cmake_minimum_required(VERSION 3.0)

project(
    c-dojo
    DESCRIPTION "dojo part for good old C Programming Language"
    LANGUAGES C
    )

enable_testing()

# Because purpose is write portable code
# safe versions of standard C functions should be avoided
if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(nanotest)
add_subdirectory(clap)
