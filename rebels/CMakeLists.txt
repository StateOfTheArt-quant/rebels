cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(rebels)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Release)

add_library(${PROJECT_NAME} STATIC
        src/utilities/csv.cpp
        src/data/streamer.cpp
        src/data/records.cpp
        src/data/basic.cpp
        src/market/order.cpp
        src/market/book.cpp
        src/utilities/accumulator.cpp
        src/utilities/mock.cpp
        src/data/data_source.cpp
        src/object/order.cpp
        src/object/trade.cpp
        src/object/events.cpp
        src/core/executor.cpp
        src/core/strategy.cpp
        src/core/context.cpp
        src/mod/sys_account/position.cpp
        src/mod/sys_account/account.cpp
        src/mod/sys_account/portfolio.cpp
        src/mod/sys_analyzer/analyzer.cpp
        src/mod/sys_simulation/matcher.cpp
        src/mod/sys_simulation/simulation_broker.cpp
        src/mod/sys_transaction_cost/decider.cpp
        src/environment/environment.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} fmt::fmt spdlog::spdlog Dexode::EventBus)
