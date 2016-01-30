//
//  seResourceManager.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "seResourceManager.hpp"
#include "seResource.hpp"
#include "seShader.hpp"
#include "seProgram.hpp"
#include "seTexture.hpp"
#include "../Utils/seHelpers.hpp"
#include "../Debug/seDebug.hpp"
#include <iostream>

namespace spriteEngine {
    seResourceManager::seResourceManager() : seSingleton<seResourceManager>(), seCollection<seResource>()
    {
        InitializeDefaultResources();
    }

    seResourceManager::~seResourceManager() {
    }

    seShader* seResourceManager::GetShader(const std::string &shaderName) {
        seResource *res = GetResource(shaderName);
        seAssert(!res);
        seAssert(res->Type() == seResourceType::seRESOURCE_SHADER);

        return dynamic_cast<seShader *>(res);
    }

    seProgram* seResourceManager::GetProgram(const std::string &programName) {
        seResource *res = GetResource(programName);
        seAssert(res);
        seAssert(res->Type() == seResourceType::seRESOURCE_SHADER_PROGRAM);

        return dynamic_cast<seProgram *>(res);
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
        seAssert(!GetResource(textureName));

        seResource *res = new seTexture(textureName);
        AddItem(textureName, res);

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

    seProgram* seResourceManager::AddProgram(const std::string &programName) {
        seAssert(GetResource(programName) == nullptr);

        seProgram *res = new seProgram(programName);
        AddItem(programName, res);

        return res;
    }

    void seResourceManager::InitializeDefaultResources() {
        seProgram *basicShaderProgram = AddProgram("vs:basic,fs:basic");
        basicShaderProgram->SetShaders(AddShader("basic.vs"), AddShader("basic.fs"));
    }
}
