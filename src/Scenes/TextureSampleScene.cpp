#include "TextureSampleScene.hpp"
#include "../Erics3DEngine/Engine/Debug.hpp"
#include "../Erics3DEngine/Engine/Input.hpp"

#include <iostream>
#include <utility>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TextureSampleScene::~TextureSampleScene() { }

void TextureSampleScene::initScene() {
    Input::initialize(this);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    mModelObj = new Model("/home/eric/ClionProjects/Erics3DEngine/src/Scenes/data/nanosuit/nanosuit.obj");

    // Create our shader program
    Shader vs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/default.vs.glsl", Shader::ShaderType::VERTEX);
    Shader fs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/default.fs.glsl", Shader::ShaderType::FRAGMENT);
    vs.compile();
    fs.compile();

    std::vector<Shader> shaders;
    shaders.push_back(vs);
    shaders.push_back(fs);

    mDefaultProgram = new Program(shaders);
    mDefaultProgram->link();
    mDefaultProgram->use();

    mMatrixId = mDefaultProgram->getUniformLocation("mvp");
    mViewMatrixId = mDefaultProgram->getUniformLocation("v");
    mModelMatrixId = mDefaultProgram->getUniformLocation("m");
    mLightId = mDefaultProgram->getUniformLocation("lightPosWorldspace");

    mTexture = new Texture("/home/eric/ClionProjects/Erics3DEngine/src/Scenes/data/textures/moon.jpg");
    mTexture->bind();

    mTextureId = mDefaultProgram->getUniformLocation("mTextureSampler");

    /**
     * Camera defaults:
     *
     * FOV: 45.0f
     * Aspect ratio: 4.0f / 3.0f
     * Near plane: 0.1f
     * Far plane: 100.0f
     * Eye location: (0.0f, 0.0f, 0.0f)
     * Center location: (0.0f, 0.0f, 0.0f)
     * Up: (0.0f, 1.0f, 0.0f)
     */

    std::pair<int, int> windowSize = getWindowSize();

    mCamera = Camera();
    mCamera.setAspectRatio((float)windowSize.first / (float)windowSize.second);
    mCamera.setEyeLocation(vec3(0.0f, 0.0f, -6.0f));
    mCamera.setCenterLocation(vec3(0.0f, 1.0f, 0.0f));

    mRotationSpeed = 25;
    mScale = 0.2f;

    Util::printFirstNDebugMessages(10);
}

void TextureSampleScene::render() {
    static double lastPrintTime = glfwGetTime();
    if (glfwGetTime() - lastPrintTime >= DEBUG_PRINT_FREQ) {
        lastPrintTime = glfwGetTime();
        shouldPrint = true;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mDefaultProgram->use();

    float angle = glfwGetTime() / 1000.0 * mRotationSpeed;
    // Model matrix = TranslationMatrix * RotationMatrix * ScaleMatrix
    mModelMatrix = glm::mat4(1.0f);
    mModelMatrix = glm::rotate(mModelMatrix, glm::degrees(angle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(mModelMatrix, glm::vec3(mScale, mScale, mScale));
    mMVPMatrix = mCamera.getProjectionMatrix() * mCamera.getViewMatrix() * mModelMatrix;

    glUniformMatrix4fv(mMatrixId, 1, GL_FALSE, &mMVPMatrix[0][0]);
    glUniformMatrix4fv(mModelMatrixId, 1, GL_FALSE, &mModelMatrix[0][0]);
    glUniformMatrix4fv(mViewMatrixId, 1, GL_FALSE, &mCamera.getViewMatrix()[0][0]);

    glm::vec3 lightPos = glm::vec3(-1.0f, 2.0f, -3.0f);
    glUniform3f(mLightId, lightPos.x, lightPos.y, lightPos.z);

    mTexture->bind();
    glUniform1i(mTextureId, 0);

    mModelObj->render(mDefaultProgram);

    if (shouldPrint)
        Util::printFirstNDebugMessages(10);

    shouldPrint = false;
}

void TextureSampleScene::cleanup() {
    glDeleteProgram(mDefaultProgram->getProgramHandle());
    for (std::vector<Shader>::iterator it = mDefaultProgram->getShaders().begin(); it != mDefaultProgram->getShaders().end(); it++)
        glDeleteShader(it->getShaderHandle());
    delete mTexture;
    delete mDefaultProgram;
}

void TextureSampleScene::onKeyPress(GLFWwindow * window, int key, int scanCode, int action, int mode) {
    glm::vec3 centerLoc = mCamera.getCenterLocation();
    switch (key) {
        case GLFW_KEY_KP_ADD:
            mRotationSpeed += 10;
            break;
        case GLFW_KEY_KP_SUBTRACT:
            mRotationSpeed -= 10;
            break;
        case GLFW_KEY_UP:
            mCamera.setCenterLocation(glm::vec3(centerLoc.x, centerLoc.y + 0.1f, centerLoc.z));
            break;
        case GLFW_KEY_DOWN:
            mCamera.setCenterLocation(glm::vec3(centerLoc.x, centerLoc.y - 0.1f, centerLoc.z));
            break;
        case GLFW_KEY_LEFT:
            mCamera.setCenterLocation(glm::vec3(centerLoc.x - 0.1f, centerLoc.y, centerLoc.z));
            break;
        case GLFW_KEY_RIGHT:
            mCamera.setCenterLocation(glm::vec3(centerLoc.x + 0.1f, centerLoc.y, centerLoc.z));
            break;
        default:
            break;
    }
}

void TextureSampleScene::onMouseMove(GLFWwindow * window, double xPos, double yPos) {

}

void TextureSampleScene::onMouseWheelScroll(GLFWwindow * window, double xOffset, double yOffset) {
    if (yOffset > 0) {
        mScale += 0.01f;
    } else {
        if ((mScale - 0.01f) > 0)
            mScale -= 0.01f;
    }
}

std::pair<int, int> TextureSampleScene::getWindowSize() {
    std::pair<int, int> size;
    glfwGetWindowSize(glfwGetCurrentContext(), &size.first, &size.second);
    return size;
}