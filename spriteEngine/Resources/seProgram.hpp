//
//  seProgram.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seProgram_hpp
#define seProgram_hpp

#include "seResource.hpp"
#include <stdio.h>
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace spriteEngine {
    class seShader;

    class seProgram : public seResource {
        using IDsMap = std::map<std::string, GLuint>;

        seShader *m_VertexShader;
        seShader *m_FragmentShader;
        IDsMap m_Attributes;
        IDsMap m_Uniforms;

        void Link();
        void CheckProgramError(const std::string &message);

        bool InUse() const;

        void GetAttributes();
        void GetUniforms();
    public:
        seProgram(seShader *vertexShader, seShader *fragmentShader);
        virtual ~seProgram();

        virtual void Bind();
        virtual void Unbind();

        void SetShaders(seShader *vertexShader, seShader *fragmentShader);
        void SetShaders(const std::string &vertexShader, const std::string &fragmentShader);

        GLuint Attribute(const std::string &attrib);
        GLuint Uniform(const std::string &uniform);

#define _SEOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE) \
        void SetUniform(const std::string &uniformName, OGL_TYPE v0); \
        void SetUniform(GLuint uniform, OGL_TYPE v0);

        _SEOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLfloat);
        _SEOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLint);
        _SEOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLuint);

        void SetUniform(const std::string &uniformName, const glm::mat4 &m, GLboolean transpose = GL_FALSE);
        void SetUniform(GLuint uniform, const glm::mat4 &m, GLboolean transpose = GL_FALSE);

        static unsigned int sProgramCount;
    };
}

#endif /* seProgram_hpp */
