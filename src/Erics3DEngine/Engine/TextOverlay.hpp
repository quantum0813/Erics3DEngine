#ifndef ERICS3DENGINE_TEXTOVERLAY_HPP
#define ERICS3DENGINE_TEXTOVERLAY_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

#include "../../Erics3DEngine/Engine/Texture.hpp"
#include "../../Erics3DEngine/Engine/Shader.hpp"
#include "../../Erics3DEngine/Engine/Program.hpp"

class TextOverlay {
    public:
        TextOverlay() : mCursorX(0), mCursorY(0) {}

        void init(int width, int height, const char * font = nullptr);
        void cleanup();
        void render();

        void drawText(const char * str, int x, int y);
        void print(const char * str);
        void scroll(int lines);
        void moveCursor(int x, int y);
        void clear();

    private:
        GLuint mTextBuffer;
        Texture * mFontTexture;
        GLuint mVAO;

        Program * mTextProgram;
        char * mScreenBuffer;
        int mBufferWidth;
        int mBufferHeight;
        bool mDirty;
        int mCursorX;
        int mCursorY;
};

#endif //ERICS3DENGINE_TEXTOVERLAY_HPP
