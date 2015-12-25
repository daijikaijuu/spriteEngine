#pragma once

#include <map>
#include <vector>
#include <string>
#include <assert.h>

class GenericActor;

class GenericScene
{
	using actorsMap = std::map<std::string, GenericActor*>;
public:
	GenericScene(double width, double height) :
		m_sceneWidth(width),
		m_sceneHeight(height),
		m_actors(),
		m_actorsIndex()
	{}

    virtual ~GenericScene();

    virtual void Draw();

    void AddActor(std::string name, GenericActor *actor);
    virtual GenericActor* GetActor(std::string name);

    void ResizeScene(double width, double height);

protected:
	double    m_sceneWidth, m_sceneHeight;
	actorsMap m_actors;
	std::vector<std::string> m_actorsIndex;
};