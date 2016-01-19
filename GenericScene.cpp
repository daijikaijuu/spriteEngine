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

void GenericScene::AddActor(string name, GenericActor *actor)
{
    m_actors.insert(std::pair<string, GenericActor*>(name, actor));
    Debug("Actor ", quoteStr(name), " added. TYPE: ", quoteStr(actor->class_type()));
}

GenericActor* GenericScene::GetActor(string name)
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

    DEBUG_OK();

    Draw();
}

bool GenericScene::DEBUG_OK() const
{
    HW_ASSERT(m_sceneWidth > 0);
    HW_ASSERT(m_sceneHeight > 0);

    return true;
}

string GenericScene::DEBUG_DUMP() const
{
    std::stringstream result;
    result << endl << " CLASS: " << GenericScene::class_type() << endl;
    result << DUMP_VAR(m_sceneWidth) << DUMP_VAR(m_sceneHeight) << endl;
    result << DUMP_VAR(m_actors.size()) << endl;
    result << DUMP_VAR(m_elapsedTime) << endl;
    return result.str();
}
