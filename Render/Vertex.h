#pragma once

#include "../homework02.h"

typedef glm::vec3 Vertex;

struct VertexColored
{
    glm::vec3 position;
    glm::vec3 color;
};

struct TexturedVertex
{
    glm::vec3 position;
    glm::vec2 texture;
};

template<typename T>
struct ShapeData
{
    ShapeData() :
        vertices(NULL),
        numVertices(0)
    {}
    ~ShapeData()
    {
        if (vertices)
        {
            delete vertices;
            vertices = NULL;
        }
    }

    T *vertices;
    GLuint numVertices;

    GLsizeiptr vertexBufferSize() const
    {
        return numVertices * sizeof(T);
    }
    GLsizei itemSize() const
    {
        return sizeof(T);
    }
    GLvoid* position(GLuint pos)
    {
        assert(pos >= 0);
        assert(pos < 2);
        if (pos == 0)
            return NULL;
        return (GLvoid*)sizeof(glm::vec3);
    }
};

class shapeGenerator
{
public:
    static ShapeData<VertexColored>* generateQuad(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height, glm::vec3 color)
    {
        const GLuint size = 4;

        ShapeData<VertexColored> *ret = new ShapeData<VertexColored>();

        VertexColored data[size] = {
            glm::vec3(x - width / 2, y - height / 2, z), color,
            glm::vec3(x - width / 2, y + height / 2, z), color,
            glm::vec3(x + width / 2, y + height / 2, z), color,
            glm::vec3(x + width / 2, y - height / 2, z), color,
        };

        ret->numVertices = size;
        ret->vertices = new VertexColored[ret->numVertices];
        memcpy(ret->vertices, data, sizeof(data));

        return ret;
    }

    static ShapeData<TexturedVertex>* generateTexturedQuad(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height)
    {
        const GLuint size = 4;

        ShapeData<TexturedVertex> *ret = new ShapeData<TexturedVertex>();

        TexturedVertex data[size] = {
            glm::vec3(x - width / 2, y - height / 2, z), glm::vec2(0, 1),
            glm::vec3(x - width / 2, y + height / 2, z), glm::vec2(0, 0),
            glm::vec3(x + width / 2, y + height / 2, z), glm::vec2(1, 0),
            glm::vec3(x + width / 2, y - height / 2, z), glm::vec2(1, 1),
        };

        ret->numVertices = size;
        ret->vertices = new TexturedVertex[ret->numVertices];
        memcpy(ret->vertices, data, sizeof(data));

        return ret;
    }

    static ShapeData<VertexColored>* generateCircle(GLfloat radius, GLfloat z, glm::vec3 color)
    {
        ShapeData<VertexColored> *ret = new ShapeData<VertexColored>();

        const GLuint numberOfSides = 32;
        const GLuint numberofVertices = numberOfSides + 2;

        VertexColored data[numberofVertices];
        data[0].position = glm::vec3(0.0f, 0.0f, z);
        data[0].color = color;
        for (size_t i = 1; i < numberofVertices; i++)
        {
            data[i].position = glm::vec3(radius * cos(i * PI * 2 / numberOfSides),
                                      radius * sin(i * PI * 2 / numberOfSides),
                                      z);
            data[i].color = color;
        }

        ret->numVertices = numberofVertices;
        ret->vertices = new VertexColored[ret->numVertices];
        memcpy(ret->vertices, data, sizeof(data));

        return ret;
    }
};