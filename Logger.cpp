#include <stdexcept>
#include "Logger.h"

const std::string Logger::LL_DEBUG = "DEBUG: ";
const std::string Logger::LL_INFO = "INFO: ";
const std::string Logger::LL_ERROR = "ERROR: ";

#ifdef _DEBUG
const char * const Logger::LOG_FILENAME = "homework02.log";
#endif // _DEBUG

Logger::Logger() :
    Singleton<Logger>()
{
#ifdef _DEBUG
    m_OutputStream.open(LOG_FILENAME);
    if (!m_OutputStream.good())
    {
        throw std::runtime_error("Unable to initialize Logger!");
    }
#endif // _DEBUG
}

Logger::~Logger()
{
#ifdef _DEBUG
    m_OutputStream.close();
#endif // _DEBUG
}

void Logger::log()
{
#ifdef _DEBUG
    m_OutputStream << std::endl;
#endif // _DEBUG
    std::cout << std::endl;
}
