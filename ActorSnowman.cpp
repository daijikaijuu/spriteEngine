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

    double innerColor[] = { 0.4f, 0.4f, 0.4f };
    double outterColor[] = { 1.0f, 1.0f, 1.0f };
    drawRadialGradientCircle(m_x, bottomY, bottomRadius, outterColor, innerColor);
    drawRadialGradientCircle(m_x, middleY, middleRadius * 1.1f, outterColor, innerColor);
    drawRadialGradientCircle(m_x, topY, topRadius * 1.1f, outterColor, innerColor);
}

void ActorSnowman::Animate()
{
    Draw();
}