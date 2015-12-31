#pragma once

#include "../homework02.h"
#include "GenericActor.h"

class Texture;

class ActorSnowflake :
    public GenericActor
{
public:
    ActorSnowflake(GLfloat x, GLfloat y, GLfloat size, GLuint sceneWidth, GLuint sceneHeight, GLfloat z);
    virtual ~ActorSnowflake();

    virtual void Draw();
    virtual void Animate();

    virtual void Move(GLfloat xShift, GLfloat yShift);

private:
    Texture *m_texture;
    GLuint m_modelview;
    GLfloat m_size;
    GLfloat m_speed, m_deltaX;
    GLfloat m_angle;
    GLuint m_sceneWidth, m_sceneHeight;
};
