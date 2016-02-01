//
//  seHelpers.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seHelpers_h
#define seHelpers_h

#include <string>
#include <sstream>
#include <glm/glm.hpp>

namespace spriteEngine {
    inline std::string quoteStr(const std::string &str) {
        return "'" + str + "'";
    }

    inline bool ends_with(std::string const &value, std::string const &ending) {
        if (ending.size() > value.size())
            return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

    template<typename T>
    std::string to_string(const T &n) {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

#endif /* seHelpers_h */
