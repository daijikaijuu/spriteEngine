#pragma once

#include "../helpers.h"
#include "GenericActor.h"

class Shader;

class ActorBackgroundMountain : public GenericActor
{
public:
    ActorBackgroundMountain(GLfloat width, GLfloat height, GLfloat z);
    virtual ~ActorBackgroundMountain();

    virtual void Draw();
    virtual void ResizeScene(GLsizei width, GLsizei height);

    virtual std::string class_type() { return "ActorBackgroundMountain"; }

protected:
    virtual void BindShaderAttributesAndUniforms();

private:
    std::vector<GLfloat> CalculateHeights();
};

