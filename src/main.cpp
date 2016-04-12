#include "Erics3DEngine/Engine/Shader.hpp"
#include "Erics3DEngine/Engine/Program.hpp"
#include "Erics3DEngine/Engine/Application.hpp"
#include "Scenes/RainbowTriangleScene.hpp"
#include "Scenes/TextureSampleScene.hpp"

int main() {
    Application * app = new Application(640, 480, "Test App");
    //RainbowTriangleScene * scene = new RainbowTriangleScene();
    TextureSampleScene * scene = new TextureSampleScene();

    app->setScene(scene);
    app->setGLVersion(4, 1);
    app->initialize();
    app->run();

    delete app;
    delete scene;

    return 0;
}