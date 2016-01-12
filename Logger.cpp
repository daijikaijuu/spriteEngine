#include <stdexcept>
#include <iostream>
#include "Logger.h"

const std::string LogManager::LL_DEBUG = "DEBUG: ";
const std::string LogManager::LL_INFO = "INFO: ";
const std::string LogManager::LL_ERROR = "ERROR: ";

#ifdef _DEBUG
const char * const LogManager::LOG_FILENAME = "homework02.log";
#endif // _DEBUG

LogManager::LogManager() :
    Singleton<LogManager>()
{
#ifdef _DEBUG
    m_OutputStream.open(LOG_FILENAME);
    if (!m_OutputStream.good())
    {
        throw std::runtime_error("Unable to initialize LogManager!");
    }
#endif // _DEBUG
}

LogManager::~LogManager()
{
#ifdef _DEBUG
    m_OutputStream.close();
#endif // _DEBUG
}

void LogManager::Log()
{
#ifdef _DEBUG
    m_OutputStream << std::endl;
#endif // _DEBUG
    std::cout << std::endl;
}

Logger::Logger(bool verbose) :
    m_verbose(verbose)
{
}
