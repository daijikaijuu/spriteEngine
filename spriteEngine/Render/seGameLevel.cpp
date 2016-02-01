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
#include <vector>

namespace spriteEngine {
    seGameLevel::seGameLevel(seProgram *shaderProgram, seTexture *tileSet) :
        seGenericSceneObject(shaderProgram),
        m_tileSet(tileSet),
        m_tileSize(0),
        m_indexBuffer(0)
    {
        seAssert(m_tileSet);

        glGenBuffers(1, &m_indexBuffer);

        m_width = 11;
        m_height = 8;
        std::vector<uint> levelMap = {
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  3,  4,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
           14, 15, 15, 15, 15, 16, 18, 14, 15, 15, 16,
        };

        m_VBO->Bind(GL_ARRAY_BUFFER);
        m_shaderProgram->Bind();

        m_tileSize = 600.0f / m_height;
        GLfloat uvStepX = 1.0f / 10.0f;
        GLfloat uvStepY = 1.0f / 2.0f;
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                unsigned int item = levelMap[x + m_width * y];
                GLfloat itemX = item % 11;
                GLfloat itemY = 1 - (item / 11) % 2;
                seVertexUV vertexData[] = {
                    glm::vec3(m_tileSize * x,              m_tileSize * y + m_tileSize, 0.0f), glm::vec2(uvStepX * itemX,           uvStepY * itemY),
                    glm::vec3(m_tileSize * x + m_tileSize, m_tileSize * y + m_tileSize, 0.0f), glm::vec2(uvStepX * itemX + uvStepX, uvStepY * itemY),
                    glm::vec3(m_tileSize * x,              m_tileSize * y,              0.0f), glm::vec2(uvStepX * itemX,           uvStepY * itemY + uvStepY),

                    glm::vec3(m_tileSize * x,              m_tileSize * y,              0.0f), glm::vec2(uvStepX * itemX,           uvStepY * itemY + uvStepY),
                    glm::vec3(m_tileSize * x + m_tileSize, m_tileSize * y + m_tileSize, 0.0f), glm::vec2(uvStepX * itemX + uvStepX, uvStepY * itemY),
                    glm::vec3(m_tileSize * x + m_tileSize, m_tileSize * y,              0.0f), glm::vec2(uvStepX * itemX + uvStepX, uvStepY * itemY + uvStepY),

                };
                m_VBO->AddData(&vertexData, sizeof(vertexData));
            }
        }
        m_VBO->UploadDataToGPU(GL_STATIC_DRAW);

        m_VAO->Generate(m_shaderProgram->Attribute("inPosition"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
        m_VAO->Generate(m_shaderProgram->Attribute("inCoord"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));

        m_shaderProgram->SetUniform("gSampler", 0);
        m_MVP = m_shaderProgram->Uniform("MVP");

        m_projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
        UpdateMVP();
    }

    seGameLevel::~seGameLevel() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &m_indexBuffer);
    }

    void seGameLevel::Render() {
        seGenericSceneObject::Render();

        m_shaderProgram->Bind();
        m_tileSet->Bind();
        m_VAO->Bind();

//        glDrawArrays(GL_TRIANGLES, 0, m_width * m_height * 6);

        std::vector<GLshort> indices;
        for (int i = 0; i < 2 * m_width * m_height; i++) {
            float x = (i % (uint)m_width);
            float y = (i / (uint)m_width) % (uint)m_height;
            if (!(((x + 1) * m_tileSize + m_x < 0) || ((x - 1) * m_tileSize + m_x + m_tileSize > 800))) {
                for (int ic = 0; ic < 6; ic++) {
                    indices.push_back(i * 6 + ic);
                }
                //LogDebug << "Tile drawn x:" << x << " y:" << y << eol;
            }
            else
                LogDebug << "Tile out of screen x:" << x << " y:" << y << eol;

        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), indices.data(), GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_SHORT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


        m_VAO->Unbind();
        m_tileSet->Unbind();
        m_shaderProgram->Unbind();
    }

    void seGameLevel::UpdateMVP() {
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(m_x, m_y, m_z));
        seGenericSceneObject::UpdateMVP();
    }
}
