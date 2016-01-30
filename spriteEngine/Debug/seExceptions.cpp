//
//  seExceptions.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 30.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "seExceptions.hpp"
#include "seLogger.hpp"
#include <sstream>

namespace spriteEngine {
    seException::seException(const std::string &message) : m_message(message) {
    }

    seException::~seException() throw() {
    }

    const char* seException::what() const throw() {
        return m_message.c_str();
    }
}
