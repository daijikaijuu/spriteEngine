#pragma once
#include "GenericActor.h"

class Texture;

class ActorSun :
    public GenericActor
{
public:
    ActorSun(GLfloat x, GLfloat y, GLfloat z, GLfloat size = 200);
    virtual ~ActorSun();

    virtual void Draw();
    virtual void Animate();

    virtual void Move(GLfloat xShift, GLfloat yShift);

private:
    GLfloat m_angle;
    GLfloat m_rotaryStartAngle;
    GLfloat m_rotaryEndAngle;
    GLint m_modelview;
    Texture *m_lightTexture;

    void RecalcAngles();
};
