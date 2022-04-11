project(understand_objects)

#add_executable(understand_account understand_account.cpp)
#target_link_libraries(understand_account rebels)



add_executable(understand_broker understand_broker.cpp)
target_link_libraries(understand_broker rebels)

add_executable(understand_executor understand_executor.cpp)
target_link_libraries(understand_executor rebels)
