#pragma once
#include "GenericActor.h"

class ActorSun : public TexturedActor
{
public:
    ActorSun(GLfloat x, GLfloat y, GLfloat z, GLfloat size = 200);
    virtual ~ActorSun();

    virtual void Draw();
    virtual void Animate(GLint elapsedTime);

protected:
    virtual void BindShaderAttributesAndUniforms();

private:
    GLfloat m_angle;
    GLfloat m_rotaryStartAngle;
    GLfloat m_rotaryEndAngle;

    void RecalcAngles();
};
