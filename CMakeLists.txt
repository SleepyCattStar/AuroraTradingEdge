cmake_minimum_required(VERSION 3.16)
project(AuroraEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(aurora
    src/main.cpp
    src/market/market_data_producer.cpp
    src/market/market_data_consumer.cpp
    src/utils/formatter.cpp
    src/core/dispatcher.cpp
    src/analytics/market_analyzer.cpp
    src/logger/market_logger.cpp
)


target_compile_definitions(aurora PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)