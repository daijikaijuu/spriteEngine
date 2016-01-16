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
    GenericActor(0, 0, 0, z)
{
    CalculateHeights();

    InitializeShader("Data/Shaders/basic", "Data/Shaders/mountain");

    std::vector<GLfloat> heights = CalculateHeights();
    const GLuint size = (GLuint)heights.size();
    VertexColored *a = new VertexColored[size];
    a[0].position = glm::vec3(0, 600, m_z);
    a[0].color = glm::vec3(1, 0, 0);
    for (size_t i = 1; i < size; i++)
    {
        a[i].position = glm::vec3(i, 100 + heights[i] * 100, m_z);
        a[i].color = glm::vec3(1.0f, 1.0f, 1.0f);
    }
    a[size-1].position = glm::vec3(800, 600, m_z);
    a[size-1].color = glm::vec3(1, 0, 0);

    m_VAO->GetVBO()->Bind(GL_ARRAY_BUFFER);
    m_VAO->GetVBO()->AddData(a, sizeof(GLfloat) * 6 * size);
    m_VAO->GetVBO()->UploadDataToGPU(GL_STATIC_DRAW);

    BindShaderAttributesAndUniforms();

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

void ActorBackgroundMountain::ResizeScene(GLsizei width, GLsizei height)
{
    GenericActor::ResizeScene(width, height);

    GLint windowHeight = m_shader->GetUniformLocation("windowHeight");
    if (windowHeight != -1)
    {
        m_shader->Bind();
        glUniform1i(windowHeight, (GLsizei)height);
        m_shader->UnBind();
    }
}

void ActorBackgroundMountain::BindShaderAttributesAndUniforms()
{
    GenericActor::BindShaderAttributesAndUniforms();

    m_shader->RegisterAttribute({ "inPosition", "inColor" });
    m_shader->RegisterUniform("windowHeight");

    m_VAO->Generate(m_shader->GetAttributeLocation("inPosition"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    m_VAO->Generate(m_shader->GetAttributeLocation("inColor"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}

std::vector<GLfloat> ActorBackgroundMountain::CalculateHeights()
{
    const int step = int(800);
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
        GLfloat x = i / 800.0f;

        if (x < 0) x = -x;

        GLfloat r = 0;
        GLfloat ampl = 0.6f;
        int xi = (int)x;
        GLfloat xf = x - xi;

        for (int i = 0; i < OCTAVEN; i++)
        {
            GLfloat n1;
            n1 = buffer[xi & 4095];
            n1 += fsc(xf) * (buffer[(xi + 1) & 4095] - n1);
            r += n1 * ampl;
            ampl *= 0.9f;
            xi <<= 1;
            xf *= 2;

            if (xf >= 1.0f) xi++, xf -= 1.0f;
        }

        result.push_back(r);
    }

    return result;
}
