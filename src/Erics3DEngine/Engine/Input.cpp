#include "Input.hpp"

Scene * Input::scene;

void Input::initialize(Scene * sc) {
    scene = sc;

    glfwSetKeyCallback(glfwGetCurrentContext(), keyboardCallback);
    glfwSetCursorPosCallback(glfwGetCurrentContext(), mousePosCallback);
    glfwSetScrollCallback(glfwGetCurrentContext(), scrollCallback);
}

void Input::keyboardCallback(GLFWwindow * window, int key, int scanCode, int action, int mode) {
    scene->onKeyPress(window, key, scanCode, action, mode);
}

void Input::mousePosCallback(GLFWwindow * window, double xPos, double yPos) {
    scene->onMouseMove(window, xPos, yPos);
}

void Input::scrollCallback(GLFWwindow * window, double xOffset, double yOffset) {
    scene->onMouseWheelScroll(window, xOffset, yOffset);
}