#include <stdlib.h>
#include <math.h>
#include "homework02.h"
#include "ActorCloud.h"
#include "helpers.h"
#include "Texture.h"

ActorCloud::ActorCloud(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLfloat z, int octave) :
    GenericActor(x, y, z),
    m_width(width),
    m_height(height),
    m_map32(),
    m_textureArray(),
    m_octave(octave),
    m_texture(NULL)
{
    m_shader = new Shader("shaders/cloud");
    m_texture = new Texture();

    SetNoise();
    OverlapOctaves();

    GLfloat cover = 15.0f;
    GLfloat sharpness = 0.99f;
    for (int x = 0; x < 256 * 256; x++)
    {
        GLfloat c = m_map256[x] - (255.0f - cover);
        if (c < 0)
            c = 0;
        m_map256[x] = 255.0f - ((GLfloat)(pow(sharpness, c))*255.0f);
    }

    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            GLfloat color = m_map256[i * 256 + j];
            m_textureArray[i][j][0] = (char)color;
            m_textureArray[i][j][1] = (char)color;
            m_textureArray[i][j][2] = (char)color;
            m_textureArray[i][j][3] = (char)color;
        }
    }

    TexturedVertex quad[4] = {
        glm::vec3(m_x, m_y, m_z),                      glm::vec2(0, 0), glm::vec3(1, 1, 1),
        glm::vec3(m_x, m_y + m_height, m_z),           glm::vec2(0, 1), glm::vec3(1, 1, 1),
        glm::vec3(m_x + m_width, m_y + m_height, m_z), glm::vec2(1, 1), glm::vec3(1, 1, 1),
        glm::vec3(m_x + m_width, m_y, m_z),            glm::vec2(1, 0), glm::vec3(1, 1, 1),
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    m_shader->Bind();
    m_shader->RegisterAttribute("inPosition");
    m_shader->RegisterAttribute("inCoord");
    m_shader->RegisterUniform("projectionMatrix");
    m_shader->RegisterUniform("gSampler");

    GLint posAttrib = m_shader->GetAttributeLocation("inPosition");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

    GLint texAttrib = m_shader->GetAttributeLocation("inCoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    GLint projection = m_shader->GetUniformLocation("projectionMatrix");
    if (projection != -1)
    {
        glm::mat4 p = glm::ortho(0.0f, 1.0f * 800, 1.0f * 600, 0.0f);
        glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(p));
    }
    
    GLuint samplerLoc = m_shader->GetUniformLocation("gSampler");
    glUniform1i(samplerLoc, 0);

    m_texture->LoadTexture(256, 256, m_textureArray);
    m_texture->setFiltering();

    m_shader->UnBind();
}

ActorCloud::~ActorCloud()
{
    if (m_texture)
    {
        delete m_texture;
        m_texture = 0;
    }
}

void ActorCloud::Draw()
{
    GenericActor::Draw();

    glEnable(GL_TEXTURE_2D);

    m_shader->Bind();
    m_texture->BindTexture();
    glDrawArrays(GL_QUADS, 0, 4);
    m_shader->UnBind();
    glDisable(GL_TEXTURE_2D);
}

void ActorCloud::SetNoise()
{
    GLfloat temp[34][34];
    int random = rand() % 5000;

    for (int y = 0; y < 33; y++)
    {
        for (int x = 0; x < 33; x++)
        {
            temp[x][y] = 128.0f + noise(x, y, random) * 128.0f;
        }
    }

    for (int x = 1; x < 33; x++)
    {
        temp[0][x] = temp[32][x];
        temp[33][x] = temp[1][x];
        temp[x][0] = temp[x][32];
        temp[x][33] = temp[x][1];
    }
    temp[0][0] = temp[32][32];
    temp[33][33] = temp[1][1];
    temp[0][33] = temp[32][1];
    temp[33][0] = temp[1][32];


    for (int y = 1; y < 33; y++)
    {
        for (int x = 1; x < 33; x++)
        {
            GLfloat center = temp[x][y] / 4.0f;
            GLfloat sides = (temp[x + 1][y] + temp[x - 1][y] + temp[x][y + 1] + temp[x][y - 1]) / 8.0f;
            GLfloat corners = (temp[x + 1][y + 1] + temp[x + 1][y - 1] + temp[x - 1][y + 1] + temp[x - 1][y - 1]) / 16.0f;
            m_map32[((x - 1) * 32) + (y - 1)] = center + sides + corners;
        }
    }
}

void ActorCloud::OverlapOctaves()
{
    for (int x = 0; x < 256 * 256; x++)
    {
        m_map256[x] = 0;
    }

    for (int octave = 0; octave < m_octave; octave++)
    {
        for (int x = 0; x < 256; x++)
        {
            for (int y = 0; y < 256; y++)
            {
                GLfloat scale = 1 / (GLfloat)pow(2, 3 - octave);
                GLfloat noise = Interpolate(x * scale, y * scale, m_map32);
                m_map256[(y * 256) + x] += noise / (GLfloat)pow(2, octave);
            }
        }
    }
}

GLfloat ActorCloud::Interpolate(GLfloat x, GLfloat y, GLfloat *map)
{
    int Xint = (int)x;
    int Yint = (int)y;

    GLfloat Xfrac = x - Xint;
    GLfloat Yfrac = y - Yint;

    int X0 = Xint % 32;
    int Y0 = Yint % 32;
    int X1 = (Xint + 1) % 32;
    int Y1 = (Yint + 1) % 32;

    GLfloat bot = map[X0 * 32 + Y0] + Xfrac * (map[X1 * 32 + Y0] - map[X0 * 32 + Y0]);
    GLfloat top = map[X0 * 32 + Y1] + Xfrac * (map[X1 * 32 + Y1] - map[X0 * 32 + Y1]);

    return (bot + Yfrac * (top - bot));
}

GLfloat ActorCloud::noise(int x, int y, int random)
{
    int n = x + y * 57 + random * 131;
    n = (n << 13) ^ 2;

    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) &
                    0x7fffffff)* 0.000000000931322574615478515625f);
}