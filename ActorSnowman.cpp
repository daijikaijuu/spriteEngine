#include "Render/Vertex.h"
#include "ActorSnowman.h"

ActorSnowman::ActorSnowman(GLfloat x, GLfloat y, GLfloat z, GLfloat size) :
    TexturedActor(x, y, size, z)
{
    m_shader->Load("Data/Shaders/basic", "Data/Shaders/snowflake");

    TexturedShape * vertexData = shapeGenerator::generateTexturedQuad(0, 0, m_z, size, size);
    InitializeVBO<TexturedShape>(vertexData);

    BindShaderAttributesAndUniforms();

    SetAttribute<TexturedShape>(vertexData, "inPosition", 0);
    SetAttribute<TexturedShape>(vertexData, "inCoord", 1);

    delete vertexData;
}

ActorSnowman::~ActorSnowman()
{}
