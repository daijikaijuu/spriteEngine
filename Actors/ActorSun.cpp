#include "../homework02.h"
#include "../helpers.h"
#include "../Render/Vertex.h"
#include "../Render/Shader.h"
#include "../Render/Texture.h"
#include "../Render/TextureManager.h"
#include "ActorSun.h"

ActorSun::ActorSun(GLfloat x, GLfloat y, GLfloat z, GLfloat size) :
    TexturedActor(x, y, size, z),
    m_angle(0),
    m_rotaryStartAngle(0),
    m_rotaryEndAngle(0),
    m_modelview(0)
{
    RecalcAngles(); 

    m_shader->Load("Data/Shaders/sun");
    m_texture = TextureManager::GetInstance()->GetTexture("Data/Textures/light.png");

    ShapeData<TexturedVertex> *vertexData = shapeGenerator::generateTexturedQuad(0, 0, m_z, m_size, m_size);

    m_VAO->GetVBO()->Bind(GL_ARRAY_BUFFER);
    m_VAO->GetVBO()->AddData(vertexData->vertices, vertexData->vertexBufferSize());
    m_VAO->GetVBO()->UploadDataToGPU(GL_STATIC_DRAW);

    m_shader->Bind();
    m_shader->RegisterAttribute({ "inPosition", "inCoord" });
    m_shader->RegisterUniform({ "projection", "modelview", "gSampler" });

    m_VAO->Generate<TexturedVertex>(m_shader, vertexData, "inPosition", 0);
    m_VAO->Generate<TexturedVertex>(m_shader, vertexData, "inCoord", 1);

    GLint projection = m_shader->GetUniformLocation("projection");
    if (projection != -1)
    {
        glm::mat4 p = glm::ortho(0.0f, 1.0f * 800, 1.0f * 600, 0.0f);
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(p));
    }
    GLuint sampler = m_shader->GetUniformLocation("gSampler");
    glUniform1i(sampler, 0);
    m_modelview = m_shader->GetUniformLocation("modelview");
    Move(0, 0); // Update modelview

    m_shader->UnBind();

    delete vertexData;
}

ActorSun::~ActorSun()
{
}

void ActorSun::Draw()
{
    TexturedActor::Draw();

    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_ALPHA, GL_ONE); 
    m_shader->Bind();
    glDrawArrays(GL_QUADS, 0, 4);
    m_shader->UnBind();
    glDisable(GL_BLEND);

    //double length = radius;
    //double rays = 16;
    //double theta = m_rotaryStartAngle;
    //double angleAmount = (m_rotaryEndAngle - m_rotaryStartAngle) / rays;

    //glColor3dv(outterColor);
    //for (int i = 0; i < rays; i++)
    //{
    //    double x1 = m_x + cos(theta) * radius * 1.2f;
    //    double y1 = m_y + sin(theta) * radius * 1.2f;
    //    double x2 = x1 + cos(theta) * length;
    //    double y2 = y1 + sin(theta) * length;
    //    theta += angleAmount;
    //    glBegin(GL_LINES);
    //    glVertex2d(x1, y1);
    //    glVertex2d(x2, y2);
    //    glEnd();
    //}
}

void ActorSun::Animate(GLint elapsedTime)
{
    GenericActor::Animate(elapsedTime);

    m_angle += 0.005f;
    RecalcAngles();
}

void ActorSun::Move(GLfloat xShift, GLfloat yShift)
{
    GenericActor::Move(xShift, yShift);

    m_shader->Bind();
    if (m_modelview != -1)
    {
        glm::mat4 p = glm::translate(glm::mat4(1.0f), glm::vec3(m_x, m_y, 0.0f));
        glUniformMatrix4fv(m_modelview, 1, GL_FALSE, glm::value_ptr(p));
    }
    m_shader->UnBind();
}

void ActorSun::RecalcAngles()
{
    m_rotaryStartAngle = GLfloat(m_angle * PI / 180);
    m_rotaryEndAngle = GLfloat((m_angle + 360) * PI / 180);
}