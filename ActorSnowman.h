#pragma once
#include "GenericActor.h"

class ActorSnowman :
    public GenericActor
{
public:
    ActorSnowman(double x, double y, double height = 200);
    ~ActorSnowman();

    virtual void Draw();
    virtual void Animate();

private:
    double m_height;
};

