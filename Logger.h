#pragma once

#ifdef _DEBUG
#include <fstream>
#endif // _DEBUG

#include <string>
#include "homework02.h"
#include "helpers.h"

#define debug(message, ...) Logger::GetInstance()->log(Logger::LL_DEBUG, message, __VA_ARGS__)
#define debugError(message, ...) Logger::GetInstance()->log(Logger::LL_ERROR, message, __VA_ARGS__)
#define debugInfo(message, ...) Logger::GetInstance()->log(Logger::LL_INFO, message, __VA_ARGS__)

class Logger : public Singleton<Logger>
{
    friend class Singleton<Logger>;
public:
    static const std::string LL_DEBUG;
    static const std::string LL_INFO;
    static const std::string LL_ERROR;

    Logger();
    virtual ~Logger();

    template<typename T, typename ... Params>
    void log(T message, Params &&... args);

private:
#ifdef _DEBUG
    static const char * const LOG_FILENAME;
    std::ofstream m_OutputStream;
#endif // _DEBUG

    void log();
};

template<typename T, typename... Params>
void Logger::log(T message, Params &&... args)
{
#ifdef _DEBUG
    m_OutputStream << message;
#endif // _DEBUG
    std::cout << message;

    log(std::forward<Params>(args)...);
}
