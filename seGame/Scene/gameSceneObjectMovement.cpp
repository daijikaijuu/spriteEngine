//
//  gameSceneObjectMovement.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 18.02.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "gameSceneObjectMovement.hpp"
#include <cmath>

gameSceneObjectMovement::gameSceneObjectMovement() :
    m_speed(0.0f, 0.0f),
    m_speedLimit(10.0f, 10.0f)
{
}

gameSceneObjectMovement::~gameSceneObjectMovement() {
}

void gameSceneObjectMovement::SetSpeed(const glm::vec2 &speed) {
    m_speed = speed;
    if (std::fabs(m_speed.x) > m_speedLimit.x) {
        float n = m_speed.x > 0 ? +1.0f : -1.0f;
        m_speed.x = m_speedLimit.x * n;
    }
    if (std::fabs(m_speed.y) > m_speedLimit.y) {
        float n = m_speed.y > 0 ? +1.0f : -1.0f;
        m_speed.y = m_speedLimit.y * n;
    }
}
