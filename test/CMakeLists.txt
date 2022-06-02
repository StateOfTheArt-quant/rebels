# unittest for rebels
project(rebels-unittest)

add_executable(test_reward test_reward.cpp)
target_link_libraries(test_reward PRIVATE rebels)

add_executable(test_transaction_cost test_transaction_cost.cpp)
target_link_libraries(test_transaction_cost PRIVATE rebels)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)