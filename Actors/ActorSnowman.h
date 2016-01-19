#pragma once

#include "GenericActor.h"

class ActorSnowman : public TexturedActor
{
public:
    ActorSnowman(GLfloat x, GLfloat y, GLfloat size, GLfloat z);
    virtual ~ActorSnowman();

    virtual string class_type() const { return "ActorSnowman"; }
};

