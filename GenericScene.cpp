#include "GenericScene.h"
#include "GenericActor.h"
#include "helpers.h"

GenericScene::~GenericScene()
{
    for (auto &it : m_actors)
    {
        delete it.second;
    }
    m_actors.clear();
}

void GenericScene::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    for (auto &it : m_actors)
    {
        it.second->Animate();
    }
}

void GenericScene::AddActor(std::string name, GenericActor *actor)
{
    m_actors.insert(std::pair<std::string, GenericActor*>(name, actor));
}

GenericActor* GenericScene::GetActor(std::string name)
{
    actorsMap::iterator it = m_actors.find(name);
    assert(it != m_actors.end());

    return it->second;
}

void GenericScene::ResizeScene(GLuint width, GLuint height)
{
    m_sceneWidth = width;
    m_sceneHeight = height;

    Draw();
}
