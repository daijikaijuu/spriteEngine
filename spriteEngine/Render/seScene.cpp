//
//  seScene.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "seScene.hpp"
#include "../Resources/seResourceManager.hpp"

namespace spriteEngine {
    seScene::seScene(GLfloat width, GLfloat height) :
        seCollection<seGenericSceneObject>(),
        m_width(width), m_height(height)
    {
        InitializeResources();
    }

    seScene::~seScene() {
        seResourceManager::Destroy();
    }

    void seScene::InitializeResources() {
    }

    void seScene::Render() {
        for (auto &it : Items()) {
            it.second->Render();
        }
    }
}