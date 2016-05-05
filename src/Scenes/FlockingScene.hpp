#ifndef ERICS3DENGINE_FLOCKINGSCENE_HPP
#define ERICS3DENGINE_FLOCKINGSCENE_HPP

#include "../Erics3DEngine/Engine/Scene.hpp"
#include "../Erics3DEngine/Engine/Shader.hpp"
#include "../Erics3DEngine/Engine/Program.hpp"
#include "../Erics3DEngine/Engine/Texture.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <utility>

class FlockingScene : public Scene {
    public:
        FlockingScene(GLFWwindow * window) : Scene(window) {}
        ~FlockingScene();

        void initScene();
        void render();
        void cleanup();

        void onKeyPress(GLFWwindow * window, int key, int scanCode, int action, int mode);
        void onMouseMove(GLFWwindow * window, double xPos, double yPos);
        void onMouseWheelScroll(GLFWwindow * window, double xOffset, double yOffset);

    private:
        const int WORKGROUP_SIZE = 256;
        const int NUM_WORKGROUPS = 64;
        const int FLOCK_SIZE = (NUM_WORKGROUPS * WORKGROUP_SIZE);

        Program * mFlockUpdateProgram;
        Program * mFlockRenderProgram;

        GLuint mFlockBuffer[2];

        GLuint mFlockRenderVAO[2];
        GLuint mGeometryBuffer;

        struct Boid {
            glm::vec3 position;
            unsigned int i1 : 32;
            glm::vec3 velocity;
            unsigned int i2 : 32;
        };

        GLuint mFrameIndex;
        GLint mUniformUpdateGoal;
        GLuint mUniformRenderMVP;

        glm::vec3 randVec3();

        glm::vec3 mGoal;
        glm::mat4 mMVMatrix;
        glm::mat4 mProjMatrix;
};

#endif //ERICS3DENGINE_FLOCKINGSCENE_HPP
