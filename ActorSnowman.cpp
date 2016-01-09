#include "Render/Vertex.h"
#include "ActorSnowman.h"

ActorSnowman::ActorSnowman(GLfloat x, GLfloat y, GLfloat z, GLfloat size) :
    TexturedActor(x, y, size, z)
{
    m_shader->Load("Data/Shaders/basic", "Data/Shaders/snowflake");

    ShapeData<TexturedVertex> * vertexData = shapeGenerator::generateTexturedQuad(0, 0, m_z, size, size);

}

ActorSnowman::~ActorSnowman()
{}
