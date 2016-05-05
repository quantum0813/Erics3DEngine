#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../ThirdParty/stb_image/stb_image.h"
#include <iostream>
#include <algorithm>
#include "Debug.hpp"

// This define allows for more user-friendly error messages from stbi_image
#define STBI_FAILURE_USERMSG    1

Texture::Texture(std::string filename) {
    std::string fileExt = filename.substr(filename.find_last_of(".") + 1);
    std::transform(fileExt.begin(), fileExt.end(), fileExt.begin(), ::tolower);
    if (fileExt == "ktx") {
        loadKTX(filename);
    } else {
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
}

Texture::~Texture() {
    glDeleteTextures(1, &mTexture);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, mTexture);
}

unsigned int Texture::loadKTX(std::string filename, unsigned int texture) {
    FILE * fp;
    GLuint temp = 0;
    GLuint retVal = 0;
    KTXHeader h;
    size_t dataStart, dataEnd;
    unsigned char * data;
    GLenum target = GL_NONE;

    fp = fopen(filename.c_str(), "rb");

    if (!fp)
        return 0;

    if (fread(&h, sizeof(h), 1, fp) != 1)
        goto FailRead;

    if (memcmp(h.identifier, ktxIdentifier, sizeof(ktxIdentifier) != 0))
        goto FailHeader;

    if (h.endianness == 0x04030201) {
        // No swap needed
    } else if (h.endianness == 0x01020304) {
        // Swap needed
        h.endianness = swap32(h.endianness);
        h.glType = swap32(h.glType);
        h.glTypesize = swap32(h.glTypesize);
        h.glFormat = swap32(h.glFormat);
        h.glInternalFormat = swap32(h.glInternalFormat);
        h.glBaseInternalFormat = swap32(h.glBaseInternalFormat);
        h.pixelWidth = swap32(h.pixelWidth);
        h.pixelHeight = swap32(h.pixelHeight);
        h.pixelDepth = swap32(h.pixelDepth);
        h.arrayElements = swap32(h.arrayElements);
        h.faces = swap32(h.faces);
        h.mipLevels = swap32(h.mipLevels);
        h.keyPairBytes = swap32(h.keyPairBytes);
    } else {
        goto FailHeader;
    }

    // Guess target (texture type)
    if (h.pixelHeight == 0) {
        if (h.arrayElements == 0)
            target = GL_TEXTURE_1D;
        else
            target = GL_TEXTURE_1D_ARRAY;
    } else if (h.pixelDepth == 0) {
        if (h.arrayElements == 0) {
            if (h.faces == 0)
                target = GL_TEXTURE_2D;
            else
                target = GL_TEXTURE_CUBE_MAP;
        } else {
            if (h.faces == 0)
                target = GL_TEXTURE_2D_ARRAY;
            else
                target = GL_TEXTURE_CUBE_MAP_ARRAY;
        }
    } else {
        target = GL_TEXTURE_2D;
    }

    // Check for insanity...
    if (target == GL_NONE || (h.pixelWidth == 0) || (h.pixelHeight == 0 && h.pixelDepth != 0))
        goto FailHeader;

    temp = texture;
    if (texture == 0)
        glGenTextures(1, &texture);

    glBindTexture(target, texture);

    dataStart = ftell(fp) + h.keyPairBytes;
    fseek(fp, 0, SEEK_END);
    dataEnd = ftell(fp);
    fseek(fp, dataStart, SEEK_SET);

    data = new unsigned char[dataEnd - dataStart];
    memset(data, 0, dataEnd - dataStart);

    fread(data, 1, dataEnd - dataStart, fp);

    if (h.mipLevels == 0)
        h.mipLevels = 1;

    switch (target) {
        case GL_TEXTURE_1D:
            glTexStorage1D(GL_TEXTURE_1D, h.mipLevels, h.glInternalFormat, h.pixelWidth);
            glTexSubImage1D(GL_TEXTURE_1D, 0, 0, h.pixelWidth, h.glFormat, h.glInternalFormat, data);
            break;
        case GL_TEXTURE_2D:
            if (h.glType == GL_NONE) {
                glCompressedTexImage2D(GL_TEXTURE_2D, 0, h.glInternalFormat, h.pixelWidth, h.pixelHeight, 0, 420 * 380 / 2, data);
            } else {
                {
                    glTexStorage2D(GL_TEXTURE_2D, h.mipLevels, h.glInternalFormat, h.pixelWidth, h.pixelHeight);
                    unsigned char *ptr = data;
                    unsigned int height = h.pixelHeight;
                    unsigned int width = h.pixelWidth;
                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                    for (unsigned int i = 0; i < h.mipLevels; i++) {
                        glTexSubImage2D(GL_TEXTURE_2D, i, 0, 0, width, height, h.glFormat, h.glType, ptr);
                        ptr += height * calculateStride(h, width, 1);
                        height >>= 1;
                        width >>= 1;
                        if (!height)
                            height = 1;
                        if (!width)
                            width = 1;
                    }
                }
            }
            break;
        case GL_TEXTURE_3D:
            glTexStorage3D(GL_TEXTURE_3D, h.mipLevels, h.glInternalFormat, h.pixelWidth, h.pixelHeight, h.pixelDepth);
            glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, h.pixelWidth, h.pixelHeight, h.pixelDepth, h.glFormat, h.glType, data);
            break;
        case GL_TEXTURE_1D_ARRAY:
            glTexStorage2D(GL_TEXTURE_1D_ARRAY, h.mipLevels, h.glInternalFormat, h.pixelWidth, h.arrayElements);
            glTexSubImage2D(GL_TEXTURE_1D_ARRAY, 0, 0, 0, h.pixelWidth, h.arrayElements, h.glFormat, h.glType, data);
            break;
        case GL_TEXTURE_2D_ARRAY:
            glTexStorage3D(GL_TEXTURE_2D_ARRAY, h.mipLevels, h.glInternalFormat, h.pixelWidth, h.pixelHeight, h.arrayElements);
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, h.pixelWidth, h.pixelHeight, h.arrayElements, h.glFormat, h.glType, data);
            break;
        case GL_TEXTURE_CUBE_MAP:
            glTexStorage2D(GL_TEXTURE_CUBE_MAP, h.mipLevels, h.glInternalFormat, h.pixelWidth, h.pixelHeight);
            {
                unsigned int faceSize = calculateFaceSize(h);
                for (unsigned int i = 0; i < h.faces; i++)
                    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, h.pixelWidth, h.pixelHeight, h.glFormat, h.glType, data + faceSize * i);
            }
            break;
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, h.mipLevels, h.glInternalFormat, h.pixelWidth, h.pixelHeight, h.arrayElements);
            glTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, 0, 0, 0, h.pixelWidth, h.pixelHeight, h.faces * h.arrayElements, h.glFormat, h.glType, data);
            break;
        default:
            goto FailTarget;
    }

    if (h.mipLevels == 1)
        glGenerateMipmap(target);

    retVal = texture;
    mTexture = texture;

    FailTarget:
        delete [] data;
    FailHeader:;
    FailRead:;

    return retVal;
}