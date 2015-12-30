#include "Shader.h"
#include <iostream>
#include <fstream>
#include <streambuf>

Shader::Shader() :
    m_program(0)
{
    m_program = glCreateProgram();
    m_attribList.clear();
    m_uniforms.clear();
    for (size_t i = 0; i < NUM_SHADER_TYPES; i++)
    {
        m_shaders[i] = 0;
    }
}

Shader::~Shader()
{
    for (size_t i = 0; i < NUM_SHADER_TYPES; i++)
    {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    glDeleteProgram(m_program);
}

void Shader::Load(const std::string &fileName)
{
    std::string vertex, fragment, geometry;
    vertex = loadFromFile(VERTEX_SHADER, fileName + ".vert.glsl");
    fragment = loadFromFile(FRAGMENT_SHADER, fileName + ".frag.glsl");
    geometry = loadFromFile(GEOMETRY_SHADER, fileName + ".geom.glsl");

    Load(vertex, fragment, geometry);
}

void Shader::Load(const std::string & vertex, const std::string & fragment, const std::string & geometry)
{
    if (!vertex.empty())
        m_shaders[VERTEX_SHADER] = loadFromText(VERTEX_SHADER, vertex);
    if (!fragment.empty())
        m_shaders[FRAGMENT_SHADER] = loadFromText(FRAGMENT_SHADER, fragment);
    if (!geometry.empty())
        m_shaders[GEOMETRY_SHADER] = loadFromText(GEOMETRY_SHADER, geometry);

    if (m_shaders[VERTEX_SHADER] != 0)
        glAttachShader(m_program, m_shaders[VERTEX_SHADER]);
    if (m_shaders[FRAGMENT_SHADER] != 0)
        glAttachShader(m_program, m_shaders[FRAGMENT_SHADER]);
    if (m_shaders[GEOMETRY_SHADER] != 0)
        glAttachShader(m_program, m_shaders[GEOMETRY_SHADER]);

    glLinkProgram(m_program);
    CheckShaderError(m_program, GL_LINK_STATUS, true, "ERROR LNK shader program");

    glValidateProgram(m_program);
    CheckShaderError(m_program, GL_LINK_STATUS, true, "ERROR INV shader program");
}

GLuint Shader::GetProgramID() const
{
    return m_program;
}

GLuint Shader::GetAttributeLocation(const char *attrib)
{
    return m_attribList[attrib];
}

GLuint Shader::GetUniformLocation(const char *uniform)
{
    return m_uniforms[uniform];
}

void Shader::RegisterAttribute(const char *attrib)
{
    m_attribList[attrib] = glGetAttribLocation(m_program, attrib);
}

void Shader::RegisterUniform(const char *uniform)
{
    m_uniforms[uniform] = glGetUniformLocation(m_program, uniform);
}

std::string Shader::loadFromFile(ShaderType type, const std::string &fileName)
{
    std::ifstream fparser;
    fparser.open(fileName, std::ios_base::in);
    if (fparser)
    {
        std::string buffer((std::istreambuf_iterator<char>(fparser)),
                           std::istreambuf_iterator<char>());
        return buffer;
    }

    return "";
}

GLuint Shader::loadFromText(ShaderType type, std::string text)
{
    GLenum st;
    switch (type)
    {
    case Shader::VERTEX_SHADER:
        st = GL_VERTEX_SHADER;
        break;
    case Shader::FRAGMENT_SHADER:
        st = GL_FRAGMENT_SHADER;
        break;
    case Shader::GEOMETRY_SHADER:
        st = GL_GEOMETRY_SHADER;
        break;
    default:
        break;
    }

    GLuint shader = glCreateShader(st);
    const char *cstr = text.c_str();
    glShaderSource(shader, 1, &cstr, nullptr);

    glCompileShader(shader);
    CheckShaderError(shader, GL_COMPILE_STATUS, false, "ERROR : Shader compilation failed");

    return shader;
}

void Shader::Bind()
{
    glUseProgram(m_program);
}

void Shader::UnBind()
{
    glUseProgram(0);
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const char* msg)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if (isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if (success == GL_FALSE)
    {
        if (isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cout << msg << " : " << error << " " << std::endl;
    }
}
