#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <iostream>
#include <string>
#include <map>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Exception.hpp"
#include "Scene.hpp"

class Application {
    public:
        Application(int windowWidth, int windowHeight, std::string title, bool enableLogging = true);
        ~Application();
        void setGLVersion(int major, int minor);
        void initialize();
        void setScene(Scene * scene);
        void run();
        GLFWwindow * getWindow();

    private:
        int mGLMajor;
        int mGLMinor;
        int mWindowWidth;
        int mWindowHeight;
        std::string mWindowTitle;
        bool mLoggingEnabled;

        bool isRunning();

        GLFWwindow * mWindow;
        Scene * mCurrentScene;

        static void errorCallback(int error, const char * desc);

        static const int DEFAULT_CONTEXT_VERSION_MAJOR = 3;
        static const int DEFAULT_CONTEXT_VERSION_MINOR = 3;
};

#endif //APPLICATION_HPP