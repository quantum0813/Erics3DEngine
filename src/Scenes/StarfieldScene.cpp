#include <string.h>
#include "StarfieldScene.hpp"

StarfieldScene::~StarfieldScene() { }

void StarfieldScene::initScene() {
    Shader vs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/starfield.vs.glsl", Shader::ShaderType::VERTEX);
    vs.compile();
    Shader fs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/starfield.fs.glsl", Shader::ShaderType::FRAGMENT);
    fs.compile();

    std::vector<Shader> shaders;
    shaders.push_back(vs);
    shaders.push_back(fs);

    mRenderProgram = new Program(shaders);
    mRenderProgram->link();

    mUniformTime = mRenderProgram->getUniformLocation("time");
    mUniformProjMatrix = mRenderProgram->getUniformLocation("projMatrix");

    mStarTexture = new Texture("/home/eric/ClionProjects/Erics3DEngine/src/Scenes/data/textures/star.ktx");

    glGenVertexArrays(1, &mStarVAO);
    glBindVertexArray(mStarVAO);

    glGenBuffers(1, &mStarBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mStarBuffer);
    glBufferData(GL_ARRAY_BUFFER, mNumStars * sizeof(Star), NULL, GL_STATIC_DRAW);

    Star * star = (Star *)glMapBufferRange(GL_ARRAY_BUFFER, 0, mNumStars * sizeof(Star), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    for (int i = 0; i < 10000; i++) {
        star[i].position = glm::vec3(
            (randomFloat() * 2.0f - 1.0f) * 100.0f,
            (randomFloat() * 2.0f - 1.0f) * 100.0f,
            randomFloat()
        );
        star[i].color = glm::vec3(
            0.8f + randomFloat() * 0.2f,
            0.8f + randomFloat() * 0.2f,
            0.8f + randomFloat() * 0.2f
        );
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Star), NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Star), (void *)sizeof(glm::vec3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void StarfieldScene::render() {
    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static const GLfloat one[] = { 1.0f };
    float t = (float)glfwGetTime();

    glm::mat4 projMatrix = glm::perspective(
        50.0f,
        (float)1280 / (float)800,
        0.1f,
        1000.0f
    );

    t *= 0.1f;
    t -= floor(t);

    glViewport(0, 0, 1280, 800);
    glClearBufferfv(GL_COLOR, 0, black);
    glClearBufferfv(GL_DEPTH, 0, one);

    mRenderProgram->use();

    glUniform1f(mUniformTime, t);
    glUniformMatrix4fv(mUniformProjMatrix, 1, GL_FALSE, &projMatrix[0][0]);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glDrawArrays(GL_POINTS, 0, mNumStars);
}

void StarfieldScene::cleanup() {
}

void StarfieldScene::onKeyPress(GLFWwindow *window, int key, int scanCode, int action, int mode) { }

void StarfieldScene::onMouseMove(GLFWwindow *window, double xPos, double yPos) { }

void StarfieldScene::onMouseWheelScroll(GLFWwindow *window, double xOffset, double yOffset) { }