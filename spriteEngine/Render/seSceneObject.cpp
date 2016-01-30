//
//  seSceneObject.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 29.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "seSceneObject.hpp"
#include "seVBO.hpp"
#include "seVAO.hpp"
#include "seVertex.hpp"
#include "../Resources/seProgram.hpp"
#include "../Resources/seResourceManager.hpp"
#include "../Debug/seDebug.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace spriteEngine {
    seSceneObject::seSceneObject(bool centered, seProgram *program) :
        m_x(0), m_y(0), m_z(0),
        m_width(0), m_height(0),
        m_VBO(NULL), m_VAO(NULL),
        m_MVP(-1), m_model(glm::mat4(1.0f)), m_view(glm::mat4(1.0f)), m_projection(glm::mat4(1.0f)), m_MVPupdated(false),
        m_shaderProgram(program)
    {
        // If no shader program specified, when use "vs:basic,fs:basic" shader program
        if (!m_shaderProgram)
            m_shaderProgram = seResourceManager::GetInstance()->GetProgram("vs:basic,fs:basic");
        seAssert(m_shaderProgram);

        m_VAO = new seVAO(); seAssert(m_VAO);
        m_VBO = new seVBO(); seAssert(m_VBO);
        m_VBO->Bind(GL_ARRAY_BUFFER);
        m_shaderProgram->Bind();

        if (centered)
            m_VBO->AddData(&seQUAD_Centred_UV, sizeof(seQUAD_Centred_UV));
        else
            m_VBO->AddData(&seQUAD_UV, sizeof(seQUAD_Centred_UV));
        m_VBO->UploadDataToGPU(GL_STATIC_DRAW);

        m_VAO->Generate(m_shaderProgram->Attribute("inPosition"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
        m_VAO->Generate(m_shaderProgram->Attribute("inCoord"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));

        m_shaderProgram->SetUniform("gSampler", 0);
        m_MVP = m_shaderProgram->Uniform("MVP");
    }

    seSceneObject::~seSceneObject() {
        if (m_VBO) {
            delete m_VBO;
            m_VBO = NULL;
        }
        if (m_VAO) {
            delete m_VAO;
            m_VAO = NULL;
        }
    }

    void seSceneObject::Render() {
        Bind();
        if (!m_MVPupdated)
            UpdateMVP();
    }

    void seSceneObject::UpdateMVP() {
        if (m_MVP == -1) return;

        m_projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(m_x, m_y, m_z));
        m_model = glm::scale(glm::mat4(1.0f), glm::vec3(m_width, m_height, 1.0f));
        glm::mat4 matrixMVP = m_projection * m_view * m_model;
        m_shaderProgram->SetUniform(m_MVP, matrixMVP);

        m_MVPupdated = true;
    }

    void seSceneObject::Bind() {
        m_shaderProgram->Bind();
        m_VAO->Bind();
    }

    void seSceneObject::Unbind() {
        m_VAO->Unbind();
        m_shaderProgram->Unbind();
    }

    void seSceneObject::Move(GLfloat shiftX, GLfloat shiftY, GLfloat shiftZ) {
        MoveTo(m_x + shiftX, m_y + shiftY, m_z + shiftZ);
    }

    void seSceneObject::MoveTo(GLfloat x, GLfloat y, GLfloat z) {
        m_x = x;
        m_y = y;
        m_z = z;

        m_MVPupdated = false;
    }

    void seSceneObject::Resize(GLfloat deltaWidth, GLfloat deltaHeight) {
        SetSize(m_width + deltaWidth, m_height + deltaHeight);
    }

    void seSceneObject::SetSize(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height) {
        MoveTo(x, y, z);
        SetSize(width, height);
    }

    void seSceneObject::SetSize(GLfloat width, GLfloat height) {
        seAssert(width == height == 0);

        m_width = width;
        m_height = height;

        m_MVPupdated = false;
    }
}