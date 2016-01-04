#pragma once

#include "../homework02.h"
#include "GenericActor.h"

class ActorSnowflake : public TexturedActor
{
public:
    ActorSnowflake(GLfloat x, GLfloat y, GLfloat size, GLuint sceneWidth, GLuint sceneHeight, GLfloat z);
    virtual ~ActorSnowflake();

    virtual void Draw();
    virtual void Animate(GLint elapsedTime);

    virtual void Move(GLfloat xShift, GLfloat yShift);

private:
    GLuint m_modelview;
    GLfloat m_speed, m_deltaX;
    GLfloat m_angle;
    GLuint m_sceneWidth, m_sceneHeight;
};
