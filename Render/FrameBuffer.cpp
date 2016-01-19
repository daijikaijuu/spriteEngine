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

    m_shader->Load("Data/Shaders/basic", "Data/Shaders/basic");

    TexturedShape *vertexData = shapeGenerator::generateTexturedQuad(400, 300, 0, 800, 600);
    InitializeVBO<TexturedShape>(vertexData);

    BindShaderAttributesAndUniforms();

    SetAttribute<TexturedShape>(vertexData, "inPosition", 0);
    SetAttribute<TexturedShape>(vertexData, "inCoord", 1);

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

bool FrameBuffer::BindFrameBuffer()
{
    if (!DEBUG_OK())
        return false;

    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    m_texture->BindTexture();
    m_shader->Bind();

    return true;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->Bind();
    glDrawArrays(GL_QUADS, 0, 4);
}

void FrameBuffer::AttachTexture()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->GetID(), 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Error("AtachTexture - Something went wrong!!!");
}

Texture* FrameBuffer::GetTexture() const
{
    return m_texture;
}

bool FrameBuffer::DEBUG_OK() const
{
    HW_ASSERT(m_ID > 0);
    HW_ASSERT(m_depthBuffer > 0);

    return true;
}

string FrameBuffer::DEBUG_DUMP() const
{
    std::stringstream result;
    result << GenericActor::DEBUG_DUMP();
    result << " CLASS: " << FrameBuffer::class_type() << endl;
    result << DUMP_VAR(m_ID) << DUMP_VAR(m_depthBuffer) << endl;
    return result.str();
}

void FrameBuffer::BindShaderAttributesAndUniforms()
{
    TexturedActor::BindShaderAttributesAndUniforms();

    m_shader->RegisterAttribute({ "inPosition", "inCoord" });
    m_shader->RegisterUniform("gSampler");

    glUniform1i(m_shader->GetUniformLocation("gSampler"), 0);
}
