#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() :
    m_ID(0),
    m_depthBuffer(0),
    m_width(0),
    m_height(0),
    m_texture(NULL),
    m_VAO(NULL),
    m_shader(NULL)
{
    m_VAO = new VertexArrayObject();
    m_texture = new Texture();
    m_shader = new Shader();

    glGenFramebuffers(1, &m_ID);
    glGenRenderbuffers(1, &m_depthBuffer);
    Bind();

    m_shader->Load("Data/Shaders/cloud");

    ShapeData<TexturedVertex> *vertexData = shapeGenerator::generateTexturedQuad(400, 300, 0, 800, 600);

    m_VAO->GetVBO()->Bind(GL_ARRAY_BUFFER);
    m_VAO->GetVBO()->AddData(vertexData->vertices, vertexData->vertexBufferSize());
    m_VAO->GetVBO()->UploadDataToGPU(GL_STATIC_DRAW);

    m_shader->Bind();
    m_shader->RegisterAttribute({ "inPosition", "inCoord" });
    m_shader->RegisterUniform({ "projectionMatrix", "gSampler" });

    m_VAO->Generate<TexturedVertex>(m_shader, vertexData, "inPosition", 0);
    m_VAO->Generate<TexturedVertex>(m_shader, vertexData, "inCoord", 1);

    GLint projection = m_shader->GetUniformLocation("projectionMatrix");
    if (projection != -1)
    {
        glm::mat4 p = glm::ortho(0.0f, 1.0f * 800, 1.0f * 600, 0.0f);
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(p));
    }
    GLuint samplerLoc = m_shader->GetUniformLocation("gSampler");
    glUniform1i(samplerLoc, 0);

    m_shader->UnBind();

    Resize(800, 600);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

    delete vertexData;
}

FrameBuffer::~FrameBuffer()
{
    if (m_texture)
    {
        delete m_texture;
        m_texture = NULL;
    }
    if (m_shader)
    {
        delete m_shader;
        m_shader = NULL;
    }
    if (m_VAO)
    {
        delete m_VAO;
        m_texture = NULL;
    }
    UnBind();
    glDeleteRenderbuffers(1, &m_depthBuffer);
    glDeleteFramebuffers(1, &m_ID);
}

void FrameBuffer::Resize(GLsizei width, GLsizei height)
{
    Bind();
    
    m_texture->LoadTexture(width, height, NULL);
    m_texture->setFiltering();

    AttachTexture(m_texture);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    m_texture->BindTexture();
}

void FrameBuffer::UnBind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Render()
{
    UnBind();
    m_VAO->Bind();
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    //glViewport(0, 0, 800, 600);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    m_shader->Bind();
    m_texture->BindTexture();
    glDrawArrays(GL_QUADS, 0, 4);
}

void FrameBuffer::AttachTexture(Texture *texture)
{
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetID(), 0);

#ifdef _DEBUG
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Something went wrong with frame buffer" << std::endl;
#endif // DEBUG

}
