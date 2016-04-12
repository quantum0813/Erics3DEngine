#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../ThirdParty/stb_image/stb_image.h"
#include <iostream>
#include "Debug.hpp"

// This define allows for more user-friendly error messages from stbi_image
#define STBI_FAILURE_USERMSG    1

Texture::Texture(std::string filename) {
    int width, height, numComponents;
    unsigned char * imageData = stbi_load(filename.c_str(), &width, &height, &numComponents, 4);

    if (imageData != nullptr) {
        glGenTextures(1, &mTexture);
        glBindTexture(GL_TEXTURE_2D, mTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

        stbi_image_free(imageData);
    } else {
        std::cout << "Error occurred while trying to read image: " << stbi_failure_reason() << std::endl;
    }
}

Texture::~Texture() {
    glDeleteTextures(1, &mTexture);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, mTexture);
}

/*Texture::Texture(GLenum target, std::string filename, bool hasAlpha) : mTarget(target), mFilename(filename) {
    Util::printFirstNDebugMessages(10);
    GLenum textureUnit = getNextUnusedTextureUnit();

    if (textureUnit != GL_INVALID_ENUM) {
        mTextureUnit = textureUnit;
        std::cout << "Reading image \"" << filename << "\"" << std::endl;

        int x, y, n;
        unsigned char * imageData = stbi_load(filename.c_str(), &x, &y, &n, hasAlpha ? STBI_rgb_alpha : STBI_rgb);

        if (imageData != nullptr) {
            glActiveTexture(mTextureUnit);
            Util::printFirstNDebugMessages(10);
            glGenTextures(1, &mTextureId);
            Util::printFirstNDebugMessages(10);
            glBindTexture(mTarget, mTextureId);
            Util::printFirstNDebugMessages(10);

            switch (mTarget) {
                case GL_TEXTURE_1D:
                    glTexImage1D(mTarget, 0, hasAlpha ? GL_RGBA : GL_RGB, x, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);
                    break;
                case GL_TEXTURE_2D:
                    glTexImage2D(mTarget, 0, hasAlpha ? GL_RGBA : GL_RGB, x, y, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);
                    break;
                default:
                    std::cout << "Texture target format not supported." << std::endl;
            }
            Util::printFirstNDebugMessages(10);

            // Use trilinear filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            Util::printFirstNDebugMessages(10);
            glGenerateMipmap(GL_TEXTURE_2D);
            Util::printFirstNDebugMessages(10);

            delete [] imageData;
        } else {
            std::cout << "Error occurred while trying to read image: " << stbi_failure_reason() << std::endl;
        }
    }
}

Texture::~Texture() {
    glDeleteTextures(1, &mTextureId);
}

GLuint Texture::getTextureId() {
    return mTextureId;
}

GLenum Texture::getTextureUnit() {
    return mTextureUnit;
}

void Texture::use() {
    glActiveTexture(mTextureUnit);
    glBindTexture(mTarget, mTextureId);
}

GLenum Texture::getNextUnusedTextureUnit() {
    auto iter = Texture::usedTextures.begin();
    bool foundUnusedUnit = false;
    for (; iter != Texture::usedTextures.end(); iter++) {
        if (!iter->second) {
            foundUnusedUnit = true;
            break;
        }
    }

    if (!foundUnusedUnit) {
        std::cout << "All available texture units used." << std::endl;
        return GL_INVALID_ENUM;
    }

    return iter->first;
}

std::map<GLenum, bool> const Texture::usedTextures = {
    {GL_TEXTURE0, false}, {GL_TEXTURE1, false}, {GL_TEXTURE2, false}, {GL_TEXTURE3, false},
    {GL_TEXTURE4, false}, {GL_TEXTURE5, false}, {GL_TEXTURE6, false}, {GL_TEXTURE7, false},
    {GL_TEXTURE8, false}, {GL_TEXTURE9, false}, {GL_TEXTURE10, false}, {GL_TEXTURE11, false},
    {GL_TEXTURE12, false}, {GL_TEXTURE13, false}, {GL_TEXTURE14, false}, {GL_TEXTURE15, false},
    {GL_TEXTURE16, false}, {GL_TEXTURE17, false}, {GL_TEXTURE18, false}, {GL_TEXTURE19, false},
    {GL_TEXTURE20, false}, {GL_TEXTURE21, false}, {GL_TEXTURE22, false}, {GL_TEXTURE23, false},
    {GL_TEXTURE24, false}, {GL_TEXTURE25, false}, {GL_TEXTURE26, false}, {GL_TEXTURE27, false},
    {GL_TEXTURE28, false}, {GL_TEXTURE29, false}, {GL_TEXTURE30, false}, {GL_TEXTURE31, false},
};*/