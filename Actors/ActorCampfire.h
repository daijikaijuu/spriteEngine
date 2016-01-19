#pragma once
#include "GenericActor.h"

class ActorCampfire : public TexturedActor
{
public:
    ActorCampfire(GLfloat x, GLfloat y, GLfloat z, GLfloat size);
    virtual ~ActorCampfire();

    virtual void Draw();
    virtual void Animate(GLint elapsedTime);

    virtual string class_type() const { return "ActorCampfire"; }
    virtual bool DEBUG_OK() const;
    virtual string DEBUG_DUMP() const;

protected:
    virtual void BindShaderAttributesAndUniforms();

private:
    GLuint m_item, m_animationItem;
};

