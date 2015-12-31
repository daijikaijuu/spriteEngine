#pragma once
#include "homework02.h"

#define GLSL(src) "#version 400\n" #src

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
