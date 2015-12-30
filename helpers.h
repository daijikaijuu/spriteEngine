#pragma once
#include "homework02.h"

#define GLSL(src) "#version 330\n" #src

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
};

struct TexturedVertex
{
    glm::vec3 position;
    glm::vec2 texture;
    glm::vec3 color;
};

void drawCircle(double x, double y, double radius);
void drawRadialGradientCircle(double x, double y, double radius, const double innerColor[], const double outterColor[]);

