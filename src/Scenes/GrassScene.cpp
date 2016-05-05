#include <glm/gtc/matrix_transform.hpp>
#include "GrassScene.hpp"

GrassScene::~GrassScene() { }

void GrassScene::initScene() {
    static const GLfloat grassBlade[] = {
        -0.3f, 0.0f,
        0.3f, 0.0f,
        -0.20f, 1.0f,
        0.1f, 1.3f,
        -0.05f, 2.3f,
        0.0f, 3.3f
    };

    glGenBuffers(1, &mGrassBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mGrassBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grassBlade), grassBlade, GL_STATIC_DRAW);

    glGenVertexArrays(1, &mGrassVAO);
    glBindVertexArray(mGrassVAO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    Shader vs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/grass.vs.glsl", Shader::ShaderType::VERTEX);
    vs.compile();
    Shader fs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/grass.fs.glsl", Shader::ShaderType::FRAGMENT);
    fs.compile();
    std::vector<Shader> shaders;
    shaders.push_back(vs);
    shaders.push_back(fs);

    mGrassProgram = new Program(shaders);
    mGrassProgram->link();

    mUniformMVPMatrix = mGrassProgram->getUniformLocation("mvpMatrix");

    glActiveTexture(GL_TEXTURE1);
    mTexGrassLength = new Texture("/home/eric/ClionProjects/Erics3DEngine/src/Scenes/data/textures/grass_length.ktx");
    glActiveTexture(GL_TEXTURE2);
    mTexGrassOrientation = new Texture("/home/eric/ClionProjects/Erics3DEngine/src/Scenes/data/textures/grass_orientation.ktx");
    glActiveTexture(GL_TEXTURE3);
    mTexGrassColor = new Texture("/home/eric/ClionProjects/Erics3DEngine/src/Scenes/data/textures/grass_color.ktx");
    glActiveTexture(GL_TEXTURE4);
    mTexGrassBend = new Texture("/home/eric/ClionProjects/Erics3DEngine/src/Scenes/data/textures/grass_bend.ktx");
}

void GrassScene::render() {
    float t = (float) glfwGetTime() * 0.02f;
    float r = 550.0f;

    static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
    static const GLfloat one = 1.0f;
    glClearBufferfv(GL_COLOR, 0, black);
    glClearBufferfv(GL_DEPTH, 0, &one);

    glm::mat4 mvMatrix = glm::lookAt(
            glm::vec3(sinf(t) * r, 25.0f, cosf(t) * r),
            glm::vec3(0.0f, -50.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
    );

    glm::mat4 projMatrix = glm::perspective(
            45.0f,
            (float) 1280 / (float) 800,
            0.1f,
            1000.0f
    );

    glm::mat4 mvp = projMatrix * mvMatrix;

    mGrassProgram->use();
    glUniformMatrix4fv(mUniformMVPMatrix, 1, GL_FALSE, &mvp[0][0]);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glViewport(0, 0, 1280, 800);

    glBindVertexArray(mGrassVAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, 1024 * 1024);
}

void GrassScene::cleanup() {

}

void GrassScene::onKeyPress(GLFWwindow *window, int key, int scanCode, int action, int mode) { }

void GrassScene::onMouseMove(GLFWwindow *window, double xPos, double yPos) { }

void GrassScene::onMouseWheelScroll(GLFWwindow *window, double xOffset, double yOffset) { }