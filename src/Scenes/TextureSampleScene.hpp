#ifndef ERICS3DENGINE_TEXTURESAMPLESCENE_HPP
#define ERICS3DENGINE_TEXTURESAMPLESCENE_HPP

#include "../Erics3DEngine/Engine/Scene.hpp"
#include "../Erics3DEngine/Engine/Shader.hpp"
#include "../Erics3DEngine/Engine/Program.hpp"
#include "../Erics3DEngine/Engine/Texture.hpp"
#include "../Erics3DEngine/Engine/Geometry/Geometry.hpp"
#include "../Erics3DEngine/Engine/Model/Model.hpp"
#include "../Erics3DEngine/Engine/Camera.hpp"
#include "../Scenes/LeapListener.hpp"

#include <Leap.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <utility>

class TextureSampleScene : public Scene {
    public:
        TextureSampleScene(GLFWwindow * window) : Scene(window) {}
        ~TextureSampleScene();

        void initScene();
        void render();
        void cleanup();

        void onKeyPress(GLFWwindow * window, int key, int scanCode, int action, int mode);
        void onMouseMove(GLFWwindow * window, double xPos, double yPos);
        void onMouseWheelScroll(GLFWwindow * window, double xOffset, double yOffset);

    private:
        GLuint mMatrixId;
        GLuint mModelMatrixId;
        GLuint mViewMatrixId;
        GLuint mTextureId;
        GLuint mLightId;
        glm::mat4 mProjectionMatrix;
        glm::mat4 mViewMatrix;
        glm::mat4 mModelMatrix;
        glm::mat4 mMVPMatrix;
        glm::mat4 mMMatrix;
        glm::mat4 mVMatrix;

        Texture * mTexture;
        Program * mDefaultProgram;
        Model * mModelObj;
        Camera mCamera;

        int mRotationSpeed;
        float mScale;

        const float DEBUG_PRINT_FREQ = 2.0f;
        bool shouldPrint = false;

        std::pair<int, int> getWindowSize();
};

#endif //ERICS3DENGINE_TEXTURESAMPLESCENE_HPP