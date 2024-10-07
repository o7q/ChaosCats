#pragma once

#include <iostream>
#include <string>

namespace Logger
{
    enum class LogType
    {
        Info,
        Warning,
        Error,
        Fatal
    };

    inline void log(LogType type, const std::string &title, const std::string &description)
    {
        std::string error_string = "";
        switch (type)
        {
        case LogType::Info:
            error_string = "INFO";
            break;
        case LogType::Warning:
            error_string = "WARNING";
            break;
        case LogType::Error:
            error_string = "ERROR";
            break;
        case LogType::Fatal:
            error_string = "FATAL";
            break;
        }

        std::string title_string = title == "" ? "" : "(" + title + ") ";

        std::cout << "[" << error_string << "] " << title_string << description << std::endl;
    }
}