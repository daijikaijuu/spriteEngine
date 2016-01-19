#pragma once

#include "../homework02.h"
#include "../helpers.h"
#include "../Actors/GenericActor.h"

class Texture;
class VertexArrayObject;
class Shader;

class FrameBuffer : public Singleton<FrameBuffer>, public TexturedActor
{
    friend Singleton<FrameBuffer>;
public:
    FrameBuffer();
    virtual ~FrameBuffer();

    virtual void ResizeScene(GLsizei width, GLsizei height);

    bool BindFrameBuffer();
    void UnbindFrameBuffer();
    virtual void Draw();

    Texture* GetTexture() const;

    virtual string class_type() const { return "FrameBuffer"; }
    virtual bool DEBUG_OK() const;
    virtual string DEBUG_DUMP() const;

protected:
    virtual void BindShaderAttributesAndUniforms();

private:
    GLuint m_ID, m_depthBuffer;

    void AttachTexture();
};

