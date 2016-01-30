//
//  seShader.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seShader_hpp
#define seShader_hpp

#include "seResource.hpp"
#include <stdio.h>
#include <string>

namespace spriteEngine {
    static const std::string seSHADER_VERSION = "#version 330\n";

    class seShader : public seResource {
    public:
        seShader(const std::string &filename);
        virtual ~seShader();

    protected:
        virtual void LoadFromFile();
    };
}

#endif /* seShader_hpp */
