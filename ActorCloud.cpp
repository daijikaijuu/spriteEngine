#include <stdlib.h>
#include <math.h>
#include "homework02.h"
#include "ActorCloud.h"
#include "helpers.h"

ActorCloud::ActorCloud(double x, double y, double width, double height) :
    GenericActor(x, y),
    m_width(width),
    m_height(height),
    m_map32(),
    m_texture()
{
    SetNoise();
    OverlapOctaves();

    double cover = 10.0f;
    double sharpness = 0.99f;
    for (int x = 0; x < 256 * 256; x++)
    {
        double c = m_map256[x] - (255.0f - cover);
        if (c < 0)
            c = 0;
        m_map256[x] = 255.0f - ((double)(pow(sharpness, c))*255.0f);
    }

    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            double color = m_map256[i * 256 + j];
            m_texture[i][j][0] = color;
            m_texture[i][j][1] = color;
            m_texture[i][j][2] = color;
            m_texture[i][j][3] = color;
        }
    }

    unsigned int ID;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_texture);
}

ActorCloud::~ActorCloud()
{
}

void ActorCloud::Draw()
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
    glTexCoord2d(1, 1); glVertex2d(m_x + m_width, m_y + m_height);
    glTexCoord2d(0, 1); glVertex2d(m_x, m_y + m_height);
    glTexCoord2d(0, 0); glVertex2d(m_x, m_y);
    glTexCoord2d(1, 0); glVertex2d(m_x + m_width, m_y);
    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void ActorCloud::Animate()
{
    Draw();
}

void ActorCloud::SetNoise()
{
    double temp[34][34];
    int random = rand() % 5000;

    for (int y = 0; y < 33; y++)
    {
        for (int x = 0; x < 33; x++)
        {
            temp[x][y] = 128.0f + noise(x, y, random) * 128.0f;
        }
    }

    for (int x = 1; x < 33; x++)
    {
        temp[0][x] = temp[32][x];
        temp[33][x] = temp[1][x];
        temp[x][0] = temp[x][32];
        temp[x][33] = temp[x][1];
    }
    temp[0][0] = temp[32][32];
    temp[33][33] = temp[1][1];
    temp[0][33] = temp[32][1];
    temp[33][0] = temp[1][32];


    for (int y = 1; y < 33; y++)
    {
        for (int x = 1; x < 33; x++)
        {
            double center = temp[x][y] / 4.0f;
            double sides = (temp[x + 1][y] + temp[x - 1][y] + temp[x][y + 1] + temp[x][y - 1]) / 8.0f;
            double corners = (temp[x + 1][y + 1] + temp[x + 1][y - 1] + temp[x - 1][y + 1] + temp[x - 1][y - 1]) / 16.0f;
            m_map32[((x - 1) * 32) + (y - 1)] = center + sides + corners;
        }
    }
}

void ActorCloud::OverlapOctaves()
{
    for (int x = 0; x < 256 * 256; x++)
    {
        m_map256[x] = 0;
    }

    for (int octave = 0; octave < 5; octave++)
    {
        for (int x = 0; x < 256; x++)
        {
            for (int y = 0; y < 256; y++)
            {
                double scale = 1 / pow(2, 3 - octave);
                double noise = Interpolate(x * scale, y * scale, m_map32);
                m_map256[(y * 256) + x] += noise / pow(2, octave);
            }
        }
    }
}

double ActorCloud::Interpolate(double x, double y, double *map)
{
    int Xint = (int)x;
    int Yint = (int)y;

    double Xfrac = x - Xint;
    double Yfrac = y - Yint;

    int X0 = Xint % 32;
    int Y0 = Yint % 32;
    int X1 = (Xint + 1) % 32;
    int Y1 = (Yint + 1) % 32;

    double bot = map[X0 * 32 + Y0] + Xfrac * (map[X1 * 32 + Y0] - map[X0 * 32 + Y0]);
    double top = map[X0 * 32 + Y1] + Xfrac * (map[X1 * 32 + Y1] - map[X0 * 32 + Y1]);

    return (bot + Yfrac * (top - bot));
}