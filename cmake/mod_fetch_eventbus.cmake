
function(download_eventbus)
    set(EVENTBUS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/eventbus)

    #if eventbus exists, then skip download step
    if(EXISTS ${EVENTBUS_DIR})
        message(STATUS "${EVENTBUS_DIR} is already exists, skip download eventbus, auto building ...")
        add_subdirectory(${EVENTBUS_DIR}/lib)
    else()
        set(EVENTBUS_URL "https://github.com/gelldur/EventBus.git")
        set(EVENTBUS_VERSION "3.0.1")
        set(EVENTBUS_TAG "v${EVENTBUS_VERSION}")

        message(STATUS "Downloading EVENTBUS tag ${EVENTBUS_TAG},fetch url is: ${EVENTBUS_URL}")

        include(FetchContent)

        FetchContent_Declare(
                eventbus_suites
                GIT_REPOSITORY ${EVENTBUS_URL}
                GIT_TAG ${EVENTBUS_TAG}
                SOURCE_DIR ${EVENTBUS_DIR}
        )

        FetchContent_MakeAvailable(eventbus_suites)

        # debug message
        message(STATUS "download eventbus successfully")
    endif()
endfunction(download_eventbus)