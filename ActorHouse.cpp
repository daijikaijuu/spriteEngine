#include "homework02.h"
#include "ActorHouse.h"

ActorHouse::ActorHouse(double x, double y, double size) :
    GenericActor(x, y),
    m_size(size)
{
}

ActorHouse::~ActorHouse()
{
}

void ActorHouse::Draw()
{
    double hw = m_size / 2;
    double hh = m_size * 2 / 3;

    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.6f, 0.65f);
    glVertex2d(m_x - hw, m_y);
    glVertex2d(m_x - hw, m_y + hh);
    glVertex2d(m_x + hw, m_y + hh);
    glVertex2d(m_x + hw, m_y);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.9f, 0.0f, 0.0f);
    glVertex2d(m_x - hw, m_y + hh);
    glVertex2d(m_x, m_y + m_size);
    glVertex2d(m_x + hw, m_y + hh);
    glEnd();
}

void ActorHouse::Animate()
{
    Draw();
}
