function(download_fmt)
    set(FMT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fmt)

    if(EXISTS ${FMT_DIR})
        message(STATUS "${FMT_DIR} is already exists, skip download fmt, auto building ...")
        add_subdirectory(${FMT_DIR})
    else()
        set(FMT_URL "https://github.com/fmtlib/fmt.git")
        set(FMT_VERSION "8.1.1")
        set(FMT_TAG "${FMT_VERSION}")

        message(STATUS "Downloading FMT tag ${FMT_TAG},fetch url is: ${FMT_URL}")

        include(FetchContent)

        FetchContent_Declare(
                fmt_suites
                GIT_REPOSITORY ${FMT_URL}
                GIT_TAG ${FMT_TAG}
                SOURCE_DIR ${FMT_DIR}
        )

        FetchContent_MakeAvailable(fmt_suites)

        # debug message
        message(STATUS "download fmt successfully")
    endif()

endfunction(download_fmt)
