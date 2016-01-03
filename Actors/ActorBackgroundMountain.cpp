#include <time.h>
#include "../homework02.h"
#include "ActorBackgroundMountain.h"
#include "../Render/Shader.h"
#include "../Render/Vertex.h"

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

    m_shader->Load("Data/Shaders/mountain");

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

    m_VAO->GetVBO()->Bind(GL_ARRAY_BUFFER);
    m_VAO->GetVBO()->AddData(a, sizeof(GLfloat) * 6 * size);
    m_VAO->GetVBO()->UploadDataToGPU(GL_STATIC_DRAW);

    m_shader->Bind();
    m_shader->RegisterAttribute({ "pos", "color" });
    m_shader->RegisterUniform({ "projection", "windowHeight" });

    m_VAO->Generate(m_shader->GetAttributeLocation("pos"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    m_VAO->Generate(m_shader->GetAttributeLocation("color"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
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
