project(understand_event_driven)


add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} rebels)

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)
