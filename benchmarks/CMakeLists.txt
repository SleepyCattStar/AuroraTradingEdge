cmake_minimum_required(VERSION 3.16)

project(AuroraBenchmarks)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../include)

find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)

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
    ../src/logger/analyzer_logger.cpp
    ../src/logger/engine_logger.cpp
    ../src/utils/formatter.cpp
)

add_executable(latency_benchmark
    latency_benchmark.cpp
    ../src/market/binance_market_data_producer.cpp
    ../src/analytics/market_analyzer.cpp
    ../src/logger/engine_logger.cpp
    ../src/logger/analyzer_logger.cpp
    ../src/utils/formatter.cpp
)

# Project root available to source files
target_compile_definitions(queue_benchmark PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}/.."
)

target_compile_definitions(pipeline_benchmark PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}/.."
)

target_compile_definitions(latency_benchmark PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}/.."
)

# Libraries required by pipeline benchmark
target_link_libraries(pipeline_benchmark PRIVATE
    Boost::system
    OpenSSL::SSL
    OpenSSL::Crypto
    nlohmann_json::nlohmann_json
)

# Libraries required by Binance WebSocket benchmark
target_link_libraries(latency_benchmark PRIVATE
    Boost::system
    OpenSSL::SSL
    OpenSSL::Crypto
    nlohmann_json::nlohmann_json
)