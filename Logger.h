#pragma once

#ifdef _DEBUG
#include <fstream>
#endif // _DEBUG

#include <windows.h>
#include <iostream>
#include <string>
#include "helpers.h"

using std::string;
using std::endl;

enum LogType
{
    LOGTYPE_DEBUG,
    LOGTYPE_INFO,
    LOGTYPE_ERROR,
    LOGTYPE_DUMP,
};

#define debug(message, ...)      LogManager::GetInstance()->Log(LogManager::LL_DEBUG, message, ##__VA_ARGS__)
#define debugError(message, ...) LogManager::GetInstance()->Log(LogManager::LL_ERROR, message, ##__VA_ARGS__)
#define debugInfo(message, ...)  LogManager::GetInstance()->Log(LogManager::LL_INFO,  message, ##__VA_ARGS__)

#define DUMP_VAR(variable) "   " << #variable << ": " << variable
#ifdef _DEBUG
#define HW_ASSERT(condition) { \
    if (!(condition)) { \
        Debug("Assertion failed at ", __FILE__, ":", __LINE__, \
              "\n inside function ", __FUNCTION__, \
              "\n Condition: ", #condition); \
        Log(LogType::LOGTYPE_DUMP, DEBUG_DUMP()); \
        return false; \
    } \
}
#else
#define HW_ASSERT(condition) (condition)
#endif

class LogManager : public Singleton<LogManager>
{
    friend class Singleton<LogManager>;
public:
    static const string LL_DEBUG;
    static const string LL_INFO;
    static const string LL_ERROR;
    static const string LL_DUMP;

    LogManager();
    virtual ~LogManager();

    template<typename T, typename ...Params>
    void Log(T message, Params && ...args);
    template<typename T, typename ...Params>
    void Log(LogType logType, T message, Params && ...args);

    void Dump(const string& dump);

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

    virtual string class_type() const { return "Logger"; }
    virtual bool DEBUG_OK() const;
    virtual string DEBUG_DUMP() const;

protected:
    template<typename T, typename ...Params>
    void Log(LogType logType, T message, Params && ...args) const;

    template<typename T, typename ...Params>
    inline void Debug(T message, Params && ...args) const { Log(LogType::LOGTYPE_DEBUG, message, args...); }
    template<typename T, typename ...Params>
    inline void Error(T message, Params && ...args) const { Log(LogType::LOGTYPE_ERROR, message, args...); }
    inline void CriticalError(const string& error = "Something went wrong!!! Aborting.") const { Log(LogType::LOGTYPE_ERROR, error); abort(); }
    template<typename T, typename ...Params>
    inline void Info(T message, Params && ...args) const { Log(LogType::LOGTYPE_INFO, message, args...); }

private:
    bool m_verbose;
};

template<typename T, typename ...Params>
void LogManager::Log(T message, Params && ...args)
{
#ifdef _DEBUG
    m_OutputStream << message;
    std::cout << message;
#endif // _DEBUG

    Log(std::forward<Params>(args)...);
}

template<typename T, typename ...Params>
void LogManager::Log(LogType logType, T message, Params && ...args)
{
    string resultMessage;
    switch (logType)
    {
    case LOGTYPE_DEBUG:
        resultMessage += LL_DEBUG;
        break;
    case LOGTYPE_INFO:
        resultMessage += LL_INFO;
        break;
    case LOGTYPE_ERROR:
        resultMessage += LL_ERROR;
        break;
    case LOGTYPE_DUMP:
        resultMessage += LL_DUMP;
        break;
    default:
        break;
    }

    resultMessage += message;
    Log(resultMessage, args...);
}

template<typename T, typename ...Params>
void Logger::Log(LogType logType, T message, Params && ...args) const
{
    if (m_verbose)
    {
        string resultMessage;
        resultMessage = class_type() + "::" + message;
        LogManager::GetInstance()->Log(logType, resultMessage, args...);
    }
}
