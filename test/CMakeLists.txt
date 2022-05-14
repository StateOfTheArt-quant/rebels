# unittest for rebels
project(rebels-unittest)

add_executable(test_reward test_reward.cpp)
target_link_libraries(test_reward PRIVATE rebels)
# set_property(TARGET draft PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)