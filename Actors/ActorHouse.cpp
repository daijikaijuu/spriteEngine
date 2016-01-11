#include "../Render/Texture.h"
#include "../Render/TextureManager.h"
#include "../Render/Vertex.h"
#include "../helpers.h"
#include "ActorHouse.h"

ActorHouse::ActorHouse(GLfloat x, GLfloat y, GLfloat z, GLfloat size) :
    TexturedActor(x, y, size, z)
{
    InitializeShader("Data/Shaders/basic", "Data/Shaders/house");
    m_texture = TextureManager::GetInstance()->GetTexture("Data/Textures/house.png");

    TexturedShape *vertexData = shapeGenerator::generateTexturedQuad(0, -m_size / 2, m_z, m_size, m_size);
    InitializeVBO<TexturedShape>(vertexData);

    BindShaderAttributesAndUniforms();

    SetAttribute<TexturedShape>(vertexData, "inPosition", 0);
    SetAttribute<TexturedShape>(vertexData, "inCoord", 1);

    delete vertexData;
}

ActorHouse::~ActorHouse()
{
}

void ActorHouse::Draw()
{
    TexturedActor::Draw();

    m_shader->Bind();
    glDrawArrays(GL_QUADS, 0, 4);
    m_shader->UnBind();
}

void ActorHouse::BindShaderAttributesAndUniforms()
{
    TexturedActor::BindShaderAttributesAndUniforms();

    m_shader->RegisterAttribute({ "inPosition", "inCoord" });
}
