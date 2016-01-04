#pragma once
#include "GenericActor.h"

class ActorCampfire : public TexturedActor
{
public:
    ActorCampfire(GLfloat x, GLfloat y, GLfloat z, GLfloat size);
    virtual ~ActorCampfire();

    virtual void Draw();
    virtual void Animate(GLint elapsedTime);

private:
    GLuint m_item, m_animationItem;
};

