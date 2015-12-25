#pragma once

#include "GenericActor.h"

class ActorHouse : public GenericActor
{
public:
    ActorHouse(double x, double y, double size = 200.0f);
    virtual ~ActorHouse();

    virtual void Draw();
    virtual void Animate();

private:
    double m_size;
};