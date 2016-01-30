//
//  seLogger.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 30.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seLogger_hpp
#define seLogger_hpp

#include "../Utils/seSingleton.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <map>

namespace spriteEngine {
    const char eol[] = "\n";

    class seLogger {
    public:
        virtual ~seLogger();

        static void Initialize();
        static void AddLog(const std::string &logName, seLogger *logger);
        static void SetLogger(seLogger *logger);
        static void Destroy();

        static seLogger& GetLogger(const std::string &loggerName);
        static seLogger& Debug();
        static seLogger& Error();
        static seLogger& Omni();

        template<typename T>
        friend seLogger& operator<<(seLogger &logger, T const &toLog) {
            if (m_useLogger) {
                std::ostringstream oss;
                oss << toLog;
                logger.Write(oss.str());
            }

            return logger;
        }

    protected:
        static bool m_useLogger;

    private:
        static seLogger *m_currentInstance;
        static std::map<const std::string, seLogger *> m_instanceStack;

        virtual void Write(const std::string &message) = 0;
    };

    class seFileLogger : public seLogger {
    public:
        seFileLogger(const std::string &filename);

    private:
        std::ofstream m_file;

        ~seFileLogger();

        virtual void Write(const std::string &message);
    };

    class seDebugLogger : public seLogger {
    private:
        virtual void Write(const std::string &message);
    };

    class seOmniLogger : public seLogger {
    private:
        virtual void Write(const std::string &message);
    };

#define DebugLogger seLogger::Debug()
#define ErrorLogger seLogger::Error()
#define Logger seLogger::Omni()
#define LogError Logger << "ERROR: "
#define LogDebug Logger
}

#endif /* seLogger_hpp */
