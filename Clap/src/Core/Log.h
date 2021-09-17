#pragma once

namespace Clap
{
    class Log
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
        static const std::string GetCurrentTime(); //Returns in format [hh::mm::ss]

        //Converts log type to a string
        //Example: GetLogString(Strength::Trace); -> return: "[T]"
        static const std::string GetLogString(Strength strength); 

        //-- Variadic Log functions
        //Syntax example: LogArgs("Logged: ", x, ", ", y); -> output: (Logged: 100.0, 120.0) 
        static void LogArgs(std::istringstream & iss);
        
        template <typename T, typename ...Rest>
        static void LogArgs(std::istringstream & iss, T && t, Rest &&... rest)
        {
            iss << std::forward<T>(t);
            LogArgs(iss, std::forward<Rest>(rest)...);
        }

        template <typename ...Args>
        static void LogArgs(Strength strength, Args &&... args)
        {
            std::istringstream iss;

            iss<<GetLogString(strength)<<GetCurrentTime()<<": ";

            LogArgs(iss, std::forward<Args>(args)...);
        }
    };
}

#define CLAP_LOG     (...) LogArgs(Strength::None,     __VA_ARGS__)
#define CLAP_TRACE   (...) LogArgs(Strength::Trace,    __VA_ARGS__)
#define CLAP_INFO    (...) LogArgs(Strength::Info,     __VA_ARGS__)
#define CLAP_WARN    (...) LogArgs(Strength::Warn,     __VA_ARGS__)
#define CLAP_ERROR   (...) LogArgs(Strength::Error,    __VA_ARGS__)
#define CLAP_CRITICAL(...) LogArgs(Strength::Critical, __VA_ARGS__)