#pragma once

#include <map>
#include <string>
#include <assert.h>
#include <GL/glew.h>
#include "Logger.h"

class GenericActor;

class GenericScene : public Logger
{
	using actorsMap = std::map<std::string, GenericActor*>;
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

    void AddActor(std::string name, GenericActor *actor);
    virtual GenericActor* GetActor(std::string name);

    void ResizeScene(GLsizei width, GLsizei height);

    virtual std::string class_type() { return "GenericScene"; }

protected:
    GLuint    m_sceneWidth, m_sceneHeight;
	actorsMap m_actors;
    GLint     m_elapsedTime;
};
