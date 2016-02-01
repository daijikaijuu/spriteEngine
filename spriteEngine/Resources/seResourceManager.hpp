//
//  seResourceManager.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seResourceManager_hpp
#define seResourceManager_hpp

#include "../Utils/seSingleton.hpp"
#include "../Utils/seCollection.hpp"
#include "../Utils/seEnums.hpp"
#include <stdio.h>
#include <vector>
#include <map>
#include <string>

namespace spriteEngine {
    class seResource;
    class seShader;
    class seProgram;
    class seTexture;

    class seResourceManager : public seSingleton<seResourceManager>, public seCollection<seResource>
    {
        friend class seSingleton<seResourceManager>;
    public:
        seResourceManager();
        virtual ~seResourceManager();

        seTexture* GetTexture(const std::string &textureName);
        seShader* GetShader(const std::string &shaderName);
        seResource* GetResource(const std::string &resourceName);

        seTexture* AddTexture(const std::string &textureName);
        seShader* AddShader(const std::string &shaderName);
    };

#define seRManager seResourceManager::GetInstance()
}

#endif /* seResourceManager_hpp */
