#ifndef ERICS3DENGINE_OMPPARTICLESSCENE_HPP
#define ERICS3DENGINE_OMPPARTICLESSCENE_HPP

#include "../Erics3DEngine/Engine/Scene.hpp"
#include "../Erics3DEngine/Engine/Shader.hpp"
#include "../Erics3DEngine/Engine/Program.hpp"
#include "../Erics3DEngine/Engine/Texture.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OMPParticlesScene : public Scene {
    public:
        OMPParticlesScene(GLFWwindow * window) : mFrameIndex(0), mUseOMP(true), Scene(window) {}
        ~OMPParticlesScene();

        void initScene();
        void render();
        void cleanup();

        void onKeyPress(GLFWwindow * window, int key, int scanCode, int action, int mode);
        void onMouseMove(GLFWwindow * window, double xPos, double yPos);
        void onMouseWheelScroll(GLFWwindow * window, double xOffset, double yOffset);

    private:
        const int PARTICLE_COUNT = 2048;
        struct Particle {
            glm::vec3 position;
            glm::vec3 velocity;
        };

        GLuint mParticleBuffer;
        Particle * mMappedBuffer;
        Particle * mParticles[2];
        int mFrameIndex;
        GLuint mVAO;
        Program * mDrawProgram;
        bool mUseOMP;

        void initializeParticles();
        void updateParticles(float deltaTime);
        void updateParticlesOMP(float deltaTime);
};

static unsigned int seed = 0x13371337;

static inline float randomFloat() {
    float res;
    unsigned int tmp;

    seed *= 16807;
    tmp = seed ^ (seed >> 4) ^ (seed << 15);
    *((unsigned int *) &res) = (tmp >> 9) | 0x3F800000;

    return (res - 1.0f);
}

#endif //ERICS3DENGINE_OMPPARTICLESSCENE_HPP
