#pragma once

#include "GenericScene.h"

class GenericActor;

class Scene :
    public GenericScene
{
public:
    Scene(GLuint width, GLuint height);
    virtual ~Scene();

    virtual void Draw();
    virtual void Animate(GLint elapsedTime);

    virtual GenericActor* GetActor(std::string name);

    virtual std::string class_type() { return "Scene"; }

private:
    GenericActor *m_sun;

    void RecalcBackground();
};

