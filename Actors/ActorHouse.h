#pragma once

#include "../homework02.h"
#include "GenericActor.h"

class Texture;

class ActorHouse : public GenericActor
{
public:
    ActorHouse(GLfloat x, GLfloat y, GLfloat z, GLfloat size);
    virtual ~ActorHouse();

    virtual void Draw();

    virtual void Move(GLfloat xShift, GLfloat yShift);
private:
    GLfloat m_size;
    GLuint m_modelview;
    Texture *m_texBrick,
            *m_texWindow;
};
