#include "homework02.h"
#include "Render/FrameBuffer.h"
#include "Render/TextureManager.h"
#include "Scene.h"
#include "Actors/ActorSun.h"
#include "Actors/ActorCloud.h"
#include "Actors/ActorBackgroundMountain.h"
#include "Actors/ActorSnowflake.h"
#include "Actors/ActorHouse.h"
#include "Actors/ActorCampfire.h"
#include "Actors/ActorAmbientLight.h"

Scene::Scene(GLuint width, GLuint height) :
    GenericScene(width, height),
    m_sun(NULL)
{
    AddActor("sun", new ActorSun(100.0f, 100.0f, -0.19f, 170.0f));
    AddActor("cloud01", new ActorCloud(0, 0, (GLfloat)m_sceneWidth, (GLfloat)m_sceneHeight / 1.5f, -0.2f));
    AddActor("background_mountain", new ActorBackgroundMountain((GLfloat)m_sceneWidth, (GLfloat)m_sceneHeight, -0.1f));
    AddActor("house", new ActorHouse(200, (GLfloat)m_sceneHeight, 0.0f, 300));
    AddActor("campfire", new ActorCampfire(500, (GLfloat)m_sceneHeight, 0.0f, 150));
    //AddActor("sunlight", new ActorAmbientLight(100.0f, 100.0f, -0.18f, 200, glm::vec3(1, 1, 0)));
    for (size_t i = 0; i < 50; i++)
    {
        std::string name = "snowflake" + i;
        AddActor(name, new ActorSnowflake(GLfloat(rand() % m_sceneWidth), GLfloat(rand() % m_sceneHeight / 2),
                                          GLfloat(15 + rand() % 20), m_sceneWidth, m_sceneHeight, 0.2f));
    }

    ResizeScene(m_sceneWidth, m_sceneHeight);
}


Scene::~Scene()
{
}

void Scene::Draw()
{
    static FrameBuffer *fb = FrameBuffer::GetInstance();
    fb->BindFrameBuffer();
    RecalcBackground();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GenericScene::Draw();
    fb->UnbindFrameBuffer();

    fb->Draw();
}

void Scene::Animate(GLint elapsedTime)
{
    GenericScene::Animate(elapsedTime);

    GenericActor *sun = GetActor("sun");
    double x = sun->X();
    if (x < m_sceneWidth - 100)
    {
        sun->Move(1.1f, 0);
    }
    else // In case of window resize
    {        
        sun->MoveTo((GLfloat)m_sceneWidth - 100, sun->Y());
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
    GLfloat sunX = GetActor("sun")->X();

    if ((sunX < 0) || (sunX > m_sceneWidth))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
        return;
    }

    GLfloat step = (GLfloat)m_sceneWidth / 255;
    GLfloat blue = sunX / step / 255;

    glClearColor(0.0f, 0.0f, blue, 1.0f);
}
