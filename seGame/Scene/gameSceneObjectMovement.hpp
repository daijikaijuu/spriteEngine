//
//  gameSceneObjectMovement.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 18.02.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef gameSceneObjectMovement_hpp
#define gameSceneObjectMovement_hpp

#include <glm/glm.hpp>

class gameSceneObjectMovement {
public:
    gameSceneObjectMovement();
    virtual ~gameSceneObjectMovement();

    void SetSpeed(const glm::vec2 &speed);
    void Stop() { m_speed = glm::vec2(0.0f, 0.0f); }
    void IncHorizontalSpeed(float x) { m_speed.x += x; CheckSpeedLimits(); }
    void IncVerticalSpeed(float y) { m_speed.y += y; CheckSpeedLimits(); }

    glm::vec2 Speed() const { return m_speed; }

private:
    glm::vec2 m_speed;
    glm::vec2 m_speedLimit;

    void CheckSpeedLimits();
};



#endif /* gameSceneObjectMovement_hpp */
