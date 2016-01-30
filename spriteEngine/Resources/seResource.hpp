//
//  seResource.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seResource_hpp
#define seResource_hpp

#include "../Utils/seEnums.hpp"
#include <stdio.h>
#include <string>
#include <GL/glew.h>

namespace spriteEngine {
    class seResource {
    public:
        seResource(seResourceType resourceType, const std::string &resourceName) :
            m_resourceType(resourceType),
            m_resourceName(resourceName),
            m_ID(0)
        {
            if (m_resourceType == seResourceType::seRESOURCE_SHADER)
                m_resourceName = "Data/Shaders/" + m_resourceName;
            if (m_resourceType == seResourceType::seRESOURCE_TEXTURE)
                m_resourceName = "Data/Textures/"+ m_resourceName;
        }
        virtual ~seResource() { Unbind(); }

        virtual void Bind(GLuint param) {}
        virtual void Unbind() {}

        GLuint ID() const { return m_ID; }
        const std::string& Name() const { return m_resourceName; }
        seResourceType Type() const { return m_resourceType; }

        seResource& operator=(const seResource &resource) = delete;
        seResource(const seResource &resource) = delete;

    protected:
        seResourceType m_resourceType;
        std::string m_resourceName;
        GLuint m_ID;

        virtual void LoadFromFile() {}
    };
}

#endif /* seResource_hpp */
