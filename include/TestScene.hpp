#ifndef ERICS3DENGINE_TESTSCENE_HPP
#define ERICS3DENGINE_TESTSCENE_HPP

#include "Scene.hpp"
#include "Shader.hpp"
#include "Program.hpp"

class TestScene : public Scene {
    public:
        TestScene() : Scene() {}
        ~TestScene();
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
