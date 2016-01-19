#pragma once
#include "GenericActor.h"

class ActorSun : public TexturedActor
{
public:
    ActorSun(GLfloat x, GLfloat y, GLfloat z, GLfloat size = 200);
    virtual ~ActorSun();

    virtual void Draw();
    virtual void Animate(GLint elapsedTime);

    virtual string class_type() const { return "ActorSun"; }

protected:
    virtual void BindShaderAttributesAndUniforms();
};
