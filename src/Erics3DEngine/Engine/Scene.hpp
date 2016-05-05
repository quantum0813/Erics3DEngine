#ifndef SCENE_HPP
#define SCENE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Scene {
    public:
        Scene(GLFWwindow * window) : mWindow(window) {}
        ~Scene() {};
        virtual void initScene() = 0;
        virtual void render() = 0;
        virtual void cleanup() = 0;

        virtual void onKeyPress(GLFWwindow * window, int key, int scanCode, int action, int mode) = 0;
        virtual void onMouseMove(GLFWwindow * window, double xPos, double yPos) = 0;
        virtual void onMouseWheelScroll(GLFWwindow * window, double xOffset, double yOffset) = 0;

    private:
        GLFWwindow * mWindow;
};

#endif //SCENE_HPP