#include <iomanip>
#include "GenericScene.h"
#include "Actors/GenericActor.h"
#include "Render/FrameBuffer.h"
#include "Render/TextureManager.h"

GenericScene::~GenericScene()
{
    for (auto &it : m_actors)
    {
        delete it.second;
    }
    m_actors.clear();

    FrameBuffer::Destroy();
    TextureManager::Destroy();
}

void GenericScene::Draw()
{
    for (auto &it : m_actors)
    {
        it.second->Draw();
    }
}

void GenericScene::Animate(GLint elapsedTime)
{
    m_elapsedTime = elapsedTime;

    for (auto &it : m_actors)
    {
        it.second->Animate(m_elapsedTime);
    }
}

void GenericScene::AddActor(std::string name, GenericActor *actor)
{
    m_actors.insert(std::pair<std::string, GenericActor*>(name, actor));
    Debug("Actor ", name, " added. TYPE: ", actor->class_type());
}

GenericActor* GenericScene::GetActor(std::string name)
{
    actorsMap::iterator it = m_actors.find(name);
    assert(it != m_actors.end());

    return it->second;
}

void GenericScene::ResizeScene(GLsizei width, GLsizei height)
{
    m_sceneWidth = width;
    m_sceneHeight = height;

    for (auto &it : m_actors)
        it.second->ResizeScene(width, height);

    FrameBuffer::GetInstance()->ResizeScene(width, height);

    Draw();
}
