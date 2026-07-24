#include "logger/engine_logger.hpp"
#include<iostream>
#include<filesystem>
#include<chrono>
#include "utils/formatter.hpp"  // to format the time

// EngineLogger::EngineLogger(){

// }

std::ofstream EngineLogger::log_file;
bool EngineLogger::initialized = false;

void EngineLogger::initialize(){
    if (initialized)
    {
        return;
    }
    std::filesystem::path log_directory =
        std::filesystem::path(PROJECT_ROOT) / "logs";

        // path now is trade-engine/logs , here as trade-engine is the project root

    // creates directory logs/ if it doesnt exist
    std::filesystem::create_directories(log_directory);

    log_file.open(
        log_directory / "engine.log",
        std::ios::app
    );

    initialized = true;
}

std::string EngineLogger::get_timestamp(){
    auto time = std::chrono::system_clock::now();
    return format_timestamp(time);
}

void EngineLogger::log(const std::string& level, const std::string& message){
    if(!initialized){
        initialize();
    }

    // std::string log_message = "[" + get_timestamp() + "]"
    //                             +"[" + level + "] "
    //                             + message;
    std::string log_message =
    "[" + get_timestamp() + "] "
    "[" + level + "] "
    + message;  

    std::cout << log_message <<"\n";
    if(log_file.is_open()){
        log_file << log_message << "\n";
        log_file.flush();
    }
}

void EngineLogger::info(const std::string& message){
    log("INFO", message);
}
void EngineLogger::warning(const std::string& message){
    log("WARNING",message);
}
void EngineLogger::error(const std::string& message){
    log("ERROR", message);
}

const void EngineLogger::shutdown(){

    if(log_file.is_open()){
        log_file.close();
    }

    initialized = false;
}