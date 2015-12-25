#pragma once
#include "GenericActor.h"

class ActorSun :
    public GenericActor
{
public:
    ActorSun(double x, double y, double size = 200);
    virtual ~ActorSun();

    virtual void Draw();
    virtual void Animate();

private:
    double m_size;
    double m_angle;
    double m_rotaryStartAngle;
    double m_rotaryEndAngle;

    void RecalcAngles();
};

