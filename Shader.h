#pragma once
#include <GL/glew.h>
#include <string>
#include <map>

#define NUM_SHADER_TYPES 3

class Shader
{
public:
    Shader(const std::string &fileName);
    Shader(const std::string &vertex,
           const std::string &fragment,
           const std::string &geometry);
    virtual ~Shader();

    void RegisterAttribute(const char *attrib);
    void RegisterUniform(const char *uniform);
    GLuint GetProgramID() const;
    GLuint GetAttributeLocation(const char *attrib);
    GLuint GetUniformLocation(const char *uniform);

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

    void Init(const std::string &vertex,
              const std::string &fragment,
              const std::string &geometry);
};


