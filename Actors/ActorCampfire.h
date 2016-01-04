#pragma once
#include "GenericActor.h"

class Texture;

class ActorCampfire : public GenericActor
{
public:
    ActorCampfire(GLfloat x, GLfloat y, GLfloat z, GLfloat size);
    virtual ~ActorCampfire();

    virtual void Draw();
    virtual void Animate(GLint elapsedTime);

    virtual void Move(GLfloat xShift, GLfloat yShift);

private:
    Texture *m_texCapmfire;
    GLuint m_modelview;
    GLuint m_item, m_animationItem;
};

