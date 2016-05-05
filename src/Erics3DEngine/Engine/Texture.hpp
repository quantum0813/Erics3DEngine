#ifndef ERICS3DENGINE_TEXTURE_HPP
#define ERICS3DENGINE_TEXTURE_HPP

#include <string>
#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>

class Texture {
    public:
        Texture(std::string filename);
        ~Texture();
        void bind();

    private:
        unsigned int loadKTX(std::string filename, unsigned int texture = 0);
        GLuint mTexture;
};

struct KTXHeader {
    unsigned char identifier[12];
    unsigned int endianness;
    unsigned int glType;
    unsigned int glTypesize;
    unsigned int glFormat;
    unsigned int glInternalFormat;
    unsigned int glBaseInternalFormat;
    unsigned int pixelWidth;
    unsigned int pixelHeight;
    unsigned int pixelDepth;
    unsigned int arrayElements;
    unsigned int faces;
    unsigned int mipLevels;
    unsigned int keyPairBytes;
};

union KeyPairValue {
    unsigned int size;
    unsigned char rawBytes[4];
};

static const unsigned char ktxIdentifier[] = {
    0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A
};

static const unsigned int swap32(const unsigned int u32) {
    union {
        unsigned int u32;
        unsigned char u8[4];
    } a, b;

    a.u32 = u32;
    b.u8[0] = a.u8[3];
    b.u8[1] = a.u8[2];
    b.u8[2] = a.u8[1];
    b.u8[3] = a.u8[0];

    return b.u32;
}

static unsigned int calculateStride(const KTXHeader & h, unsigned int width, unsigned int pad = 4) {
    unsigned int channels = 0;

    switch (h.glBaseInternalFormat) {
        case GL_RED:
            channels = 1;
            break;
        case GL_RG:
            channels = 2;
            break;
        case GL_BGR:
        case GL_RGB:
            channels = 3;
            break;
        case GL_BGRA:
        case GL_RGBA:
            channels = 4;
            break;
    }

    unsigned int stride = h.glTypesize * channels * width;

    stride = (stride + (pad - 1)) & ~(pad - 1);

    return stride;
}

static unsigned int calculateFaceSize(const KTXHeader & h) {
    unsigned int stride = calculateStride(h, h.pixelWidth);

    return stride * h.pixelHeight;
}

#endif //ERICS3DENGINE_TEXTURE_HPP