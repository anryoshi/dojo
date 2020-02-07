#!/usr/bin/env cmake -P

# File downloader script
# All pairs of argument will be used as source and destination

math(EXPR NUMBER_OF_ARGUMENTS "${CMAKE_ARGC} - 3")
math(EXPR NUMBER_OF_ARGUMENTS_PARITY "${NUMBER_OF_ARGUMENTS} % 2")

if (${NUMBER_OF_ARGUMENTS_PARITY} EQUAL 1)
    message(
        FATAL_ERROR
        "Number of arguments (${NUMBER_OF_ARGUMENTS}) is not even"
        )
endif()

math(EXPR BORDER "${CMAKE_ARGC} - 1")

foreach(URL_INDEX RANGE 3 ${BORDER} 2)
    math(EXPR PATH_INDEX "${URL_INDEX} + 1")
    set(URL "${CMAKE_ARGV${URL_INDEX}}")
    set(PATH "${CMAKE_ARGV${PATH_INDEX}}")

    message("Downloading ${URL} to ${PATH}")

    file(
        DOWNLOAD
            "${URL}"
            "${PATH}"
        STATUS
            DOWNLOAD_STATUS
        )

    list(GET DOWNLOAD_STATUS 0 DOWNLOAD_CODE)
    list(GET DOWNLOAD_STATUS 1 DOWNLOAD_MESSAGE)
    if (NOT ${DOWNLOAD_CODE} EQUAL 0)
        message("Failed with code ${DOWNLOAD_CODE}: ${DOWNLOAD_MESSAGE}")
    endif()

endforeach()
