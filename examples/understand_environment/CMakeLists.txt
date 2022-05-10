project(understand_environment)

add_executable(understand_environment understand_environment.cpp)
target_link_libraries(understand_environment rebels)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
