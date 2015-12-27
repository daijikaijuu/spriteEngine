#include "homework02.h"
#include "helpers.h"

void drawCircle(double x, double y, double radius)
{
    glBegin(GL_POLYGON);
    for (double i = 0; i < 2 * PI; i += PI / 12)
    {
        glVertex2d(x + cos(i) * radius, y + sin(i) * radius);
    }
    glEnd();
}

void drawRadialGradientCircle(double x, double y, double radius, const double innerColor[], const double outterColor[])
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3dv(innerColor);
    glVertex2d(x, y);
    glColor3dv(outterColor);
    for (double i = 0; i < 2 * PI; i += PI / 12)
    {
        glVertex2d(x + cos(i) * radius, y + sin(i) * radius);
    }
    glVertex2d(x + radius, y);
    glEnd();
}

double noise(int x, int y, int random)
{
    int n = x + y * 57 + random * 131;
    n = (n << 13) ^ 2;

    return (1.0f - ((n * (n * n * 15731 +789221) + 1376312589) &
                    0x7fffffff)* 0.000000000931322574615478515625f);
}
