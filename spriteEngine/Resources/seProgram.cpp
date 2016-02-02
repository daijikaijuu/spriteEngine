//
//  seProgram.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "../Debug/Debug.hpp"
#include "../Utils/seHelpers.hpp"
#include "../Resources/Resources.hpp"
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace spriteEngine {
    unsigned int seProgram::sProgramCount = 0;

    seProgram::seProgram(seShader *vertexShader, seShader *fragmentShader) :
        seResource(seResourceType::seRESOURCE_SHADER_PROGRAM, std::string("shaderProgram:" + to_string(sProgramCount))),
        m_VertexShader(vertexShader),
        m_FragmentShader(fragmentShader),
        m_Attributes(),
        m_Uniforms()
    {
        m_ID = glCreateProgram();
        if (!m_ID)
            throw std::runtime_error("glCreateProgram failed");

        Link();
        sProgramCount++;
    }

    seProgram::~seProgram() {
        m_Attributes.clear();
        m_Uniforms.clear();

        if (m_ID != 0)
            glDeleteProgram(m_ID);
    }

    void seProgram::Bind() {
        seAssert(m_ID);
        if (!InUse())
            glUseProgram(m_ID);
    }

    void seProgram::Unbind() {
        glUseProgram(0);
    }

    void seProgram::SetShaders(seShader *vertexShader, seShader *fragmentShader) {
        seAssert(vertexShader && fragmentShader);

        m_VertexShader = vertexShader;
        m_FragmentShader = fragmentShader;

        Link();
    }

    void seProgram::SetShaders(const std::string &vertexShader, const std::string &fragmentShader) {
        seAssert(!vertexShader.empty());
        seAssert(!fragmentShader.empty());

        SetShaders(seRManager->GetShader(vertexShader), seRManager->GetShader(fragmentShader));
    }

    void seProgram::Link() {
        m_Attributes.clear();
        m_Uniforms.clear();

        seAssert(m_ID);
        seAssert(m_VertexShader && m_FragmentShader);

        glAttachShader(m_ID, m_VertexShader->ID());
        glAttachShader(m_ID, m_FragmentShader->ID());

        glLinkProgram(m_ID);
        CheckProgramError("LNK shader program");

        glValidateProgram(m_ID);
        CheckProgramError("INVALID shader program");

        GetAttributes();
        GetUniforms();

        glDetachShader(m_ID, m_VertexShader->ID());
        glDetachShader(m_ID, m_FragmentShader->ID());
    }

    void seProgram::CheckProgramError(const std::string &message) {
        GLint success = 0;
        GLchar error[1024] = { 0 };
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

        if (success == GL_FALSE) {
            glGetProgramInfoLog(m_ID, sizeof(error), nullptr, error);
            LogError << message << ": " << error << eol;
        }
    }

    bool seProgram::InUse() const {
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        return (currentProgram == (GLuint)m_ID);
    }

    GLuint seProgram::Attribute(const std::string &attrib) {
        seAssert(!attrib.empty());

        IDsMap::iterator it = m_Attributes.find(attrib);
        if (it == m_Attributes.end()) {
            GLuint result = glGetAttribLocation(m_ID, attrib.c_str());
            m_Attributes[attrib] = result;
            return result;
        }
        else
            return it->second;
    }

    GLuint seProgram::Uniform(const std::string &uniform) {
        seAssert(!uniform.empty());

        IDsMap::iterator it = m_Uniforms.find(uniform);
        if (it == m_Uniforms.end()) {
            GLuint result = glGetUniformLocation(m_ID, uniform.c_str());
            m_Uniforms[uniform] = result;
            return result;
        }
        else
            return it->second;
    }

    void seProgram::GetAttributes() {
        seAssert(m_ID);

        GLint numActiveAttribs = 0;
        glGetProgramiv(m_ID, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);

        GLint maxAttribNameLength = 0;
        glGetProgramiv(m_ID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);
        std::vector<GLchar> nameData(maxAttribNameLength);
        for(int attrib = 0; attrib < numActiveAttribs; attrib++)
        {
            GLint arraySize = 0;
            GLenum type = 0;
            GLsizei actualLength = 0;
            glGetActiveAttrib(m_ID, attrib, (GLsizei)nameData.size(), &actualLength, &arraySize, &type, &nameData[0]);
            std::string name((char *)&nameData[0], actualLength);

            seAssert(!name.empty());

            m_Attributes[name] = glGetAttribLocation(m_ID, name.c_str());
        }
    }

    void seProgram::GetUniforms() {
        seAssert(m_ID);

        GLint numActiveUniforms = 0;
        glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

        std::vector<GLchar> nameData(256);
        for (int unif = 0; unif < numActiveUniforms; unif++) {
            GLint arraySize = 0;
            GLenum type = 0;
            GLsizei actualLength = 0;
            glGetActiveUniform(m_ID, unif, (GLsizei)nameData.size(), &actualLength, &arraySize, &type, &nameData[0]);
            std::string name((char *)&nameData[0], actualLength);

            seAssert(!name.empty());

            m_Uniforms[name] = glGetUniformLocation(m_ID, name.c_str());
        }
    }

#define ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE, TYPE_PREFIX, TYPE_SUFFIX) \
    void seProgram::SetUniform(const std::string &name, OGL_TYPE v0) \
        { SetUniform(Uniform(name), v0); } \
    void seProgram::SetUniform(GLuint uniform, OGL_TYPE v0) \
        { Bind(); glUniform1 ## TYPE_SUFFIX (uniform, v0); }

    ATTRIB_N_UNIFORM_SETTERS(GLfloat, , f);
    ATTRIB_N_UNIFORM_SETTERS(GLint, I, i);
    ATTRIB_N_UNIFORM_SETTERS(GLuint, I, ui);

    void seProgram::SetUniform(const std::string &uniformName, const glm::mat4 &m, GLboolean transpose) {
        SetUniform(Uniform(uniformName), m, transpose);
    }

    void seProgram::SetUniform(GLuint uniform, const glm::mat4 &m, GLboolean transpose) {
        Bind();
        glUniformMatrix4fv(uniform, 1, transpose, glm::value_ptr(m));
    }
}
