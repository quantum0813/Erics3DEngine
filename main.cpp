#include "include/Shader.hpp"
#include "include/Program.hpp"
#include "include/Application.hpp"
#include "include/TestScene.hpp"
#include <map>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

int main() {
    Application * app = new Application(640, 480, "Test App");
    TestScene * scene = new TestScene();

    app->setScene(scene);
    app->setGLVersion(4, 1);
    app->initialize();
    app->run();

    delete app;
    delete scene;

    return 0;
}