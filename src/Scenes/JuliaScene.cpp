#include <cmath>
#include <string.h>
#include "JuliaScene.hpp"
#include "../Erics3DEngine/Engine/Input.hpp"

JuliaScene::~JuliaScene() { }

void JuliaScene::initScene() {
    Input::initialize(this);

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    Shader vs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/julia.vs.glsl", Shader::ShaderType::VERTEX);
    vs.compile();
    Shader fs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/julia.fs.glsl", Shader::ShaderType::FRAGMENT);
    fs.compile();

    std::vector<Shader> shaders;
    shaders.push_back(vs);
    shaders.push_back(fs);

    mProgram = new Program(shaders);
    mProgram->link();

    mUniformZoom = mProgram->getUniformLocation("zoom");
    mUniformOffset = mProgram->getUniformLocation("offset");
    mUniformC = mProgram->getUniformLocation("C");

    glGenTextures(1, &mPaletteTexture);
    glBindTexture(GL_TEXTURE_1D, mPaletteTexture);
    glTexStorage1D(GL_TEXTURE_1D, 8, GL_RGB8, 256);
    glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 256, GL_RGB, GL_UNSIGNED_BYTE, palette);
    glGenerateMipmap(GL_TEXTURE_1D);
}

void JuliaScene::render() {
    static const GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glClearBufferfv(GL_COLOR, 0, color);

    static float t = 0.0f;
    float r = 0.0f;

    if (!mPaused)
        t = (float)glfwGetTime();

    r = t + mTimeOffset;

    float C[2] = {
        (sinf(r * 0.1f) + cosf(r * 0.23f)) * 0.5f,
        (cosf(r * 0.13f) + sinf(r * 0.21f)) * 0.5f
    };
    float offset[2] = { mXOffset, mYOffset };

    mProgram->use();

    glUniform2fv(mUniformC, 1, C);
    glUniform2fv(mUniformOffset, 1, offset);
    glUniform1f(mUniformZoom, mZoom);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void JuliaScene::cleanup() {
    glDeleteTextures(1, &mPaletteTexture);
    glDeleteVertexArrays(1, &mVAO);
    delete mProgram;
}

void JuliaScene::onKeyPress(GLFWwindow * window, int key, int scanCode, int action, int mode) {
    if (action == 1) {
        switch (key) {
            case GLFW_KEY_SPACE:
                mPaused = !mPaused;
                break;
            case GLFW_KEY_KP_ADD:
                mTimeOffset -= 0.00001f;
                break;
            case GLFW_KEY_KP_SUBTRACT:
                mTimeOffset += 0.00001f;
                break;
            case GLFW_KEY_9:
                mTimeOffset += 0.0001f;
                break;
            case GLFW_KEY_3:
                mTimeOffset -= 0.0001f;
                break;
            case GLFW_KEY_8:
                mTimeOffset += 0.01f;
                break;
            case GLFW_KEY_2:
                mTimeOffset -= 0.01f;
                break;
            case GLFW_KEY_7:
                mTimeOffset += 1.0f;
                break;
            case GLFW_KEY_1:
                mTimeOffset -= 1.0f;
                break;
            case GLFW_KEY_RIGHT_BRACKET:
                mZoom *= 1.02f;
                break;
            case GLFW_KEY_LEFT_BRACKET:
                mZoom /= 1.02f;
                break;
            case GLFW_KEY_W:
                mYOffset -= mZoom * 0.02f;
                break;
            case GLFW_KEY_A:
                mXOffset -= mZoom * 0.02f;
                break;
            case GLFW_KEY_S:
                mYOffset += mZoom * 0.02f;
                break;
            case GLFW_KEY_D:
                mXOffset += mZoom * 0.02f;
                break;
            default:
                break;
        }
    }
}

void JuliaScene::onMouseMove(GLFWwindow *window, double xPos, double yPos) { }

void JuliaScene::onMouseWheelScroll(GLFWwindow *window, double xOffset, double yOffset) { }