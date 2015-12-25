#include "homework02.h"
#include "ActorSnowman.h"
#include "helpers.h"

ActorSnowman::ActorSnowman(double x, double y, double height) :
    GenericActor(x, y),
    m_height(height)
{
}

ActorSnowman::~ActorSnowman()
{
}

void ActorSnowman::Draw()
{
    double bottomRadius = m_height * 5 / 10 / 2;
    double middleRadius = m_height * 3 / 10 / 2;
    double topRadius = m_height * 2 / 10 / 2;
    double bottomY = m_y + bottomRadius;
    double middleY = bottomY + bottomRadius + middleRadius;
    double topY = middleY + middleRadius + topRadius;

    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(m_x, bottomY, bottomRadius);
    drawCircle(m_x, middleY, middleRadius * 1.1f);
    drawCircle(m_x, topY, topRadius * 1.1f);
}

void ActorSnowman::Animate()
{
    Draw();
}