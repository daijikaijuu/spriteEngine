//
//  gameLevel.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 31.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "seGameLevel.hpp"
#include "seVAO.hpp"
#include "seVBO.hpp"
#include "seVertex.hpp"
#include "../Resources/Resources.hpp"
#include "../Debug/Debug.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace spriteEngine {
    seGameLevel::seGameLevel(seProgram *shaderProgram, seTexture *tileSet) :
        seGenericSceneObject(shaderProgram),
        m_tileSet(tileSet)
    {
        seAssert(m_tileSet);

        int levelMap[6][11] = {
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            3,  5,  7,  0,  0,  0,  0,  0,  0,  0,  0,
           14,  6,  2,  8, 16,  0,  0,  0,  0,  0,  0,
            2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        };

        m_VBO->Bind(GL_ARRAY_BUFFER);
        m_shaderProgram->Bind();

        for (int y = 0; y < 6; y++) {
            for (int x = 0; x < 11; x++) {
                GLfloat step = 800.0f / 11;
                GLfloat uvStepX = 1.0f / 10.0f;
                GLfloat uvStepY = 1.0f / 2.0f;
                unsigned int item = levelMap[y][x];
                GLfloat itemX = item % 11;
                GLfloat itemY = 1 + (item / 11) % 2;
                seVertexUV vertexData[] = {
                    glm::vec3(step * x,        step * y + step, 0.0f), glm::vec2(uvStepX * itemX,           uvStepY * itemY),
                    glm::vec3(step * x + step, step * y + step, 0.0f), glm::vec2(uvStepX * itemX + uvStepX, uvStepY * itemY),
                    glm::vec3(step * x,        step * y,        0.0f), glm::vec2(uvStepX * itemX,           uvStepY * itemY + uvStepY),
                    glm::vec3(step * x + step, step * y,        0.0f), glm::vec2(uvStepX * itemX + uvStepX, uvStepY * itemY + uvStepY),
                };
//                seVertexUV vertexData[] = {
//                    glm::vec3(step * x,        step * y + step, 0.0f), glm::vec2(0, 0),
//                    glm::vec3(step * x + step, step * y + step, 0.0f), glm::vec2(1.0f /  10, 0),
//                    glm::vec3(step * x,        step * y,        0.0f), glm::vec2(0, 1.0f /2),
//                    glm::vec3(step * x + step, step * y,        0.0f), glm::vec2(1.0f / 10, 1.0f / 2),
//                };
                m_VBO->AddData(&vertexData, sizeof(vertexData));
            }
        }

        m_VBO->UploadDataToGPU(GL_STATIC_DRAW);

        m_VAO->Generate(m_shaderProgram->Attribute("inPosition"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
        m_VAO->Generate(m_shaderProgram->Attribute("inCoord"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));

        m_shaderProgram->SetUniform("gSampler", 0);
        m_MVP = m_shaderProgram->Uniform("MVP");

        m_width = 1800;
        m_height = 1600;

        m_projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(m_x, m_y, m_z));
        m_model = glm::mat4(1.0f);//glm::scale(glm::mat4(1.0f), glm::vec3(m_width, m_height, 1.0f));
        glm::mat4 matrixMVP = m_projection * m_view * m_model;
        m_shaderProgram->SetUniform(m_MVP, matrixMVP);

        m_MVPupdated = true;
    }

    seGameLevel::~seGameLevel() {
    }

    void seGameLevel::Render() {
        m_shaderProgram->Bind();
        m_tileSet->Bind();
        m_VAO->Bind();

        for (int i = 0; i < 6 * 11; i++)
            glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);

        m_VAO->Unbind();
        m_tileSet->Unbind();
        m_shaderProgram->Unbind();
    }
}
