#include "Logger.hpp"
#include <sstream>

Logger::Logger(const std::string& _filename)
    : m_File(_filename, std::ios::app)
{
}

void Logger::log(std::string_view _message, std::source_location _loc)
{
    std::scoped_lock lock(m_Mutex);
    //m_File << currentTimeStamp() << " " << _message << std::endl;
    
    auto now = std::chrono::system_clock::now();

    auto tt = std::chrono::system_clock::to_time_t(now);

    //m_File << std::put_time(std::localtime(&tt), "%F %T") << " [" << std::this_thread::get_id() << "] " << _loc.function_name() << " " << _message << std::endl;
    m_File << std::put_time(std::localtime(&tt), "%F %T") << _loc.function_name() << " " << _message << std::endl;
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
