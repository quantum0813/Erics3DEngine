#include <GL/glew.h>
#include "Application.hpp"
#include "Texture.hpp"

Application::Application(int windowWidth, int windowHeight, std::string title, bool enableLogging) :
    mWindowWidth(windowWidth), mWindowHeight(windowHeight), mWindowTitle(title), mLoggingEnabled(enableLogging) { }

Application::~Application() {
    glfwTerminate();
    delete mWindow;
}

void Application::initialize() {
    if (mLoggingEnabled)
        std::cout << "Beginning initialization of GLFW" << std::endl;

    // Set the error callback function for GLFW
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        if (mLoggingEnabled)
            std::cout << "Error occurred while trying to initialize GLFW" << std::endl;
        throw Exception("Error occurred while trying to initialize GLFW");
    }

    if (mLoggingEnabled)
        std::cout << "GLFW successfully initialized" << std::endl;

    if (mGLMajor == 0)
        mGLMajor = DEFAULT_CONTEXT_VERSION_MAJOR;
    if (mGLMinor == 0)
        mGLMinor = DEFAULT_CONTEXT_VERSION_MINOR;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, mGLMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, mGLMinor);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    if (mLoggingEnabled)
        std::cout << "Creating GLFW window" << std::endl;

    // Create our window object
    mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, mWindowTitle.c_str(), nullptr, nullptr);

    if (mWindow == nullptr) {
        if (mLoggingEnabled)
            std::cout << "Error creating GLFW window" << std::endl;
        glfwTerminate();
        throw Exception("Error creating GLFW window");
    }

    // Set the current OpenGL context to the window we just created
    glfwMakeContextCurrent(mWindow);

    if (mLoggingEnabled)
        std::cout << "Beginning initialization of GLEW" << std::endl;
    // Initialize GLEW so that we can load extensions
    glewExperimental = GL_TRUE; // This line is a must
    if (glewInit() != GLEW_OK) {
        if (mLoggingEnabled)
            std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        throw Exception("Failed to initialize GLEW");
    }

    if (mLoggingEnabled)
        std::cout << "GLEW successfully initialized" << std::endl;

    glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, GL_TRUE);

    //glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
    glEnable(GL_DEBUG_OUTPUT);
    //glDisable(GL_DEPTH_TEST);

    mCurrentScene->initScene();
}

void Application::setScene(Scene * scene) {
    mCurrentScene = scene;
}

void Application::run() {
    while (isRunning()) {
        //glClear(GL_COLOR_BUFFER_BIT);
        mCurrentScene->render();
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    mCurrentScene->cleanup();
}

bool Application::isRunning() {
    return glfwGetKey(mWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(mWindow) == 0;
}

void Application::setGLVersion(int major, int minor) {
    mGLMajor = major;
    mGLMinor = minor;
}

void Application::errorCallback(int error, const char * desc) {
    std::cout << "An error has occurred:\n\tError Code: " << error << "\n\tDescription: " << desc << std::endl;
}

GLFWwindow * Application::getWindow() {
    return mWindow;
}