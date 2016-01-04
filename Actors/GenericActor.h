#pragma once

#include <GL/glew.h>
#include "../Render/Shader.h"
#include "../Render/VertexArrayObject.h"
#include "../Render/VertexBufferObject.h"

class GenericActor
{
public:
	GenericActor(GLfloat x, GLfloat y, GLfloat size, GLfloat z = 0.0f) :
		m_x(x),
		m_y(y),
        m_z(z),
        m_size(size),
        m_shader(NULL),
        m_VAO(NULL),
        m_elapsedTime(0)
    {
        m_VAO = new VertexArrayObject();
        m_shader = new Shader();
    }

	virtual ~GenericActor()
    {
        if (m_shader)
        {
            delete m_shader;
            m_shader = NULL;
        }

        if (m_VAO)
        {
            delete m_VAO;
            m_VAO = NULL;
        }
    }

    virtual void Draw()
    {
        m_VAO->Bind();
    }
    virtual void Animate(GLint elapsedTime)
    {
        m_elapsedTime = elapsedTime;
    }

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
    GLfloat m_size;
    GLint m_elapsedTime;

    Shader *m_shader;
    VertexArrayObject *m_VAO;
};
