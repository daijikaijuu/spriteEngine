#pragma once
#include "../homework02.h"
#include "GenericActor.h"

class ActorCloud : public TexturedActor
{
public:
    ActorCloud(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLfloat z, int octave = 4);
    ~ActorCloud();

    virtual void Draw();

    virtual std::string class_type() { return "ActorCloud"; }

protected:
    virtual void BindShaderAttributesAndUniforms();

private:
    GLfloat m_width, m_height;
    GLfloat m_map32[32 * 32];
    GLfloat m_map256[256 * 256];
    char m_textureArray[256][256][4];
    int m_octave;

    void SetNoise();
    void OverlapOctaves();
    GLfloat noise(int x, int y, int random);
    GLfloat Interpolate(GLfloat x, GLfloat y, GLfloat *map);
};

