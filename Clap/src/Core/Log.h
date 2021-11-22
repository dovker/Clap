#pragma once
#include "pch.h"

namespace Clap
{
    class Logger
    {
    public:
        enum class Strength
        {
            None = 0,
            Trace,
            Info,
            Warn,
            Error,
            Critical
        };
        static void Log(const std::string& text);

        //-- Helper functions
        static std::string GetCurrentTime(); //Returns in format [hh::mm::ss]

        //Converts log type to a string
        //Example: GetLogString(Strength::Trace); -> return: "[T]"
        static std::string GetLogString(Strength strength); 

        //-- Variadic Log functions
        //Syntax example: LogArgs("Logged: ", x, ", ", y); -> output: (Logged: 100.0, 120.0) 
        static void LogArgs(std::stringstream & iss);
        
        template <typename T, typename ...Rest>
        static void LogArgs(std::stringstream & iss, T && t, Rest &&... rest)
        {
            iss << std::forward<T>(t);
            LogArgs(iss, std::forward<Rest>(rest)...);
        }

        template <typename ...Args>
        static void LogArgs(Strength strength, Args &&... args)
        {
            std::stringstream iss;

            iss<<GetLogString(strength)<<GetCurrentTime()<<"| ";

            LogArgs(iss, std::forward<Args>(args)...);
        }
    };
}

#define CLAP_LOG(...)      Clap::Logger::LogArgs(Clap::Logger::Strength::None,     __VA_ARGS__)
#define CLAP_TRACE(...)    Clap::Logger::LogArgs(Clap::Logger::Strength::Trace,    __VA_ARGS__)
#define CLAP_INFO(...)     Clap::Logger::LogArgs(Clap::Logger::Strength::Info,     __VA_ARGS__)
#define CLAP_WARN(...)     Clap::Logger::LogArgs(Clap::Logger::Strength::Warn,     __VA_ARGS__)
#define CLAP_ERROR(...)    Clap::Logger::LogArgs(Clap::Logger::Strength::Error,    __VA_ARGS__)
#define CLAP_CRITICAL(...) Clap::Logger::LogArgs(Clap::Logger::Strength::Critical, __VA_ARGS__)