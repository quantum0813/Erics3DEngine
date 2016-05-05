#include "FlockingScene.hpp"
#include "../Erics3DEngine/Engine/Debug.hpp"
#include "../Erics3DEngine/Engine/Input.hpp"

#include <glm/glm.hpp>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

FlockingScene::~FlockingScene() { }

void FlockingScene::initScene() {
    static const glm::vec3 geometry[] = {
        // Vertices
        glm::vec3(-5.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, 1.5f, 0.0f),
        glm::vec3(-1.0f, 1.5f, 7.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 10.0f),
        glm::vec3(1.0f, 1.5f, 0.0f),
        glm::vec3(1.0f, 1.5f, 7.0f),
        glm::vec3(5.0f, 1.0f, 0.0f),

        // Normals
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        glm::vec3(0.107f, -0.859f, 0.00f),
        glm::vec3(0.832f, 0.554f, 0.00f),
        glm::vec3(-0.59f, -0.395f, 0.00f),
        glm::vec3(-0.832f, 0.554f, 0.00f),
        glm::vec3(0.295f, -0.196f, 0.00f),
        glm::vec3(0.124f, 0.992f, 0.00f),
    };

    Shader cs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/flocking.cs.glsl", Shader::ShaderType::COMPUTE);
    cs.compile();

    std::vector<Shader> updateShaders;
    updateShaders.push_back(cs);

    mFlockUpdateProgram = new Program(updateShaders);
    mFlockUpdateProgram->link();

    Shader vs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/flocking.vs.glsl", Shader::ShaderType::VERTEX);
    Shader fs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/flocking.fs.glsl", Shader::ShaderType::FRAGMENT);
    vs.compile();
    fs.compile();

    std::vector<Shader> renderShaders;
    renderShaders.push_back(vs);
    renderShaders.push_back(fs);

    mFlockRenderProgram = new Program(renderShaders);
    mFlockRenderProgram->link();

    mUniformRenderMVP = mFlockRenderProgram->getUniformLocation("mvp");

    glGenBuffers(2, mFlockBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mFlockBuffer[0]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, FLOCK_SIZE * sizeof(Boid), NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mFlockBuffer[1]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, FLOCK_SIZE * sizeof(Boid), NULL, GL_DYNAMIC_COPY);

    glGenBuffers(1, &mGeometryBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mGeometryBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW);

    glGenVertexArrays(2, mFlockRenderVAO);

    for (int i = 0; i < 2; i++) {
        glBindVertexArray(mFlockRenderVAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, mGeometryBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)(8 * sizeof(glm::vec3)));

        glBindBuffer(GL_ARRAY_BUFFER, mFlockBuffer[i]);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Boid), NULL);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Boid), (void *)sizeof(glm::vec4));
        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
    }

    glBindBuffer(GL_ARRAY_BUFFER, mFlockBuffer[0]);
    Boid * b = reinterpret_cast<Boid *>(glMapBufferRange(GL_ARRAY_BUFFER, 0, FLOCK_SIZE * sizeof(Boid), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

    for (int i = 0; i < FLOCK_SIZE; i++) {
        b[i].position = (randVec3() - glm::vec3(0.5f)) * 300.0f;
        b[i].velocity = (randVec3() - glm::vec3(0.5f));
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    mMVMatrix = glm::lookAt(
            glm::vec3(0.0f, 0.0f, -140.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
    );

    mProjMatrix = glm::perspective(
            60.0f,
            (float)1280 / (float)800,
            0.1f,
            3000.0f
    );

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void FlockingScene::render() {
    float t = (float)glfwGetTime();
    static const float black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const float one = 1.0f;

    mFlockUpdateProgram->use();

    mGoal = glm::vec3(
        sinf(t * 0.34f),
        cosf(t * 0.29f),
        sinf(t * 0.12f) * cosf(t * 0.5f)
    );

    mGoal = mGoal * glm::vec3(35.0f, 25.0f, 60.0f);

    glUniform3fv(mUniformUpdateGoal, 1, &mGoal[0]);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mFlockBuffer[mFrameIndex]);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mFlockBuffer[mFrameIndex ^ 1]);

    glDispatchCompute(NUM_WORKGROUPS, 1, 1);

    glViewport(0, 0, 1280, 800);
    glClearBufferfv(GL_COLOR, 0, black);
    glClearBufferfv(GL_DEPTH, 0, &one);

    mFlockRenderProgram->use();

    glm::mat4 mvp = mProjMatrix * mMVMatrix;

    glUniformMatrix4fv(mUniformRenderMVP, 1, GL_FALSE, &mvp[0][0]);
    glBindVertexArray(mFlockRenderVAO[mFrameIndex]);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 8, FLOCK_SIZE);

    mFrameIndex ^= 1;
}

void FlockingScene::cleanup() {

}

void FlockingScene::onKeyPress(GLFWwindow * window, int key, int scanCode, int action, int mode) { }

void FlockingScene::onMouseMove(GLFWwindow * window, double xPos, double yPos) {

}

void FlockingScene::onMouseWheelScroll(GLFWwindow * window, double xOffset, double yOffset) { }

glm::vec3 FlockingScene::randVec3() {
    float vals[3];

    for (int i = 0; i < 3; i++) {
        static unsigned int seed = 0x13371337;
        float res;
        unsigned int tmp;

        seed *= 16807;

        tmp = seed ^ (seed >> 4) ^ (seed << 15);

        *((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;

        vals[i] = (res - 1.0f);
    }

    return glm::vec3(vals[0], vals[1], vals[2]);
}