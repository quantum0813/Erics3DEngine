#ifndef ERICS3DENGINE_CUBICBEZIERSCENE_HPP
#define ERICS3DENGINE_CUBICBEZIERSCENE_HPP

#include "../Erics3DEngine/Engine/Scene.hpp"
#include "../Erics3DEngine/Engine/Shader.hpp"
#include "../Erics3DEngine/Engine/Program.hpp"
#include "../Erics3DEngine/Engine/Texture.hpp"
#include "../Erics3DEngine/Engine/TextOverlay.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CubicBezierScene : public Scene {
    public:
        CubicBezierScene(GLFWwindow * window) : mShowPoints(false), mShowCage(false), mWireframe(false), mPaused(false), Scene(window) {}
        ~CubicBezierScene();

        void initScene();
        void render();
        void cleanup();

        void onKeyPress(GLFWwindow * window, int key, int scanCode, int action, int mode);
        void onMouseMove(GLFWwindow * window, double xPos, double yPos);
        void onMouseWheelScroll(GLFWwindow * window, double xOffset, double yOffset);

    private:
        Program * mTessProgram;
        Program * mDrawCPProgram;
        GLuint mPatchVAO;
        GLuint mPatchBuffer;
        GLuint mCageIndices;
        glm::vec3 mPatchData[16];

        bool mShowPoints;
        bool mShowCage;
        bool mWireframe;
        bool mPaused;

        GLuint mUniformPatchMVMatrix;
        GLuint mUniformPatchProjMatrix;
        GLuint mUniformPatchMVPMatrix;
        GLuint mUniformControlPtDrawColor;
        GLuint mUniformControlPtMVPMatrix;

        TextOverlay mOverlay;
};

#endif //ERICS3DENGINE_CUBICBEZIERSCENE_HPP
