//
//  seExceptions.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 30.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seExceptions_hpp
#define seExceptions_hpp

#include "seLogger.hpp"
#include <stdlib.h>
#include <exception>
#include <string>
#include <sstream>

namespace spriteEngine {
    class seException : public std::exception {
    public:
        seException(const std::string &message = "");
        virtual ~seException() throw();

        virtual const char* what() const throw();

    protected:
        std::string m_message;
    };

#ifdef _DEBUG
#define seAssert(condition) {\
    if (!(condition)) { \
        std::ostringstream oss; \
        oss << "Assertion failed at " << __FILE__ << ":" << __LINE__ << eol << \
               "  inside funtion " << __FUNCTION__ << eol << \
               " *Condition: " << #condition; \
        Logger << oss.str() << eol; \
        abort(); \
    } \
}
#else
    inline void DoNothing(bool) {}
#define seAssert(condition) DoNothing(!(condition))
#endif
}

#endif /* seExceptions_hpp */
