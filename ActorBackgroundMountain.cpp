#include <time.h>
#include "homework02.h"
#include "ActorBackgroundMountain.h"
#include "Shader.h"

#define IBUFSIZE (1<<16)
#define OCTAVEN 6

GLfloat fsc(GLfloat i)
{
    return 0.5f*(1.0f - cos(i*3.1415926535f));
}

ActorBackgroundMountain::ActorBackgroundMountain(GLfloat width, GLfloat height, GLfloat z) :
    GenericActor(width, height, z)
{
    CalculateHeights();

    m_shader = new Shader("shaders/mountain");

    std::vector<GLfloat> heights = CalculateHeights();
    const GLuint size = (GLuint)heights.size();
    Vertex *a = new Vertex[size];
    a[0].position = glm::vec3(0, m_y, m_z);
    a[0].color = glm::vec3(1, 0, 0);
    for (size_t i = 1; i < size; i++)
    {
        a[i].position = glm::vec3(i, 100 + heights[i] * 100, m_z);
        a[i].color = glm::vec3(1.0f, 1.0f, 1.0f);
    }
    a[size-1].position = glm::vec3(m_x, m_y, m_z);
    a[size-1].color = glm::vec3(1, 0, 0);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * size, a, GL_STATIC_DRAW);

    // Create VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    m_shader->Bind();
    m_shader->RegisterAttribute("pos");
    m_shader->RegisterAttribute("color");
    m_shader->RegisterUniform("projection");
    m_shader->RegisterUniform("windowHeight");

    GLint posAttrib = m_shader->GetAttributeLocation("pos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

    GLint colAttrib = m_shader->GetAttributeLocation("color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    
    GLint projection = m_shader->GetUniformLocation("projection");
    if (projection != -1)
    {
        glm::mat4 p = glm::ortho(0.0f, 1.0f * 800, 1.0f * 600, 0.0f);
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(p));
    }
    GLint windowHeight = m_shader->GetUniformLocation("windowHeight");
    if (windowHeight != -1)
        glUniform1i(windowHeight, (GLuint)m_y);
    m_shader->UnBind();

    delete a;
}


ActorBackgroundMountain::~ActorBackgroundMountain()
{
}

void ActorBackgroundMountain::Draw()
{
    GenericActor::Draw();

    m_shader->Bind();
    glDrawArrays(GL_POLYGON, 0, 800);
    m_shader->UnBind();
}

std::vector<GLfloat> ActorBackgroundMountain::CalculateHeights()
{
    const int step = int(m_x);
    srand((unsigned)time(NULL));

    GLfloat buffer[4096];
    unsigned char ucbuf[IBUFSIZE];
    unsigned char wtb[256];

    int i;
    for (i = 0; i < 4096; i++)
        buffer[i] = rand() / 32767.0f;
    for (i = 0; i < IBUFSIZE; i++)
        ucbuf[i] = rand();
    for (i = 0; i < 256; i++)
        wtb[i] = (unsigned char)(255 * fsc(i / 256.0f));

    std::vector<GLfloat> result;
    
    for (int i = 0; i < step; i++)
    {
        GLfloat x = i / m_x;

        if (x < 0) x = -x;
        GLfloat r = 0;
        GLfloat ampl = 0.6f;
        int xi = (int)x;
        GLfloat xf = x - xi;

        for (int i = 0; i < OCTAVEN; i++)
        {
            GLfloat n1;
            n1 = buffer[xi & 4095];
            n1 += fsc(xf)*(buffer[(xi + 1) & 4095] - n1);
            r += n1*ampl;
            ampl *= 0.9f;
            xi <<= 1; xf *= 2;
            if (xf >= 1.0f) xi++, xf -= 1.0f;
        }

        result.push_back(r);
    }

    return result;
}
