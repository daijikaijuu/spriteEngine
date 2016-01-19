#pragma once
#include <GL/glew.h>
#include <string>
#include <map>
#include <initializer_list>
#include "../Logger.h"

#define NUM_SHADER_TYPES 3
#define SHADER_VERSION "#version 330\n"

class Shader : public Logger
{
public:
    Shader();
    virtual ~Shader();

    void Load(const string &fileName);
    void Load(const string &vertexFilename, const string &fragmentFilename);
    void Load(const string &vertex, const string &fragment, const string &geometry);

    void RegisterAttribute(string attrib);
    void RegisterAttribute(std::initializer_list<string> a_args);
    void RegisterUniform(string uniform);
    void RegisterUniform(std::initializer_list<string> a_args);
    GLuint GetProgramID() const;
    GLuint GetAttributeLocation(string attrib);
    GLuint GetUniformLocation(string uniform);

    void Bind();
    void UnBind();

    Shader& operator=(const Shader &shader) = delete;
    Shader(const Shader &shader) = delete;

    virtual string class_type() const { return "Shader"; }
    virtual bool DEBUG_OK() const;
    virtual string DEBUG_DUMP() const;

private:
    enum ShaderType
    {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        GEOMETRY_SHADER
    };

    GLuint m_program;
    GLuint m_shaders[NUM_SHADER_TYPES];
    std::map<string, GLuint> m_attribList;
    std::map<string, GLuint> m_uniforms;

    string loadFromFile(ShaderType type, const string &fileName);
    GLuint loadFromText(ShaderType type, string text);
    void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, LogType messageType, const string &msg);
};


