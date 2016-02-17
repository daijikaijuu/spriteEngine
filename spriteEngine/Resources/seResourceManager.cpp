//
//  seResourceManager.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "Resources.hpp"
#include "../Utils/seHelpers.hpp"
#include "../Debug/Debug.hpp"
#include <iostream>

namespace spriteEngine {
    seResourceManager::seResourceManager() : seSingleton<seResourceManager>(), seCollection<seResource>()
    {
    }

    seResourceManager::~seResourceManager() {
    }

    seShader* seResourceManager::GetShader(const std::string &shaderName) {
        seResource *res = GetResource(shaderName);
        seAssert(res);
        seAssert(res->Type() == seResourceType::seRESOURCE_SHADER);

        return dynamic_cast<seShader *>(res);
    }

    seTexture* seResourceManager::GetTexture(const std::string &textureName) {
        seResource *res = GetResource(textureName);
        seAssert(res);
        seAssert(res->Type() == seResourceType::seRESOURCE_TEXTURE);

        return dynamic_cast<seTexture *>(res);
    }

    seResource* seResourceManager::GetResource(const std::string &resourceName) {
        seAssert(!resourceName.empty());

        return GetItem(resourceName);
    }

    seTexture* seResourceManager::AddTexture(const std::string &textureName) {
        seResource *res = GetResource(textureName);
        if (res && res->Type() == seResourceType::seRESOURCE_TEXTURE)
            LogDebug << "WARNING! Texture " << quoteStr(textureName) << " already exists" << eol;
        else {
            res = new seTexture(textureName);
            AddItem(textureName, res);
        }

        return dynamic_cast<seTexture *>(res);
    }

    seShader* seResourceManager::AddShader(const std::string &shaderName) {
        seResource *res = GetResource(shaderName);
        if (res && res->Type() == seResourceType::seRESOURCE_SHADER)
            LogDebug << "WARNING! Shader " << quoteStr(shaderName) << " already exists" << eol;
        else {
            res = new seShader(shaderName);
            AddItem(shaderName, res);
        }

        return dynamic_cast<seShader *>(res);
    }
}
