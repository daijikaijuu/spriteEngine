//
//  seTexture.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "seTexture.hpp"
#include "../Utils/seHelpers.hpp"
#include "../Debug/seDebug.hpp"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <png.h>


namespace spriteEngine {
    seTexture::seTexture(const std::string &resourceName, bool useEmptyTexture) :
        seResource(seResourceType::seRESOURCE_TEXTURE, resourceName),
        m_Sampler(0)
    {
        seAssert(!resourceName.empty());

        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);
        glGenSamplers(1, &m_Sampler);

        try {
            LoadFromFile(useEmptyTexture);
        } catch (const seException &e) {
            LogError << "Failed to load texture: " << quoteStr(resourceName) << eol << "\t" << e.what() << eol;
        }
    }

    seTexture::~seTexture() {
        glDeleteSamplers(1, &m_Sampler);
        glDeleteTextures(1, &m_ID);
    }

    void seTexture::Bind(GLuint textureUnit) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, m_ID);
        glBindSampler(textureUnit, m_Sampler);
    }

    void seTexture::Unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void seTexture::LoadFromFile(bool fallbackToEmptyTexture) {
        png_structp ptrPNG;
        png_infop ptrInfo;
        unsigned int sig_read = 0;
        int color_type, interlace_type;

        FILE *fp;
        if ((fp = fopen(m_resourceName.c_str(), "rb")) == nullptr) {
            if (fallbackToEmptyTexture)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            else
                throw seException("Can't open texture file. Empty texture intended?");
            return;
        }

        ptrPNG = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (ptrPNG == nullptr)
        {
            fclose(fp);
            throw seException("png_create_read_struct failed");
        }

        ptrInfo = png_create_info_struct(ptrPNG);
        if (ptrInfo == nullptr)
        {
            fclose(fp);
            png_destroy_read_struct(&ptrPNG, nullptr, nullptr);
            throw seException("png_create_info_struct failed");
        }

        if (setjmp(png_jmpbuf(ptrPNG)))
        {
            png_destroy_read_struct(&ptrPNG, nullptr, nullptr);
            fclose(fp);
            throw seException("setjmp(png_jmpbuf(ptrPNG)) failed");
        }

        png_init_io(ptrPNG, fp);
        png_set_sig_bytes(ptrPNG, sig_read);

        png_read_png(ptrPNG, ptrInfo, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, nullptr);

        png_uint_32 width, height;
        int bit_depth;
        png_get_IHDR(ptrPNG, ptrInfo, &width, &height, &bit_depth, &color_type, &interlace_type, nullptr, nullptr);

        unsigned int row_bytes = (unsigned int)png_get_rowbytes(ptrPNG, ptrInfo);
        row_bytes += 3 - ((row_bytes - 1) % 4);

        png_byte *outData = (png_byte *)malloc(row_bytes * height * sizeof(png_byte) + 15);
        if (outData == nullptr)
        {
            png_destroy_read_struct(&ptrPNG, &ptrInfo, nullptr);
            fclose(fp);
            throw seException("Could not allocate memory for PNG image data");
        }
        png_bytepp row_pointers = png_get_rows(ptrPNG, ptrInfo);

        for (size_t i = 0; i < height; i++)
        {
            memcpy(outData + (row_bytes * (height - 1 - i)), row_pointers[i], row_bytes);
        }

        png_destroy_read_struct(&ptrPNG, &ptrInfo, nullptr);

        fclose(fp);

        GLint format;
        switch (color_type)
        {
            case PNG_COLOR_TYPE_RGB:
                format = GL_RGB;
                break;
            case PNG_COLOR_TYPE_RGB_ALPHA:
                format = GL_RGBA;
                break;
            default:
                throw seException("Unknown libpng color type");
        }

        LogDebug << "Texture loaded: " << quoteStr(m_resourceName) << eol;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, outData);

        free(outData);

        glSamplerParameteri(m_Sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(m_Sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
}
