#include "Logger.hpp"
#include <sstream>

Logger::Logger(const std::string& _filename)
    : m_File(_filename, std::ios::out | std::ios::trunc)
{
}

void Logger::log(std::string_view _message, std::source_location _loc)
{
    std::scoped_lock lock(m_Mutex);
    
    const auto timestamp = currentTimeStamp();
    m_File << std::format("{:<20} {:<80} {}\n",
        timestamp,
        _loc.function_name(),
        _message);
}

std::string Logger::currentTimeStamp()
{
    auto now = std::chrono::system_clock::now();

    auto time = std::chrono::system_clock::to_time_t(now);

    std::tm tm = *std::localtime(&time);

    std::ostringstream oss;

    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}
