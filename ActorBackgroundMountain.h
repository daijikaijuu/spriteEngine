#pragma once

#include "GenericActor.h"
#include "helpers.h"

class Shader;

class ActorBackgroundMountain :
    public GenericActor
{
public:
    ActorBackgroundMountain(GLfloat width, GLfloat height, GLfloat z);
    virtual ~ActorBackgroundMountain();

    virtual void Draw();

private:
    std::vector<GLfloat> CalculateHeights();
};

