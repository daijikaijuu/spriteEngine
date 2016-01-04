#pragma once
#include "GenericActor.h"

class ActorCampfire : public TexturedActor
{
public:
    ActorCampfire(GLfloat x, GLfloat y, GLfloat z, GLfloat size);
    virtual ~ActorCampfire();

    virtual void Draw();
    virtual void Animate(GLint elapsedTime);

    virtual void Move(GLfloat xShift, GLfloat yShift);

private:
    GLuint m_modelview;
    GLuint m_item, m_animationItem;
};

