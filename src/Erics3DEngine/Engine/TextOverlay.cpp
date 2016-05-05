#include <string.h>
#include "TextOverlay.hpp"

void TextOverlay::init(int width, int height, const char * font) {
    mBufferWidth = width;
    mBufferHeight = height;

    Shader vs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/text.vs.glsl", Shader::ShaderType::VERTEX);
    vs.compile();
    Shader fs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/text.vs.glsl", Shader::ShaderType::FRAGMENT);
    fs.compile();
    std::vector<Shader> shaders;
    shaders.push_back(vs);
    shaders.push_back(fs);

    mTextProgram = new Program(shaders);
    mTextProgram->link();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenTextures(1, &mTextBuffer);
    glBindTexture(GL_TEXTURE_2D, mTextBuffer);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8UI, width, height);

    mFontTexture = new Texture(font ? std::string(font) : "/home/eric/ClionProjects/Erics3DEngine/src/Scenes/data/textures/cp437_9x16.ktx");

    mScreenBuffer = new char[width * height];
    memset(mScreenBuffer, 0, width * height);
}

void TextOverlay::cleanup() {
    delete [] mScreenBuffer;
    delete mFontTexture;
    glDeleteTextures(1, &mTextBuffer);
    glDeleteVertexArrays(1, &mVAO);
    delete mTextProgram;
}

void TextOverlay::render() {
    mTextProgram->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextBuffer);
    if (mDirty) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mBufferWidth, mBufferHeight, GL_RED_INTEGER, GL_UNSIGNED_BYTE, mScreenBuffer);
        mDirty = false;
    }
    mFontTexture->bind();

    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void TextOverlay::drawText(const char * str, int x, int y) {
    char * dst = mScreenBuffer + y * mBufferWidth + x;
    strcpy(dst, str);
    mDirty = true;
}

void TextOverlay::scroll(int lines) {
    const char * src = mScreenBuffer + lines * mBufferWidth;
    char * dst = mScreenBuffer;

    memmove(dst, src, (mBufferHeight - lines) * mBufferWidth);

    mDirty = true;
}

void TextOverlay::print(const char * str) {
    const char * p = str;
    char c;
    char * dst = mScreenBuffer + mCursorY * mBufferWidth + mCursorX;

    while (*p != 0) {
        c = *p++;
        if (c == '\n') {
            mCursorY++;
            mCursorX = 0;
            if (mCursorY >= mBufferHeight) {
                mCursorY--;
                scroll(1);
            }
            dst = mScreenBuffer + mCursorY * mBufferWidth + mCursorX;
        } else {
            *dst++ = c;
            mCursorX++;
            if (mCursorX >= mBufferWidth) {
                mCursorY++;
                mCursorX = 0;
                if (mCursorY >= mBufferHeight) {
                    mCursorY--;
                    scroll(1);
                }
                dst = mScreenBuffer + mCursorY * mBufferWidth + mCursorX;
            }
        }
    }
    mDirty = true;
}

void TextOverlay::moveCursor(int x, int y) {
    mCursorX = x;
    mCursorY = y;
}

void TextOverlay::clear() {
    memset(mScreenBuffer, 0, mBufferWidth * mBufferHeight);
    mDirty = true;
    mCursorX = 0;
    mCursorY = 0;
}