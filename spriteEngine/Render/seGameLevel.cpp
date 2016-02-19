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
#include <tinyxml2.h>

namespace spriteEngine {
    seGameLevel::seGameLevel(seProgram *shaderProgram, const std::string &resourceName) :
        seGenericSceneObject(shaderProgram),
        seResource(seResourceType::seRESOURCE_GAME_LEVEL, resourceName),
        m_tileSet(nullptr),
        m_tileSize(0),
        m_indexBuffer(0),
        m_layers()
    {
        glGenBuffers(1, &m_indexBuffer);

        tinyxml2::XMLDocument doc(true, tinyxml2::COLLAPSE_WHITESPACE);
        LogDebug << "seGameLevel: Opening game level: " << quoteStr(m_resourceName) << eol;
        doc.LoadFile(m_resourceName.c_str());
        if (doc.Error()) {
            LogError << "seGameLevel::XMLDocument.LoadFile(" << m_resourceName << "): " << doc.ErrorName() << eol;
            abort();
        }

        // Load map information
        const char *levelName = doc.FirstChildElement("map")->FirstChildElement("layer")->Attribute("name");
        m_width = doc.FirstChildElement("map")->FirstChildElement("layer")->UnsignedAttribute("width");
        m_height = doc.FirstChildElement("map")->FirstChildElement("layer")->UnsignedAttribute("height");
        LogDebug << "seGameLevel: Map name: " << quoteStr(levelName) << ". Width: " << m_width << ", height: " << m_height << eol;

        // Load tileset information
        std::string tilesetName = doc.FirstChildElement("map")->FirstChildElement("tileset")->FirstChildElement("image")->Attribute("source");
        LogDebug << "seGameLevel::Using tileset: " << quoteStr(tilesetName) << eol;
        tilesetName = ReplaceString(tilesetName, "../Textures/", "");
        m_tileSet = seRManager->AddTexture(tilesetName);
        m_tileSet->SetFiltering();
        GLuint tilesetColumns = doc.FirstChildElement("map")->FirstChildElement("tileset")->UnsignedAttribute("columns");
        GLuint tilesetRows = doc.FirstChildElement("map")->FirstChildElement("tileset")->UnsignedAttribute("tilecount") / tilesetColumns;
        m_tileSize = 600.0f / m_height; // TODO: Unhardcore scene height

        // Load collision information
        tinyxml2::XMLElement *element = doc.FirstChildElement("map")->FirstChildElement("tileset")->FirstChildElement("tile");
        std::map<GLuint, seCollisionRect *> tileBounds;
        while (element) {
            GLuint id = element->UnsignedAttribute("id");
            GLfloat x = element->FirstChildElement()->FirstChildElement()->FloatAttribute("x");
            GLfloat y = element->FirstChildElement()->FirstChildElement()->FloatAttribute("y");
            GLfloat width = element->FirstChildElement()->FirstChildElement()->FloatAttribute("width");
            GLfloat height = element->FirstChildElement()->FirstChildElement()->FloatAttribute("height");
            LogDebug << "seGameLevel::Tile " << id << ". Collision rect - x: " << x << ", y: " << y << ", width: " << width << ", height: " << height  << eol;

            tileBounds.insert(std::pair<GLuint, seCollisionRect *>(id, new seCollisionRect(x, y, width, height)));

            element = element->NextSiblingElement("tile");
        }

        m_VBO->Bind(GL_ARRAY_BUFFER);
        m_shaderProgram->Bind();

        // Load tilemap layers
        GLuint idx = 0;
        element = doc.FirstChildElement("map")->FirstChildElement("layer");
        while (element) {
            ParseLayer(&idx, tilesetColumns, tilesetRows, element, tileBounds);
            
            element = element->NextSiblingElement("layer");
        }

        m_VBO->UploadDataToGPU(GL_STATIC_DRAW);

        m_VAO->Generate(m_shaderProgram->Attribute("inPosition"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
        m_VAO->Generate(m_shaderProgram->Attribute("inCoord"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));

        m_shaderProgram->SetUniform("gSampler", 0);
        m_MVP = m_shaderProgram->Uniform("MVP");

        m_projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
        UpdateMVP();

        for (auto &it : tileBounds) {
            delete it.second;
        }
        tileBounds.clear();

        LogDebug << "seGameLevel::Loaded: " << quoteStr(m_resourceName) << eol;
    }

    seGameLevel::~seGameLevel() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &m_indexBuffer);

        for (auto &it : m_layers) {
            delete it.second;
        }
        m_layers.clear();
    }

