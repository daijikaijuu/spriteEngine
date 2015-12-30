#pragma once
#include "homework02.h"
#include "GenericActor.h"

class Texture;

class ActorCloud :
    public GenericActor
{
public:
    ActorCloud(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLfloat z, int octave = 4);
    ~ActorCloud();

    virtual void Draw();

private:
    GLfloat m_width, m_height;
    GLfloat m_map32[32 * 32];
    GLfloat m_map256[256 * 256];
    char m_textureArray[256][256][4];
    int m_octave;
    Texture *m_texture;

    void SetNoise();
    void OverlapOctaves();
    GLfloat noise(int x, int y, int random);
    GLfloat Interpolate(GLfloat x, GLfloat y, GLfloat *map);
};

