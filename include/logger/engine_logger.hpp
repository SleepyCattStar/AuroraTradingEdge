#pragma once
#include<string>
#include<fstream>

class EngineLogger{
    private:

        static std::ofstream log_file;
        static  bool initialized;
        static void initialize();
        static std::string get_timestamp();
        static void log(const std::string& level,
            const std::string& message);

    public:
        static void info(const std::string & message);
        static void warning(const std::string& message);
        static void error(const std::string& message);

        const void shutdown();
};