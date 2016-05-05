#ifndef ERICS3DENGINE_GRASSSCENE_HPP
#define ERICS3DENGINE_GRASSSCENE_HPP

#include "../Erics3DEngine/Engine/Scene.hpp"
#include "../Erics3DEngine/Engine/Shader.hpp"
#include "../Erics3DEngine/Engine/Program.hpp"
#include "../Erics3DEngine/Engine/Texture.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class GrassScene : public Scene {
    public:
        GrassScene(GLFWwindow * window) : Scene(window) {}
        ~GrassScene();

        void initScene();
        void render();
        void cleanup();

        void onKeyPress(GLFWwindow * window, int key, int scanCode, int action, int mode);
        void onMouseMove(GLFWwindow * window, double xPos, double yPos);
        void onMouseWheelScroll(GLFWwindow * window, double xOffset, double yOffset);

    private:
        GLuint mGrassBuffer;
        GLuint mGrassVAO;

        Program * mGrassProgram;
        Texture * mTexGrassColor;
        Texture * mTexGrassLength;
        Texture * mTexGrassOrientation;
        Texture * mTexGrassBend;

        GLint mUniformMVPMatrix;
};

#endif //ERICS3DENGINE_GRASSSCENE_HPP