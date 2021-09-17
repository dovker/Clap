#include "pch.h"

#include "Log.h"

namespace Clap
{
    void Log::Log(std::string const& message)
    {
        std::cout<<message<<std::endl;
        //TODO: Do file output later
    }

    void Log::LogArgs(std::istringstream& iss)
    {
        Log(iss.str());
    }

    const std::string Log::GetLogString(Strength strength)
    {
        switch (strength)
        {
        case Strength::Trace:
            return "[T]"
        case Strength::Info:
            return "[I]"
        case Strength::Warn:
            return "[W]"
        case Strength::Error:
            return "[E]"
        case Strength::Critical:
            return "[C]"
        default:
            return "";
        }
    }

    const std::string Log::GetCurrentTime()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        stringstream s;
        s << '[' << 5+ltm->tm_hour << ":" << 30+ltm->tm_min << ":" << ltm->tm_sec << ']' << std::endl;
    }
}