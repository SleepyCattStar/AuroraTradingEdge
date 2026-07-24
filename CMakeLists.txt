cmake_minimum_required(VERSION 3.16)
project(AuroraEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories(include)

add_executable(aurora
    src/main.cpp
    src/market/market_data_producer.cpp
    src/market/market_data_consumer.cpp
    src/utils/formatter.cpp
    src/core/dispatcher.cpp
    src/analytics/market_analyzer.cpp
    src/logger/market_logger.cpp
    src/market/binance_market_data_producer.cpp
)


target_compile_definitions(aurora PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(aurora PRIVATE
    Boost::system
    OpenSSL::SSL
    OpenSSL::Crypto
    nlohmann_json::nlohmann_json
)