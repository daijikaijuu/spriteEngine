#pragma once
#include "GenericActor.h"

class ActorCloud :
    public GenericActor
{
public:
    ActorCloud(double x, double y, double width, double height, int octave = 4);
    ~ActorCloud();

    virtual void Draw();
    virtual void Animate();

private:
    double m_width, m_height;
    double m_map32[32 * 32];
    double m_map256[256 * 256];
    char m_texture[256][256][4];
    int m_octave;

    void SetNoise();
    void OverlapOctaves();
    double Interpolate(double x, double y, double *map);
};

