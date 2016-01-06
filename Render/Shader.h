#pragma once
#include <GL/glew.h>
#include <string>
#include <map>
#include <initializer_list>

#define NUM_SHADER_TYPES 3
#define SHADER_VERSION "#version 130\n"

class Shader
{
public:
    Shader();
    virtual ~Shader();

    void Load(const std::string &fileName);
    void Load(const std::string &vertex, const std::string &fragment, const std::string &geometry);

    void RegisterAttribute(std::string attrib);
    void RegisterAttribute(std::initializer_list<std::string> a_args);
    void RegisterUniform(std::string uniform);
    void RegisterUniform(std::initializer_list<std::string> a_args);
    GLuint GetProgramID() const;
    GLuint GetAttributeLocation(std::string attrib);
    GLuint GetUniformLocation(std::string uniform);

    void Bind();
    void UnBind();

    Shader& operator=(const Shader &shader) = delete;
    Shader(const Shader &shader) = delete;

private:
    enum ShaderType
    {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        GEOMETRY_SHADER
    };

    GLuint m_program;
    GLuint m_shaders[NUM_SHADER_TYPES];
    std::map<std::string, GLuint> m_attribList;
    std::map<std::string, GLuint> m_uniforms;

    std::string loadFromFile(ShaderType type, const std::string &fileName);
    GLuint loadFromText(ShaderType type, std::string text);
    void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const char* msg);
};


