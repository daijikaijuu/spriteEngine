//
//  seSceneObject.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 29.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seSceneObject_hpp
#define seSceneObject_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace spriteEngine {
    class seVBO;
    class seVAO;
    class seProgram;

    class seGenericSceneObject {
    public:
        seGenericSceneObject(seProgram *shaderProgram);
        virtual ~seGenericSceneObject();

        inline GLfloat X() const { return m_x; }
        inline GLfloat Y() const { return m_y; }
        inline GLfloat Z() const { return m_z; }
        inline GLfloat Width() const { return m_width; }
        inline GLfloat Height() const { return m_height; }
        inline GLboolean IsMirrored() const { return m_mirrored; }
        seProgram* GetProgram() { return m_shaderProgram; }

        virtual void SetSize(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height) {}
        virtual void SetSize(GLfloat width, GLfloat height) {}
        virtual void Move(GLfloat shiftX, GLfloat shiftY, GLfloat shiftZ = 0.0f) {}
        virtual void MoveTo(GLfloat x, GLfloat y, GLfloat z = 0.0f) {}
        virtual void Resize(GLfloat deltaWidth, GLfloat deltaHeight) {}
        virtual void SetMirrored(GLboolean mirrored) {}
        virtual void Rotate(GLfloat angle) { m_angle += angle; m_MVPupdated = false; }
        virtual void ResetRotation() { m_angle = 0; }
        virtual void Animate() {}

        virtual void Render() {}
        virtual void UpdateMVP() {}

    protected:
        GLfloat m_x, m_y, m_z;
        GLfloat m_width, m_height;
        GLboolean m_mirrored;
        GLfloat m_angle;

        GLint m_MVP;
        glm::mat4 m_model;
        glm::mat4 m_view;
        glm::mat4 m_projection;
        bool m_MVPupdated;

        seVBO *m_VBO;
        seVAO *m_VAO;

        seProgram *m_shaderProgram;

    protected:
        virtual void Bind() {}
        virtual void Unbind() {}
    };

    class seSceneObject : public seGenericSceneObject {
    public:
        seSceneObject(bool centered, seProgram *shaderProgram);
        virtual ~seSceneObject();

        virtual void SetSize(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height);
        virtual void SetSize(GLfloat width, GLfloat height);
        virtual void Move(GLfloat shiftX, GLfloat shiftY, GLfloat shiftZ = 0.0f);
        virtual void MoveTo(GLfloat x, GLfloat y, GLfloat z = 0.0f);
        virtual void Resize(GLfloat deltaWidth, GLfloat deltaHeight);

        virtual void Render();
        virtual void UpdateMVP();

    protected:
        virtual void Bind();
        virtual void Unbind();
    };
}

#endif /* seSceneObject_hpp */
