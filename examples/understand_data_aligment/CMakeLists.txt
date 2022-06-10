project(understand_data_aligment)

add_executable(understand_data_aligment understand_data_aligment.cpp)
target_link_libraries(understand_data_aligment rebels)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
