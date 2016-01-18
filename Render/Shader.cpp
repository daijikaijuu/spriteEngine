#include "Shader.h"
#include <iostream>
#include <fstream>
#include <streambuf>

const char* VERTEX_FILETYPE = ".vs";
const char* FRAGMENT_FILETYPE = ".fs";
const char* GEOMETRY_FILETYPE = ".gs";

Shader::Shader() :
    Logger(),
    m_program(0),
    m_attribList(),
    m_uniforms()
{
    m_program = glCreateProgram();
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
    vertex = loadFromFile(VERTEX_SHADER, fileName + VERTEX_FILETYPE);
    fragment = loadFromFile(FRAGMENT_SHADER, fileName + FRAGMENT_FILETYPE);
    geometry = loadFromFile(GEOMETRY_SHADER, fileName + GEOMETRY_FILETYPE);

    Load(vertex, fragment, geometry);
}

void Shader::Load(const std::string & vertexFilename, const std::string & fragmentFilename)
{
    Load(loadFromFile(VERTEX_SHADER, vertexFilename + VERTEX_FILETYPE),
         loadFromFile(FRAGMENT_SHADER, fragmentFilename + FRAGMENT_FILETYPE),
         "");
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
    CheckShaderError(m_program, GL_LINK_STATUS, true, LogType::LOGTYPE_ERROR, "LNK shader program");

    glValidateProgram(m_program);
    CheckShaderError(m_program, GL_LINK_STATUS, true, LogType::LOGTYPE_ERROR, "INVALID shader program");
}

GLuint Shader::GetProgramID() const
{
    return m_program;
}

GLuint Shader::GetAttributeLocation(std::string attrib)
{
    return m_attribList[attrib];
}

GLuint Shader::GetUniformLocation(std::string uniform)
{
    return m_uniforms[uniform];
}

void Shader::RegisterAttribute(std::string attrib)
{
    m_attribList[attrib] = glGetAttribLocation(m_program, attrib.c_str());
}

void Shader::RegisterAttribute(std::initializer_list<std::string> a_args)
{
    for (auto i : a_args)
    {
        RegisterAttribute(i);
    }
}

void Shader::RegisterUniform(std::string uniform)
{
    m_uniforms[uniform] = glGetUniformLocation(m_program, uniform.c_str());
}

void Shader::RegisterUniform(std::initializer_list<std::string> a_args)
{
    for (auto i : a_args)
    {
        RegisterUniform(i);
    }
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
    else
    {
        Error("Failed to open shader file: ", quoteStr(fileName));
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

    text = std::string(SHADER_VERSION) + text;

    GLuint shader = glCreateShader(st);
    const char *cstr = text.c_str();
    glShaderSource(shader, 1, &cstr, nullptr);

    glCompileShader(shader);
    CheckShaderError(shader, GL_COMPILE_STATUS, false, LogType::LOGTYPE_ERROR, "Shader compilation failed");

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

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, LogType messageType, const std::string & msg)
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

        Log(messageType, msg, ": ", error);
    }
}
