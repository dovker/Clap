#include "pch.h"

#include "Log.h"

namespace Clap
{
    void Logger::Log(std::string const& message)
    {
        std::cout<<message<<std::endl;
        //TODO: Do file output later
    }

    void Logger::LogArgs(std::stringstream& iss)
    {
        Log(iss.str());
    }

    std::string Logger::GetLogString(Strength strength)
    {
        switch (strength)
        {
        case Strength::Trace:
            return "[T]";
        case Strength::Info:
            return "[I]";
        case Strength::Warn:
            return "[W]";
        case Strength::Error:
            return "[E]";
        case Strength::Critical:
            return "[C]";
        default:
            return "";
        }
    }

    std::string Logger::GetCurrentTime()
    {
        std::time_t now = time(0);
        tm *ltm = localtime(&now);

        std::stringstream s;
        s << '[' << 5+ltm->tm_hour << ":" << 30+ltm->tm_min << ":" << ltm->tm_sec << ']' << std::endl;
    }
}