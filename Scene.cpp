#include "homework02.h"
#include "Scene.h"
#include "ActorHouse.h"
#include "ActorSun.h"
#include "ActorSnowman.h"
#include "ActorCloud.h"
#include "ActorBackgroundMountain.h"

Scene::Scene(double width, double height) :
    GenericScene(width, height),
    m_sun(NULL)
{
    AddActor("sun", new ActorSun(-200.0f, m_sceneHeight - 100.0f, 100.0f));
    AddActor("cloud01", new ActorCloud(0, m_sceneHeight - 400, m_sceneWidth, 400));
    AddActor("cloud02", new ActorCloud(-100, m_sceneHeight - 500, m_sceneWidth + 100, 500));
    AddActor("background_mountain", new ActorBackgroundMountain(m_sceneWidth, m_sceneHeight - 200));
    AddActor("house", new ActorHouse(400, 0));
    AddActor("snowman", new ActorSnowman(100, 10));
}


Scene::~Scene()
{}

void Scene::Draw()
{
    RecalcBackground();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    GenericScene::Draw();

    GenericActor *sun = GetActor("sun");
    double x = sun->X();
    if (x < m_sceneWidth - 100)
    {
        sun->Move(0.01f, 0);
    }
}

GenericActor* Scene::GetActor(std::string name)
{
    if (name == "sun")
    {
        m_sun = m_sun != NULL ? m_sun : GenericScene::GetActor(name);
        return m_sun;
    }

    return GenericScene::GetActor(name);
}

void Scene::RecalcBackground()
{
    double sunX = GetActor("sun")->X();

    if ((sunX < 0) || (sunX > m_sceneWidth))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
        return;
    }

    double step = m_sceneWidth / 255;
    double blue = sunX / step / 255;

    glClearColor(0.0f, 0.0f, (float)blue, 1.0f);
}
