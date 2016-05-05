#ifndef ERICS3DENGINE_STARFIELDSCENE_HPP
#define ERICS3DENGINE_STARFIELDSCENE_HPP

#include "../Erics3DEngine/Engine/Scene.hpp"
#include "../Erics3DEngine/Engine/Shader.hpp"
#include "../Erics3DEngine/Engine/Program.hpp"
#include "../Erics3DEngine/Engine/Texture.hpp"

#include <cmath>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class StarfieldScene : public Scene {
    struct Star {
        glm::vec3 position;
        glm::vec3 color;
    };

    public:
        StarfieldScene(GLFWwindow * window) : Scene(window) {}
        ~StarfieldScene();

        void initScene();
        void render();
        void cleanup();

        void onKeyPress(GLFWwindow * window, int key, int scanCode, int action, int mode);
        void onMouseMove(GLFWwindow * window, double xPos, double yPos);
        void onMouseWheelScroll(GLFWwindow * window, double xOffset, double yOffset);

    private:
        int mNumStars = 10000;

        Program * mRenderProgram;
        Texture * mStarTexture;

        GLuint mStarVAO;
        GLuint mStarBuffer;

        GLuint mUniformTime;
        GLuint mUniformProjMatrix;
};

static unsigned int seed = 0x13371337;

static inline float randomFloat() {
    float res;
    unsigned int tmp;

    seed *= 16807;
    tmp = seed ^ (seed >> 4) ^ (seed << 15);
    *((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;

    return (res - 1.0f);
}

#endif //ERICS3DENGINE_STARFIELDSCENE_HPP
