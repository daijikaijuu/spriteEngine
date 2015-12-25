#pragma once

#include "GenericScene.h"

class GenericActor;

class Scene :
    public GenericScene
{
public:
    Scene(double width, double height);
    virtual ~Scene();

    virtual void Draw();

    virtual GenericActor* GetActor(std::string name);
private:
    GenericActor *m_sun;

    void RecalcBackground();
};

