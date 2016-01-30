//
//  seShader.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "seShader.hpp"
#include "../Utils/seHelpers.hpp"
#include "../Debug/seDebug.hpp"
#include <iostream>
#include <fstream>
#include <streambuf>
#include <GL/glew.h>

namespace spriteEngine {
    seShader::seShader(const std::string &filename) :
        seResource(seResourceType::seRESOURCE_SHADER, filename)
    {
        seAssert(!m_resourceName.empty());

        try {
            LoadFromFile();
        } catch (const std::exception &e) {
            LogError << "Failed to load shader file: " << quoteStr(e.what()) << eol;
        }
    }

    seShader::~seShader() {
        glDeleteShader(m_ID);
    }

    void seShader::LoadFromFile() {
        std::ifstream fparser;
        fparser.open(m_resourceName, std::ios_base::in);

        if (!fparser)
            throw std::runtime_error(m_resourceName);

        std::string buffer((std::istreambuf_iterator<char>(fparser)), std::istreambuf_iterator<char>());
        buffer = seSHADER_VERSION + buffer;

        GLenum st = ends_with(m_resourceName, ".vs") ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
        m_ID = glCreateShader(st);
        const char *cstr = buffer.c_str();
        glShaderSource(m_ID, 1, &cstr, nullptr);

        glCompileShader(m_ID);

        GLint success = 0;
        GLchar error[1024] = { 0 };
        glGetShaderiv(m_ID, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            glGetShaderInfoLog(m_ID, sizeof(error), nullptr, error);
            LogError << "Shader compilation failed: " << error << eol;
        }
        else
            LogDebug << "Shader loaded: " << quoteStr(m_resourceName) << eol;
    }
}
