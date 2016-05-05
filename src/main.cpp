#include "Erics3DEngine/Engine/Shader.hpp"
#include "Erics3DEngine/Engine/Application.hpp"
#include "Scenes/FlockingScene.hpp"
#include "Scenes/GrassScene.hpp"
#include "Scenes/CubicBezierScene.hpp"
#include "Scenes/StarfieldScene.hpp"
#include "Scenes/TextureSampleScene.hpp"
#include "Scenes/JuliaScene.hpp"

int main() {
    Application * app = new Application(1280, 800, "Eric's 3D Engine");
    //FlockingScene * scene = new FlockingScene(app->getWindow());
    //GrassScene * scene = new GrassScene(app->getWindow());
    //StarfieldScene * scene = new StarfieldScene(app->getWindow());
    TextureSampleScene * scene = new TextureSampleScene(app->getWindow());
    //JuliaScene * scene = new JuliaScene(app->getWindow());

    app->setScene(scene);
    app->setGLVersion(4, 3);
    app->initialize();
    app->run();

    delete app;
    delete scene;

    return 0;
}