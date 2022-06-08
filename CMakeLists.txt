cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(understand_rebels)


# cxx compile standard
set(CMAKE_CXX_STANDARD 17)
set(CAME_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# project version setup
set(REBELS_MAJOR_VERSION "0")
set(REBELS_MINOR_VERSION "1")

# debug info
message(STATUS "==================================================")
message(STATUS "rebels release version: ${REBELS_MAJOR_VERSION}.${REBELS_MINOR_VERSION}")
message(STATUS "==================================================")


# --------------------------------------
#  core third_partylibraries
# --------------------------------------
# load third cmake module
if(EXISTS ${CMAKE_SOURCE_DIR}/cmake/api.cmake)
    include(${CMAKE_SOURCE_DIR}/cmake/api.cmake)
else()
    message(FATAL_ERROR "load cmake module failed,build process interrupted.")
endif()

# download third package
download_fmt()
download_spdlog()
download_eventbus()

# --------------------------------
# custom library
# ---------------------------------
add_subdirectory(rebels)
# add_subdirectory(examples)
add_subdirectory(test)