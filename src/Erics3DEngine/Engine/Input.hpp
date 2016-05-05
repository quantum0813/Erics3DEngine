#ifndef ERICS3DENGINE_INPUT_HPP
#define ERICS3DENGINE_INPUT_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Scene.hpp"

class Input {
    public:
        static void initialize(Scene * sc);

    private:
        static Scene * scene;

        static void keyboardCallback(GLFWwindow * window, int key, int scanCode, int action, int mode);
        static void mousePosCallback(GLFWwindow * window, double xPos, double yPos);
        static void scrollCallback(GLFWwindow * window, double xOffset, double yOffset);
};

#endif //ERICS3DENGINE_INPUT_HPP
