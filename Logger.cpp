#include <stdexcept>
#include <iostream>
#include "Logger.h"

const string LogManager::LL_DEBUG = "DEBUG: ";
const string LogManager::LL_INFO = "INFO: ";
const string LogManager::LL_ERROR = "ERROR: ";
const string LogManager::LL_DUMP = "DUMP: ";

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
    m_OutputStream << endl;
    std::cout << endl;
#endif // _DEBUG
}

Logger::Logger(bool verbose) :
    m_verbose(verbose)
{
}

bool Logger::DEBUG_OK() const
{
    return true;
}

string Logger::DEBUG_DUMP() const
{
    return string();
}
