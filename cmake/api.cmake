# Using cmake filesystem to glob all cmake modules
FILE(GLOB EXTENSIONS "${CMAKE_CURRENT_LIST_DIR}/Mod*.cmake")

# Automatically load or skip extension modules
if(DEFINED EXTENSIONS)
    message(STATUS "Find custom module in ${CMAKE_CURRENT_LIST_DIR}.")
    foreach(extension ${EXTENSIONS})
        include(${extension})
        message(STATUS "Load extension ${extension} success.")
    endforeach()
else()
    message(STATUS "Can't find any custom extensions in ${CMAKE_CURRENT_LIST_DIR}.")
endif()