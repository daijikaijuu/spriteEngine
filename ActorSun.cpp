#include "homework02.h"
#include "ActorSun.h"
#include "helpers.h"
#include "Shader.h"

ActorSun::ActorSun(GLfloat x, GLfloat y, GLfloat z, GLfloat size) :
    GenericActor(x, y, z),
    m_size(size),
    m_angle(0),
    m_rotaryStartAngle(0),
    m_rotaryEndAngle(0)
{
    RecalcAngles(); 

    m_shader->Load("shaders/sun");

    const GLuint numberOfSides = 32;
    const GLuint numberofVertices = numberOfSides + 2;
    GLfloat radius = size / 2;

    Vertex a[numberofVertices];
    a[0].position = glm::vec3(0.0f, 0.0f, m_z);
    a[0].color = glm::vec3(1.0f, 1.0f, 1.0f);
    for (size_t i = 1; i < numberofVertices; i++)
    {
        a[i].position = glm::vec3(radius * cos(i * PI * 2 / numberOfSides),
                                  radius * sin(i * PI * 2 / numberOfSides),
                                  m_z);
        a[i].color = glm::vec3(1.0f, 1.0f, 0.0f);
    }

    m_VAO->GetVBO()->Bind(GL_ARRAY_BUFFER);
    m_VAO->GetVBO()->AddData(a, sizeof(a));
    m_VAO->GetVBO()->UploadDataToGPU(GL_STATIC_DRAW);

    m_shader->Bind();
    m_shader->RegisterAttribute("pos");
    m_shader->RegisterAttribute("color");
    m_shader->RegisterUniform("projection");
    m_shader->RegisterUniform("modelview");

    m_VAO->Generate(m_shader->GetAttributeLocation("pos"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    m_VAO->Generate(m_shader->GetAttributeLocation("color"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    GLint projection = m_shader->GetUniformLocation("projection");
    if (projection != -1)
    {
        glm::mat4 p = glm::ortho(0.0f, 1.0f * 800, 1.0f * 600, 0.0f);
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(p));
    }
    m_modelview = m_shader->GetUniformLocation("modelview");
    Move(0, 0); // Update modelview

    m_shader->UnBind();
}

ActorSun::~ActorSun()
{
}

void ActorSun::Draw()
{
    GenericActor::Draw();

    m_shader->Bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 34);
    m_shader->UnBind();
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