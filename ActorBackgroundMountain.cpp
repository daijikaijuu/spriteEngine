#include "homework02.h"
#include "ActorBackgroundMountain.h"
#include <time.h>

#define IBUFSIZE (1<<16)
#define OCTAVEN 6

double fsc(double i)
{
    return 0.5f*(1.0f - cos(i*3.1415926535f));
}

ActorBackgroundMountain::ActorBackgroundMountain(double width, double height) :
    GenericActor(width, height),
    m_heights(NULL)
{
    CalculateHeights();
}


ActorBackgroundMountain::~ActorBackgroundMountain()
{
    if (m_heights)
    {
        delete m_heights;
        m_heights = NULL;
    }
}

void ActorBackgroundMountain::Draw()
{
    glColor3f(0.0f, 0.2f, 0.0f);
    const int step = (int)m_x;
    glBegin(GL_POLYGON);
    glVertex2d(0, 0);
    for (int i = 1; i < step; i++)
    {
        double y = m_heights[i] / m_y;
        glColor3d(y, y, y);
        glVertex2d(i, m_heights[i]);
    }
    glColor3f(0.0f, 0.2f, 0.0f);
    glVertex2d(m_x, 0);
    glEnd();
}

void ActorBackgroundMountain::Animate()
{
    Draw();
}

void ActorBackgroundMountain::CalculateHeights()
{
    const int step = (int)m_x;
    m_heights = new double[step];
    srand((unsigned)time(NULL));

    double buffer[4096];
    unsigned char ucbuf[IBUFSIZE];
    unsigned char wtb[256];

    int i;
    for (i = 0; i < 4096; i++)
        buffer[i] = rand() / 32767.0f;
    for (i = 0; i < IBUFSIZE; i++)
        ucbuf[i] = rand();
    for (i = 0; i < 256; i++)
        wtb[i] = (unsigned char)(255 * fsc(i / 256.0f));


    for (int i = 0; i < step; i++)
    {
        double x = i / m_x;

        if (x < 0) x = -x;
        double r = 0;
        double ampl = 0.6f;
        int xi = (int)x;
        double xf = x - xi;

        for (int i = 0; i < OCTAVEN; i++)
        {
            double n1;
            n1 = buffer[xi & 4095];
            n1 += fsc(xf)*(buffer[(xi + 1) & 4095] - n1);
            r += n1*ampl;
            ampl *= 0.9f;
            xi <<= 1; xf *= 2;
            if (xf >= 1.0f) xi++, xf -= 1.0f;
        }

        m_heights[i] = r * 150;
    }
}
