#include "LeapMotionScene.hpp"
#include "../Erics3DEngine/Engine/Debug.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

LeapMotionScene::~LeapMotionScene() { }

void LeapMotionScene::initScene() {
    Shader vs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/leapScene.vs.glsl", Shader::ShaderType::VERTEX);
    Shader fs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/leapScene.fs.glsl", Shader::ShaderType::FRAGMENT);
    vs.compile();
    fs.compile();

    std::vector<Shader> shaders;
    shaders.push_back(vs);
    shaders.push_back(fs);

    mProgram = new Program(shaders);
    mProgram->link();
    mProgram->use();

    mRawSampler = mProgram->getUniformLocation("rawTexture");
    mDistortionSampler = mProgram->getUniformLocation("distortionTexture");
    glUniform1i(mRawSampler, 0);
    glUniform1i(mDistortionSampler, 1);

    mRawLeftTexture = createTextureReference();
    mRawRightTexture = createTextureReference();
    mDistortionLeftTexture = createTextureReference();
    mDistortionRightTexture = createTextureReference();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    setPerspectiveFrustrum(110, (1280 * 2) / 800, 1, 20);
    glMatrixMode(GL_MODELVIEW);
}

void LeapMotionScene::render() {
    mLeapController.setPolicyFlags(Controller::POLICY_IMAGES);

    Frame frame = mLeapController.frame();
    if (frame.isValid()) {
        // Update image and distortion textures
        Image left = frame.images()[0];
        if (left.width() > 0) {
            glBindTexture(GL_TEXTURE_2D, mRawLeftTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, left.width(), left.height(), 0, GL_RED, GL_UNSIGNED_BYTE, left.data());
            glBindTexture(GL_TEXTURE_2D, mDistortionLeftTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, left.distortionWidth() / 2, left.distortionHeight(), 0, GL_RG, GL_FLOAT, left.distortion());
        }
        Image right = frame.images()[1];
        if (right.width() > 0) {
            glBindTexture(GL_TEXTURE_2D, mRawRightTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, right.width(), right.height(), 0, GL_RED, GL_UNSIGNED_BYTE, right.data());
            glBindTexture(GL_TEXTURE_2D, mDistortionRightTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, right.distortionWidth() / 2, right.distortionHeight(), 0, GL_RG, GL_FLOAT, right.distortion());
        }
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Move world away from the camera
    glTranslatef(0.0f, 0.0f, -10.0f);

    glPushMatrix();
    // Move sensor images away from camera and scale to maintain distance-to-size ratio of 4:1
    glScalef(80.0f, 80.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, -10.0f);

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    mProgram->use();

    if (mShowLeftImage) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mRawLeftTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mDistortionLeftTexture);
    } else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mRawRightTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mDistortionRightTexture);
    }

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f); // Top Left
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f); // Top Right
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f); // Bottom Right
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f); // Bottom Left
    glEnd(); // Done Drawing The Quad
    glPopMatrix();
}

void LeapMotionScene::cleanup() {
}

GLuint LeapMotionScene::createTextureReference() {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return textureID;
}

void LeapMotionScene::setPerspectiveFrustrum(GLdouble fovY, GLdouble aspect, GLdouble nearPlane, GLdouble farPlane) {
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    fH = tan(fovY / 360 * pi) * nearPlane;
    fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, nearPlane, farPlane);
}