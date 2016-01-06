#pragma once

#include <map>
#include <string>
#include <assert.h>
#include <GL\glew.h>

class GenericActor;

class GenericScene
{
	using actorsMap = std::map<std::string, GenericActor*>;
public:
	GenericScene(GLsizei width, GLsizei height) :
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

protected:
    GLuint    m_sceneWidth, m_sceneHeight;
	actorsMap m_actors;
    GLint m_elapsedTime;
};