    void seGameLevel::Render() {
        seGenericSceneObject::Render();

        m_shaderProgram->Bind();
        m_tileSet->Bind();
        m_VAO->Bind();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
        for (auto &it : m_layers) {
            std::vector<GLshort> indices;
            for (int i = 0; i < 2 * m_width * m_height; i++) {
                float x = (i % (GLuint)m_width);
                float y = (i / (GLuint)m_width) % (GLuint)m_height;
                seTile *tile =  it.second->tiles[y * m_width + x];

                if (!(((x + 1) * m_tileSize + m_x < 0) || ((x - 1) * m_tileSize + m_x + m_tileSize > 800))) {
                    for (int ic = 0; ic < 6; ic++) {
                        indices.push_back(ic + tile->indexShift);
                    }
                    tile->onScreen = true;
//                  LogDebug << "Tile drawn x:" << x << " y:" << y << eol;
                }
                else {
//                  LogDebug << "Tile out of screen x:" << x << " y:" << y << eol;
                    tile->onScreen = false;
                }
            }
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), indices.data(), GL_STATIC_DRAW);
            glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_SHORT, 0);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        m_VAO->Unbind();
        m_tileSet->Unbind();
        m_shaderProgram->Unbind();
    }

    void seGameLevel::UpdateMVP() {
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(m_x, m_y, m_z));
        seGenericSceneObject::UpdateMVP();
    }

    GLboolean seGameLevel::Collision(seCollisionRect rect, seCollisionDirection direction) const {
        GLuint x1 = (rect.x - m_x) / m_tileSize;
        GLuint x2 = (rect.Right() - m_x) / m_tileSize;
        GLuint y1 = (rect.y - m_y) / m_tileSize;
        GLuint y2 = (rect.Bottom() - m_y) / m_tileSize;

        for (auto &it : m_layers) {
            if (direction == seCollisionDirection::seCOLLISION_UP ||
                direction == seCollisionDirection::seCOLLISION_DOWN)
            {
                for (GLuint x = x1; x <= x2; x++) {
                    GLuint id = x + m_width * (direction == seCollisionDirection::seCOLLISION_UP ? y1 : y2);
                    try {
                        seTile *tile = it.second->tiles.at(id);
                        if (!tile)
                            return true;
                        seCollisionRect r = tile->rect.Shift(m_x, m_y);
                        //                    LogDebug << "UD_Tile: " << tile->id << ": " << r.x << ", " << r.y << ", " << r.Right() << ", " << r.Bottom() << eol;
                        //                    LogDebug << "UD_Hero: " << rect.x << ", " << rect.y << ", " << rect.Right() << ", " << rect.Bottom() << eol;
                        //                    LogDebug << "Intersects: " << r.Intersects(rect) << eol;
                        if (tile->collidable && r.Intersects(rect))
                            return true;
                    } catch (std::out_of_range &e) {
                        LogError << "seGameLevel::Collision Index (" << id << ") out of bounds"<< eol;
                        return true;
                    }
                }
            }
            //        LogDebug << "----" << eol;
            if (direction == seCollisionDirection::seCOLLISION_LEFT ||
                direction == seCollisionDirection::seCOLLISION_RIGHT)
            {
                for (GLuint y = y1; y <= y2; y++) {
                    GLuint id = (direction == seCollisionDirection::seCOLLISION_LEFT ? x1 : x2) + m_width * y;
                    try {
                        seTile *tile = it.second->tiles.at(id);
                        if (!tile)
                            return true;
                        seCollisionRect r = tile->rect.Shift(m_x, m_y);
                        //                LogDebug << "UD_Tile: " << r.x << ", " << r.y << ", " << r.Right() << ", " << r.Bottom() << eol;
                        //                LogDebug << "UD_Hero: " << rect.x << ", " << rect.y << ", " << rect.Right() << ", " << rect.Bottom() << eol;
                        //                LogDebug << "Intersects: " << r.Intersects(rect) << eol;
                        if (tile->collidable && r.Intersects(rect))
                            return true;
                    } catch (std::out_of_range &e) {
                        LogError << "seGameLevel::Collision Index (" << id << ") out of bounds"<< eol;
                        return true;
                    }
                }
            }
        }

        return false;
    }

    void seGameLevel::ParseLayer(GLuint *index, GLuint tilesetColumns, GLuint tilesetRows, tinyxml2::XMLElement *element,
                                 std::map<GLuint, seCollisionRect *> &tileBounds)
    {
        GLfloat uvStepX = 1.0f / tilesetColumns;
        GLfloat uvStepY = 1.0f / tilesetRows;
        GLfloat zOrder = Z() + m_layers.size() / 100.0f;

        std::string layerName(element->Attribute("name"));
        GLuint layerWidth = element->UnsignedAttribute("width");
        GLuint layerHeight = element->UnsignedAttribute("height");
        m_layers[layerName] = new seTileLayer(layerWidth, layerHeight);

        std::string data = element->FirstChildElement("data")->GetText();
        std::stringstream ss(data);
        int item = 0;
        int count = 0;
        while (ss >> item) {
            item--;

            GLuint x = count % layerWidth;
            GLuint y = (count / layerWidth) % layerHeight;
            GLuint itemX = item % tilesetColumns;
            GLuint itemY = tilesetRows - (item / tilesetColumns) % tilesetRows;
            seVertexUV vertexData[] = {
                glm::vec3(m_tileSize * x,              m_tileSize * y + m_tileSize, zOrder), glm::vec2(uvStepX * itemX,           uvStepY * itemY - uvStepY),
                glm::vec3(m_tileSize * x + m_tileSize, m_tileSize * y + m_tileSize, zOrder), glm::vec2(uvStepX * itemX + uvStepX, uvStepY * itemY - uvStepY),
                glm::vec3(m_tileSize * x,              m_tileSize * y,              zOrder), glm::vec2(uvStepX * itemX,           uvStepY * itemY),

                glm::vec3(m_tileSize * x,              m_tileSize * y,              zOrder), glm::vec2(uvStepX * itemX,           uvStepY * itemY),
                glm::vec3(m_tileSize * x + m_tileSize, m_tileSize * y + m_tileSize, zOrder), glm::vec2(uvStepX * itemX + uvStepX, uvStepY * itemY - uvStepY),
                glm::vec3(m_tileSize * x + m_tileSize, m_tileSize * y,              zOrder), glm::vec2(uvStepX * itemX + uvStepX, uvStepY * itemY),
            };

            m_VBO->AddData(&vertexData, sizeof(vertexData));

            seCollisionRect *rect = tileBounds[item];
            m_layers[layerName]->tiles.push_back(new seTile(item,
                                                            (GLuint)x, (GLuint)y,
                                                            seCollisionRect(m_tileSize * x + (rect != nullptr ? rect->x : 0),
                                                                            m_tileSize * y + (rect != nullptr ? rect->y : 0),
                                                                            rect != nullptr ? rect->width : m_tileSize,
                                                                            rect != nullptr ? rect->height : m_tileSize),
                                                            rect != nullptr,
                                                            false,
                                                            *index));

            if (ss.peek() == ',' || ss.peek() == ' ')
                ss.ignore();
            count++;
            *index += 6;
        }

        LogDebug << "seGameLevel::Map layer " << quoteStr(layerName) << " added. Width: " << layerWidth << ", height: " << layerHeight << eol;
        zOrder += 0.01f;
    }
}
