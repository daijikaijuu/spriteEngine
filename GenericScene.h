#pragma once

#include <map>
#include <string>
#include <assert.h>
#include <GL/glew.h>
#include "Logger.h"

class GenericActor;

class GenericScene : public Logger
{
	using actorsMap = std::map<string, GenericActor*>;
public:
	GenericScene(GLsizei width, GLsizei height) :
        Logger(),
		m_sceneWidth(width),
		m_sceneHeight(height),
		m_actors(),
        m_elapsedTime(0)
	{}

    virtual ~GenericScene();

    virtual void Draw();
    virtual void Animate(GLint elapsedTime);

    void AddActor(string name, GenericActor *actor);
    virtual GenericActor* GetActor(string name);

    void ResizeScene(GLsizei width, GLsizei height);

    virtual string class_type() const { return "GenericScene"; }
    virtual bool DEBUG_OK() const;
    virtual string DEBUG_DUMP() const;

protected:
    GLuint    m_sceneWidth, m_sceneHeight;
	actorsMap m_actors;
    GLint     m_elapsedTime;
};
