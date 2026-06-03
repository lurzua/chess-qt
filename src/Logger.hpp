#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <mutex>
#include <string>
#include <source_location>

class Logger
{

public:

    explicit Logger(const std::string&);
    void log(std::string_view, std::source_location _loc = std::source_location::current());

private:

    std::string currentTimeStamp();

private:

    std::ofstream m_File;
    std::mutex m_Mutex;
};

#endif

