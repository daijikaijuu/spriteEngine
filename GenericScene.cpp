#include "GenericScene.h"
#include "GenericActor.h"

GenericScene::~GenericScene()
{
    for (auto &it : m_actors)
    {
        delete it.second;
    }
    m_actors.clear();
    m_actorsIndex.clear();
}

void GenericScene::Draw()
{
    for (auto &it : m_actorsIndex)
    {
        GenericActor *actor = m_actors[it];
        actor->Animate();
    }
}

void GenericScene::AddActor(std::string name, GenericActor *actor)
{
    m_actors.insert(std::pair<std::string, GenericActor*>(name, actor));
    m_actorsIndex.push_back(name);
}

GenericActor* GenericScene::GetActor(std::string name)
{
    actorsMap::iterator it = m_actors.find(name);
    assert(it != m_actors.end());

    return it->second;
}

void GenericScene::ResizeScene(double width, double height)
{
    m_sceneWidth = width;
    m_sceneHeight = height;

    Draw();
}
