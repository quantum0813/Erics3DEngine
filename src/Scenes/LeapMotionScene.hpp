#ifndef ERICS3DENGINE_LEAPMOTIONSCENE_HPP
#define ERICS3DENGINE_LEAPMOTIONSCENE_HPP

#include "../Erics3DEngine/Engine/Scene.hpp"
#include "../Erics3DEngine/Engine/Shader.hpp"
#include "../Erics3DEngine/Engine/Program.hpp"
#include "../Erics3DEngine/Engine/Texture.hpp"
#include "../Erics3DEngine/Engine/Geometry/Geometry.hpp"
#include "../Erics3DEngine/Engine/Model/Model.hpp"
#include "../Scenes/LeapListener.hpp"

#include <Leap.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class LeapMotionScene : public Scene {
    public:
        LeapMotionScene(GLFWwindow * window) : Scene(window) {}
        ~LeapMotionScene();

        void initScene();
        void render();
        void cleanup();

    private:
        Leap::Controller mLeapController;
        LeapListener mListener;

        Program * mProgram;
        GLuint mRawSampler;
        GLuint mDistortionSampler;

        GLuint mRawLeftTexture;
        GLuint mRawRightTexture;
        GLuint mDistortionLeftTexture;
        GLuint mDistortionRightTexture;

        bool mShowLeftImage = false;

        GLuint createTextureReference();
        void setPerspectiveFrustrum(GLdouble fovY, GLdouble aspect, GLdouble nearPlane, GLdouble farPlane);
};

#endif //ERICS3DENGINE_LEAPMOTIONSCENE_HPP
