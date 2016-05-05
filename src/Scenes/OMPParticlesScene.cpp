#include "OMPParticlesScene.hpp"
#include "../Erics3DEngine/Engine/Debug.hpp"
#include "../Erics3DEngine/Engine/Input.hpp"

#include <omp.h>
#include <iostream>
#include <utility>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

OMPParticlesScene::~OMPParticlesScene() {}

void OMPParticlesScene::initScene() {
    // Application memory particle buffers (double buffered)
    mParticles[0] = new Particle[PARTICLE_COUNT];
    mParticles[1] = new Particle[PARTICLE_COUNT];

    // Create GPU buffer
    glGenBuffers(1, &mParticleBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffer);
    glBufferStorage(GL_ARRAY_BUFFER, PARTICLE_COUNT * sizeof(Particle), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
    mMappedBuffer = (Particle *)glMapBufferRange(GL_ARRAY_BUFFER, 0, PARTICLE_COUNT * sizeof(Particle), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);

    initializeParticles();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0);
    glBindVertexBuffer(0, mParticleBuffer, 0, sizeof(Particle));
    glEnableVertexAttribArray(0);

    Shader vs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/default.vs.glsl", Shader::ShaderType::VERTEX);
    Shader fs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/default.fs.glsl", Shader::ShaderType::FRAGMENT);
    vs.compile();
    fs.compile();

    std::vector<Shader> shaders;
    shaders.push_back(vs);
    shaders.push_back(fs);

    mDrawProgram = new Program(shaders);
    mDrawProgram->link();
    mDrawProgram->use();

    int maxThreads = omp_get_max_threads();
    omp_set_num_threads(maxThreads);
}

void OMPParticlesScene::initializeParticles() {
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        mParticles[0][i].position = glm::vec3(
            randomFloat() * 6.0f - 3.0f,
            randomFloat() * 6.0f - 3.0f,
            randomFloat() * 6.0f - 3.0f
        );
        mParticles[0][i].velocity = mParticles[0][i].position * 0.001f;

        mMappedBuffer[i] = mParticles[0][i];
    }
}

void OMPParticlesScene::updateParticles(float deltaTime) {
    // Double buffer source and destination
    const Particle * const __restrict src = mParticles[mFrameIndex & 1];
    Particle * const __restrict dst = mParticles[(mFrameIndex + 1) & 1];

    // For each particle in the system
    // #pragma omp parallel for schedule (dynamic, 16)
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        // Get my own data
        const Particle & me = src[i];
        glm::vec3 deltaV(0.0f);

        // For all other particles
        for (int j = 0; j < PARTICLE_COUNT; j++) {
            if (i != j) {
                // Get the vector to the other partifle
                glm::vec3 deltaPos = src[j].position - me.position;
                float distance = glm::length(deltaPos);
                // Normalize
                glm::vec3 deltaDir = deltaPos / distance;
                // This clamp stops the system from blowing up if particles get
                // too close...
                distance = distance < 0.005f ? 0.005f : distance;
                // Update velocity
                deltaV += (deltaDir / (distance * distance));
            }
        }
        // Add my current velocity to my position
        dst[i].position = me.position + me.velocity;
        // Product new velocity from my current velocity plus the calculated delta
        dst[i].velocity = me.velocity + deltaV * deltaTime * 0.01f;
        // Write to mapped buffer
        mMappedBuffer[i].position = dst[i].position;
    }

    // Count frames so we can double buffer next frame
    mFrameIndex++;
}

void OMPParticlesScene::updateParticlesOMP(float deltaTime) {
    // Double buffer source and destination
    const Particle * const __restrict src = mParticles[mFrameIndex & 1];
    Particle * const __restrict dst = mParticles[(mFrameIndex + 1) & 1];

    // For each particle in the system
    #pragma omp parallel for schedule (dynamic, 16)
    for (int i = 0; i < PARTICLE_COUNT; i++) {
        // Get my own data
        const Particle & me = src[i];
        glm::vec3 deltaV(0.0f);

        // For all other particles
        for (int j = 0; j < PARTICLE_COUNT; j++) {
            if (i != j) {
                // Get the vector to the other partifle
                glm::vec3 deltaPos = src[j].position - me.position;
                float distance = glm::length(deltaPos);
                // Normalize
                glm::vec3 deltaDir = deltaPos / distance;
                // This clamp stops the system from blowing up if particles get
                // too close...
                distance = distance < 0.005f ? 0.005f : distance;
                // Update velocity
                deltaV += (deltaDir / (distance * distance));
            }
        }
        // Add my current velocity to my position
        dst[i].position = me.position + me.velocity;
        // Product new velocity from my current velocity plus the calculated delta
        dst[i].velocity = me.velocity + deltaV * deltaTime * 0.01f;
        // Write to mapped buffer
        mMappedBuffer[i].position = dst[i].position;
    }

    // Count frames so we can double buffer next frame
    mFrameIndex++;
}

void OMPParticlesScene::render() {
    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static double previousTime = 0.0;

    // Calculate delta time
    float deltaTime = (float)(glfwGetTime() - previousTime);
    previousTime = glfwGetTime();

    if (mUseOMP)
        updateParticlesOMP(deltaTime * 0.001f);
    else
        updateParticles(deltaTime * 0.001f);

    // Clear
    glViewport(0, 0, 1280, 800);
    glClearBufferfv(GL_COLOR, 0, black);

    // Bind our vertex arrays
    glBindVertexArray(mVAO);

    // Let OpenGL know we've changed the contents of the buffer
    glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, PARTICLE_COUNT * sizeof(Particle));

    glPointSize(3.0f);

    // Draw!
    mDrawProgram->use();
    glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);
}

void OMPParticlesScene::cleanup() {
}

void OMPParticlesScene::onKeyPress(GLFWwindow * window, int key, int scanCode, int action, int mode) {
    if (action) {
        switch (key) {
            case GLFW_KEY_M:
                mUseOMP = !mUseOMP;
                break;
            default:
                break;
        }
    }
}

void OMPParticlesScene::onMouseMove(GLFWwindow * window, double xPos, double yPos) {

}

void OMPParticlesScene::onMouseWheelScroll(GLFWwindow * window, double xOffset, double yOffset) {

}