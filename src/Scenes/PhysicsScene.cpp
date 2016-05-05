#include "PhysicsScene.hpp"
#include "../Erics3DEngine/Engine/Debug.hpp"
#include "../Erics3DEngine/Engine/Input.hpp"

#include <iostream>
#include <utility>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
//#include <bullet/btBulletDynamicsCommon.h>

PhysicsScene::~PhysicsScene() { }

void PhysicsScene::initScene() {
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

    /*btBroadphaseInterface * broadphase = new btDbvtBroadphase();

    btDefaultCollisionConfiguration * collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher * dispatcher = new btCollisionDispatcher(collisionConfiguration);

    btSequentialImpulseConstraintSolver * solver = new btSequentialImpulseConstraintSolver;

    btDiscreteDynamicsWorld * dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0, -10, 0));*/


    Util::printFirstNDebugMessages(10);
}

void PhysicsScene::render() {
    static double lastPrintTime = glfwGetTime();
    if (glfwGetTime() - lastPrintTime >= DEBUG_PRINT_FREQ) {
        lastPrintTime = glfwGetTime();
        shouldPrint = true;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mDefaultProgram->use();

    float angle = glfwGetTime() / 1000.0 * 25;
    // Model matrix = TranslationMatrix * RotationMatrix * ScaleMatrix
    mModelMatrix = glm::mat4(1.0f);
    mModelMatrix = glm::rotate(mModelMatrix, glm::degrees(angle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(mModelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
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

void PhysicsScene::cleanup() {
    glDeleteProgram(mDefaultProgram->getProgramHandle());
    for (std::vector<Shader>::iterator it = mDefaultProgram->getShaders().begin(); it != mDefaultProgram->getShaders().end(); it++)
        glDeleteShader(it->getShaderHandle());
    delete mTexture;
    delete mDefaultProgram;
}

void PhysicsScene::onKeyPress(GLFWwindow * window, int key, int scanCode, int action, int mode) {

}

void PhysicsScene::onMouseMove(GLFWwindow * window, double xPos, double yPos) {

}

void PhysicsScene::onMouseWheelScroll(GLFWwindow * window, double xOffset, double yOffset) {

}

std::pair<int, int> PhysicsScene::getWindowSize() {
    std::pair<int, int> size;
    glfwGetWindowSize(glfwGetCurrentContext(), &size.first, &size.second);
    return size;
}