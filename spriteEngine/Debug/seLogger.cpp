//
//  seLogger.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 30.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "seLogger.hpp"
#include "../Utils/seHelpers.hpp"
#include <stdio.h>
#include <assert.h>
#include <iostream>

namespace spriteEngine {
    const std::string seDEFAULT_ERROR_LOG = "seError.log";

    seLogger *seLogger::m_currentInstance = nullptr;
    bool seLogger::m_useLogger = false;
    std::map<const std::string, seLogger *> seLogger::m_instanceStack;

    seLogger::~seLogger() {
    }

    void seLogger::Initialize() {
        seLogger::AddLog("ErrorLog", new seFileLogger(seDEFAULT_ERROR_LOG));
        seLogger::AddLog("DebugLog", new seDebugLogger());
        seLogger::AddLog("OmniLogger", new seOmniLogger());

        m_useLogger = true;
    }

    void seLogger::AddLog(const std::string &logName, seLogger *logger) {
        assert(logger);

        m_instanceStack.insert(std::pair<const std::string, seLogger *>(logName, logger));
    }

    void seLogger::Destroy() {
        for (auto &it : m_instanceStack) {
            delete it.second;
            it.second = nullptr;
        }

        m_currentInstance = nullptr;
    }

    void seLogger::SetLogger(seLogger *logger) {
        assert(logger);

        m_currentInstance = logger;
    }

    seLogger& seLogger::Debug() {
        SetLogger(m_instanceStack["DebugLog"]);
        return *m_currentInstance;
    }

    seLogger& seLogger::Error() {
        SetLogger(m_instanceStack["ErrorLog"]);
        return *m_currentInstance;
    }

    seLogger& seLogger::Omni() {
        SetLogger(m_instanceStack["OmniLogger"]);
        return *m_currentInstance;
    }


    seFileLogger::seFileLogger(const std::string &filename) :
        m_file()
    {
        m_file.open(filename.c_str());
        if (!m_file.good())
            throw std::runtime_error("Log file could not be opened! Filename: " + quoteStr(filename));

        m_file << "*---------------------------------------------------------------------" << eol;
        m_file << "*                            Log begin                               *" << eol;
        m_file << "*---------------------------------------------------------------------" << eol;
    }

    seFileLogger::~seFileLogger() {
        m_file << "*---------------------------------------------------------------------" << eol;
        m_file << "*                             Log end                                *" << eol;
        m_file << "*---------------------------------------------------------------------" << eol;
        m_file.close();
    }

    void seFileLogger::Write(const std::string &message) {
        assert(m_file.is_open());

        m_file << message;
        if (message == eol)
            m_file.flush();
    }

    void seDebugLogger::Write(const std::string &message) {
        std::cout << message;
    }

    void seOmniLogger::Write(const std::string &message) {
        DebugLogger << message;
        ErrorLogger << message;
    }
}
