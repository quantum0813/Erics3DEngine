#ifndef ERICS3DENGINE_TESTSCENE_HPP
#define ERICS3DENGINE_TESTSCENE_HPP

#include "../Erics3DEngine/Engine/Scene.hpp"
#include "../Erics3DEngine/Engine/Shader.hpp"
#include "../Erics3DEngine/Engine/Program.hpp"

class RainbowTriangleScene : public Scene {
    public:
        RainbowTriangleScene() : Scene() {}
        ~RainbowTriangleScene();
        void initScene();
        void render();
        void cleanup();

    private:
        GLuint mVAO;
        GLuint mVBO;

        GLint mPosAttrib;
        GLint mColAttrib;

        Program * mDefaultProgram;

        const float VERTICES[15] = {
            0.0f,  0.5f, 1.0f, 0.0f, 0.0f, // Vertex 1: Red
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2: Green
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Vertex 3: Blue
        };
};

#endif //ERICS3DENGINE_TESTSCENE_HPP
