#include "../homework02.h"
#include "../helpers.h"
#include "../Render/Vertex.h"
#include "../Render/Shader.h"
#include "ActorSun.h"

ActorSun::ActorSun(GLfloat x, GLfloat y, GLfloat z, GLfloat size) :
    GenericActor(x, y, z),
    m_size(size),
    m_angle(0),
    m_rotaryStartAngle(0),
    m_rotaryEndAngle(0)
{
    RecalcAngles(); 

    m_shader->Load("Data/Shaders/sun");

    ShapeData<Vertex> *vertexData = shapeGenerator::generateCircle(m_size / 2, m_z, glm::vec3(1.0f, 1.0f, 0.0f));

    m_VAO->GetVBO()->Bind(GL_ARRAY_BUFFER);
    m_VAO->GetVBO()->AddData(vertexData->vertices, vertexData->vertexBufferSize());
    m_VAO->GetVBO()->UploadDataToGPU(GL_STATIC_DRAW);

    m_shader->Bind();
    m_shader->RegisterAttribute({ "pos", "color" });
    m_shader->RegisterUniform({ "projection", "modelview" });

    m_VAO->Generate<Vertex>(m_shader, vertexData, "pos", 0);
    m_VAO->Generate<Vertex>(m_shader, vertexData, "color", 1);

    GLint projection = m_shader->GetUniformLocation("projection");
    if (projection != -1)
    {
        glm::mat4 p = glm::ortho(0.0f, 1.0f * 800, 1.0f * 600, 0.0f);
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(p));
    }
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
    GenericActor::Draw();

    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_ALPHA, GL_ONE); 
    m_shader->Bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 34);
    m_shader->UnBind();
    glDisable(GL_BLEND);
    //double radius = m_size / 3;
    //double innerColor[] = { 0.6f, 0.6f, 0.0f };
    //double outterColor[] = { 1.0f, 1.0f, 0.0f };
    //drawRadialGradientCircle(m_x, m_y, radius, outterColor, innerColor);

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

void ActorSun::Animate()
{
    Draw();

    if (m_animated)
    {
        m_angle += 0.005f;
        RecalcAngles();
    }
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