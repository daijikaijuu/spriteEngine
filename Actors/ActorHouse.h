#pragma once

#include "../homework02.h"
#include "GenericActor.h"

class ActorHouse : public TexturedActor
{
public:
    ActorHouse(GLfloat x, GLfloat y, GLfloat z, GLfloat size);
    virtual ~ActorHouse();

    virtual void Draw();

    virtual string class_type() const { return "ActorHouse"; }

protected:
    virtual void BindShaderAttributesAndUniforms();
};
