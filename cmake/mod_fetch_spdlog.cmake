
function(download_spdlog)
    set(SPDLOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spdlog)

    #if spdlog exists, then skip download step
    if(EXISTS ${SPDLOG_DIR})
        message(STATUS "${SPDLOG_DIR} is already exists, skip download spdlog, auto building ...")
        add_subdirectory(${SPDLOG_DIR})
    else()
        set(SPDLOG_URL "https://github.com/gabime/spdlog.git")
        set(SPDLOG_VERSION "1.9.2")
        set(SPDLOG_TAG "v${SPDLOG_VERSION}")

        message(STATUS "Downloading spdlog tag ${SPDLOG_TAG},fetch url is: ${SPDLOG_URL}")

        include(FetchContent)

        FetchContent_Declare(
                spdlog_suites
                GIT_REPOSITORY ${SPDLOG_URL}
                GIT_TAG ${SPDLOG_TAG}
                SOURCE_DIR ${SPDLOG_DIR}
        )

        FetchContent_MakeAvailable(spdlog_suites)

        # debug message
        message(STATUS "download spdlog successfully")
    endif()
endfunction(download_spdlog)