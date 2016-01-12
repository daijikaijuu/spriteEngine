#pragma once
#include "GenericActor.h"

class ActorCampfire : public TexturedActor
{
public:
    ActorCampfire(GLfloat x, GLfloat y, GLfloat z, GLfloat size);
    virtual ~ActorCampfire();

    virtual void Draw();
    virtual void Animate(GLint elapsedTime);

    virtual std::string class_type() { return "ActorCampfire"; }

protected:
    virtual void BindShaderAttributesAndUniforms();

private:
    GLuint m_item, m_animationItem;
};

