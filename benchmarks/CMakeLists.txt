cmake_minimum_required(VERSION 3.16)

project(AuroraBenchmarks)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../include)

add_executable(queue_benchmark
    queue_benchmark.cpp
    ../src/market/market_data_producer.cpp
    ../src/market/market_data_consumer.cpp
    ../src/utils/formatter.cpp
)

add_executable(pipeline_benchmark
    pipeline_benchmark.cpp
    ../src/market/market_data_producer.cpp
    ../src/core/dispatcher.cpp
    ../src/analytics/market_analyzer.cpp
    ../src/utils/formatter.cpp
)