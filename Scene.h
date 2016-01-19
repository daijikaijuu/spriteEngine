#pragma once

#include "GenericScene.h"

class GenericActor;

class Scene : public GenericScene
{
public:
    Scene(GLuint width, GLuint height, bool useFramebuffer = true);
    virtual ~Scene();

    virtual void Draw();
    virtual void Animate(GLint elapsedTime);

    virtual GenericActor* GetActor(string name);

    virtual string class_type() const { return "Scene"; }
    virtual string DEBUG_DUMP() const;

private:
    GenericActor *m_sun;
    bool m_UseFramebuffer;

    void RecalcBackground();
};

