#pragma once

#ifdef _DEBUG
#include <fstream>
#endif // _DEBUG

#include <string>
#include "helpers.h"

enum LogType
{
    DEBUG,
    INFO,
    ERROR
};

#define debug(message, ...)      LogManager::GetInstance()->Log(LogManager::LL_DEBUG, message, ##__VA_ARGS__)
#define debugError(message, ...) LogManager::GetInstance()->Log(LogManager::LL_ERROR, message, ##__VA_ARGS__)
#define debugInfo(message, ...)  LogManager::GetInstance()->Log(LogManager::LL_INFO,  message, ##__VA_ARGS__)

class LogManager : public Singleton<LogManager>
{
    friend class Singleton<LogManager>;
public:
    static const std::string LL_DEBUG;
    static const std::string LL_INFO;
    static const std::string LL_ERROR;

    LogManager();
    virtual ~LogManager();

    template<typename T, typename ...Params>
    void Log(T message, Params && ...args);
    template<typename T, typename ...Params>
    void Log(LogType logType, T message, Params && ...args);

private:
#ifdef _DEBUG
    static const char * const LOG_FILENAME;
    std::ofstream m_OutputStream;
#endif // _DEBUG

    void Log();
};

class Logger
{
public:
    Logger(bool verbose = true);
    virtual ~Logger() {}

    virtual std::string class_type() = 0;

protected:
    template<typename T, typename ...Params>
    void Log(LogType logType, T message, Params && ...args);

private:
    bool m_verbose;
};

template<typename T, typename ...Params>
void LogManager::Log(T message, Params && ...args)
{
#ifdef _DEBUG
    m_OutputStream << message;
#endif // _DEBUG
    std::cout << message;

    Log(std::forward<Params>(args)...);
}

template<typename T, typename ...Params>
void LogManager::Log(LogType logType, T message, Params && ...args)
{
    std::string resultMessage;
    switch (logType)
    {
    case DEBUG:
        resultMessage += LL_DEBUG;
        break;
    case INFO:
        resultMessage += LL_INFO;
        break;
    case ERROR:
        resultMessage += LL_ERROR;
        break;
    default:
        break;
    }

    resultMessage += message;
    Log(resultMessage, args...);
}

template<typename T, typename ...Params>
void Logger::Log(LogType logType, T message, Params && ...args)
{
    if (m_verbose)
    {
        std::string resultMessage;
        resultMessage = class_type() + "::" + message;
        LogManager::GetInstance()->Log(logType, resultMessage, args...);
    }
}
