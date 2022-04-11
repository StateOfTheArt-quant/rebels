#

FILE(GLOB EXTEND_MODULE "${CMAKE_CURRENT_LIST_DIR}/mod*.cmake")

#loop glob list and load module
foreach(ext ${EXTEND_MODULE})
    message(STATUS "load ${ext} module successfully")
    include(${ext})
endforeach()