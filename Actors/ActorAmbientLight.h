#pragma once

#include "GenericActor.h"
#include "../homework02.h"

class ActorAmbientLight : public TexturedActor
{
public:
    ActorAmbientLight(GLfloat x, GLfloat y, GLfloat z, GLfloat size, glm::vec3 color);
    virtual ~ActorAmbientLight();

    virtual void Draw();
    virtual void ResizeScene(GLsizei width, GLsizei height);

protected:
    virtual void BindShaderAttributesAndUniforms();

private:
    glm::vec3 m_color;
};
