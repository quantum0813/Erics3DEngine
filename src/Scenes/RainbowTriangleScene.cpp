#include "RainbowTriangleScene.hpp"
#include <math.h>

void RainbowTriangleScene::initScene() {
    // Create a vertex array object
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // Create a vertex buffer object and copy the vertex data to it
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

    // Create our shader program
    Shader vs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/rainbow_tri.vs.glsl", Shader::ShaderType::VERTEX);
    Shader fs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/rainbow_tri.fs.glsl", Shader::ShaderType::FRAGMENT);
    vs.compile();
    fs.compile();

    std::vector<Shader> shaders;
    shaders.push_back(vs);
    shaders.push_back(fs);

    mDefaultProgram = new Program(shaders);
    mDefaultProgram->link();
    mDefaultProgram->use();

    mPosAttrib = glGetAttribLocation(mDefaultProgram->getProgramHandle(), "position");
    glEnableVertexAttribArray(mPosAttrib);
    glVertexAttribPointer(mPosAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

    mColAttrib = glGetAttribLocation(mDefaultProgram->getProgramHandle(), "color");
    glEnableVertexAttribArray(mColAttrib);
    glVertexAttribPointer(mColAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
}

RainbowTriangleScene::~RainbowTriangleScene() {
    delete mDefaultProgram;
}

void RainbowTriangleScene::render() {
    // Calculate new triangle alpha for current frame
    GLint uniColor = glGetUniformLocation(mDefaultProgram->getProgramHandle(), "vertexColor");
    float time = (float)glfwGetTime();
    float adjFactor = sin(time) - 0.5f;
    glUniform3f(uniColor, adjFactor, adjFactor, adjFactor);

    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a triangle from the 3 vertices
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void RainbowTriangleScene::cleanup() {
    glDeleteProgram(mDefaultProgram->getProgramHandle());
    for (std::vector<Shader>::iterator it = mDefaultProgram->getShaders().begin(); it != mDefaultProgram->getShaders().end(); it++)
        glDeleteShader(it->getShaderHandle());
    glDeleteBuffers(1, &mVBO);
    glDeleteVertexArrays(1, &mVAO);
}

void RainbowTriangleScene::onKeyPress(GLFWwindow * window, int key, int scanCode, int action, int mode) { }
void RainbowTriangleScene::onMouseMove(GLFWwindow * window, double xPos, double yPos) { }
void RainbowTriangleScene::onMouseWheelScroll(GLFWwindow * window, double xOffset, double yOffset) { }