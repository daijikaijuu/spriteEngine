#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() :
    Singleton<FrameBuffer>(),
    TexturedActor(0, 0, 800, 600),
    m_ID(0),
    m_depthBuffer(0)
{
    m_texture = new Texture();

    glGenFramebuffers(1, &m_ID);
    glGenRenderbuffers(1, &m_depthBuffer);
    BindFrameBuffer();

    m_shader->Load("Data/Shaders/cloud");

    ShapeData<TexturedVertex> *vertexData = shapeGenerator::generateTexturedQuad(400, 300, 0, 800, 600);

    m_VAO->GetVBO()->Bind(GL_ARRAY_BUFFER);
    m_VAO->GetVBO()->AddData(vertexData->vertices, vertexData->vertexBufferSize());
    m_VAO->GetVBO()->UploadDataToGPU(GL_STATIC_DRAW);

    BindShaderAttributesAndUniforms();

    m_VAO->Generate<TexturedVertex>(m_shader, vertexData, "inPosition", 0);
    m_VAO->Generate<TexturedVertex>(m_shader, vertexData, "inCoord", 1);

    ResizeScene(800, 600);

    delete vertexData;
}

FrameBuffer::~FrameBuffer()
{
    UnbindFrameBuffer();
    glDeleteRenderbuffers(1, &m_depthBuffer);
    glDeleteFramebuffers(1, &m_ID);
}

void FrameBuffer::ResizeScene(GLsizei width, GLsizei height)
{
    TexturedActor::ResizeScene(width, height);

    BindFrameBuffer();
    
    m_texture->LoadTexture(width, height, NULL);
    m_texture->setFiltering();

    AttachTexture();

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

    UnbindFrameBuffer();
}

void FrameBuffer::BindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    m_texture->BindTexture();
    m_shader->Bind();
}

void FrameBuffer::UnbindFrameBuffer()
{
    m_shader->UnBind();
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Draw()
{
    TexturedActor::Draw();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    m_shader->Bind();
    glDrawArrays(GL_QUADS, 0, 4);
}

void FrameBuffer::AttachTexture()
{
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->GetID(), 0);

#ifdef _DEBUG
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Something went wrong with frame buffer" << std::endl;
#endif // DEBUG

}

Texture* FrameBuffer::GetTexture() const
{
    return m_texture;
}

void FrameBuffer::BindShaderAttributesAndUniforms()
{
    TexturedActor::BindShaderAttributesAndUniforms();

    m_shader->RegisterAttribute({ "inPosition", "inCoord" });
    m_shader->RegisterUniform("gSampler");

    glUniform1i(m_shader->GetUniformLocation("gSampler"), 0);
}
