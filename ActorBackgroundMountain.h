#pragma once

#include "GenericActor.h"
#include "helpers.h"

class ActorBackgroundMountain :
    public GenericActor
{
public:
    ActorBackgroundMountain(double width, double height);
    virtual ~ActorBackgroundMountain();

    virtual void Draw();
    virtual void Animate();

private:
    double *m_heights;

    void CalculateHeights();
};

