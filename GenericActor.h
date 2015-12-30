#pragma once

#include <GL\glew.h>
#include "Shader.h"

class GenericActor
{
public:
	GenericActor(GLfloat x, GLfloat y, GLfloat z = 0.0f, bool animated = true) :
		m_x(x),
		m_y(y),
        m_z(z),
		m_animated(animated),
        m_VBO(0),
        m_VAO(0),
        m_shader(NULL)
    {
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
    }

	virtual ~GenericActor()
    {
        if (m_shader)
        {
            delete m_shader;
            m_shader = NULL;
        }
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    }

    virtual void Draw()
    {
        glBindVertexArray(m_VAO);
    }
    virtual void Animate() { Draw(); }

	virtual void Move(GLfloat xShift, GLfloat yShift)
	{
		m_x += xShift;
		m_y += yShift;
	}

    GLfloat X() { return m_x; }
    GLfloat Y() { return m_y; }
    GLfloat Z() { return m_z; }

protected:
    GLfloat m_x, m_y, m_z;
	bool   m_animated;

    GLuint m_VBO;
    GLuint m_VAO;
    Shader *m_shader;
};
