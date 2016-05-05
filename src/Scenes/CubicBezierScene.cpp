#include <string.h>
#include "CubicBezierScene.hpp"

CubicBezierScene::~CubicBezierScene() { }

void CubicBezierScene::initScene() {
    Shader vs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/cubicBezier.vs.glsl", Shader::ShaderType::VERTEX);
    Shader tcs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/cubicBezier.tcs.glsl", Shader::ShaderType::TESS_CONTROL);
    Shader tes = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/cubicBezier.tes.glsl", Shader::ShaderType::TESS_EVAL);
    Shader fs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/cubicBezier.fs.glsl", Shader::ShaderType::FRAGMENT);
    vs.compile();
    tcs.compile();
    tes.compile();
    fs.compile();
    std::vector<Shader> shaders;

    shaders.push_back(vs);
    shaders.push_back(tcs);
    shaders.push_back(tes);
    shaders.push_back(fs);

    mTessProgram = new Program(shaders);
    mTessProgram->link();

    mUniformPatchMVMatrix = mTessProgram->getUniformLocation("mvMatrix");
    mUniformPatchProjMatrix = mTessProgram->getUniformLocation("projMatrix");
    mUniformPatchMVPMatrix = mTessProgram->getUniformLocation("mvp");

    Shader cpVS = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/drawCtrlPts.vs.glsl", Shader::ShaderType::VERTEX);
    Shader cpFS = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/drawCtrlPts.fs.glsl", Shader::ShaderType::FRAGMENT);
    cpVS.compile();
    cpFS.compile();
    std::vector<Shader> cpShaders;

    cpShaders.push_back(cpVS);
    cpShaders.push_back(cpFS);

    mDrawCPProgram = new Program(cpShaders);
    mDrawCPProgram->link();

    mUniformControlPtDrawColor = mDrawCPProgram->getUniformLocation("drawColor");
    mUniformControlPtMVPMatrix = mDrawCPProgram->getUniformLocation("mvp");

    glGenVertexArrays(1, &mPatchVAO);
    glBindVertexArray(mPatchVAO);

    glGenBuffers(1, &mPatchBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mPatchBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(mPatchData), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    static const GLushort indices[] = {
        0, 1, 1, 2, 2, 3,
        4, 5, 5, 6, 6, 7,
        8, 9, 9, 10, 10, 11,
        12, 13, 13, 14, 14, 15,

        0, 4, 4, 8, 8, 12,
        1, 5, 5, 9, 9, 13,
        2, 6, 6, 10, 10, 14,
        3, 7, 7, 11, 11, 15
    };

    glGenBuffers(1, &mCageIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCageIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    mOverlay.init(80, 50);
    mOverlay.clear();
    mOverlay.drawText("W: Toggle wireframe", 0, 1);
    mOverlay.drawText("C: Toggle control cage", 0, 1);
    mOverlay.drawText("X: Toggle control points", 0, 2);
    mOverlay.drawText("P: Pause", 0, 3);
}

void CubicBezierScene::render() {
    static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
    static const GLfloat one = 1.0f;

    static double lastTime = 0.0;
    static double totalTime = 0.0;

    if (!mPaused)
        totalTime += (glfwGetTime() - lastTime);
    lastTime = glfwGetTime();

    float t = (float)totalTime;

    static const float patchInitializer[] = {
        -1.0f,  -1.0f,  0.0f,
        -0.33f, -1.0f,  0.0f,
        0.33f, -1.0f,  0.0f,
        1.0f,  -1.0f,  0.0f,

        -1.0f,  -0.33f, 0.0f,
        -0.33f, -0.33f, 0.0f,
        0.33f, -0.33f, 0.0f,
        1.0f,  -0.33f, 0.0f,

        -1.0f,   0.33f, 0.0f,
        -0.33f,  0.33f, 0.0f,
        0.33f,  0.33f, 0.0f,
        1.0f,   0.33f, 0.0f,

        -1.0f,   1.0f,  0.0f,
        -0.33f,  1.0f,  0.0f,
        0.33f,  1.0f,  0.0f,
        1.0f,   1.0f,  0.0f,
    };

    glViewport(0, 0, 1280, 800);
    glClearBufferfv(GL_COLOR, 0, gray);
    glClearBufferfv(GL_DEPTH, 0, &one);

    glEnable(GL_DEPTH_TEST);

    glm::vec3 * p = (glm::vec3 *)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(mPatchData), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    memcpy(p, patchInitializer, sizeof(patchInitializer));

    for (int i = 0; i < 16; i++) {
        float fi = (float)i / 16.0f;
        p[i][2] = sinf(t * (0.2f + fi * 0.3f));
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindVertexArray(mPatchVAO);
    mTessProgram->use();

    glm::mat4 projMatrix = glm::perspective(
        50.0f,
        (float)1280 / (float)800,
        1.0f,
        1000.0f
    );

    glm::mat4 mvMatrix =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f)) *
        glm::rotate(glm::mat4(1.0f), t * 10.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), t * 17.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 mvp = projMatrix * mvMatrix;

    glUniformMatrix4fv(mUniformPatchMVMatrix, 1, GL_FALSE, &mvMatrix[0][0]);
    glUniformMatrix4fv(mUniformPatchProjMatrix, 1, GL_FALSE, &projMatrix[0][0]);
    glUniformMatrix4fv(mUniformPatchMVPMatrix, 1, GL_FALSE, &mvp[0][0]);

    if (mWireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPatchParameteri(GL_PATCH_VERTICES, 16);
    glDrawArrays(GL_PATCHES, 0, 16);

    mDrawCPProgram->use();
    glUniformMatrix4fv(mUniformControlPtMVPMatrix, 1, GL_FALSE, &mvp[0][0]);

    if (mShowPoints) {
        glPointSize(9.0f);
        glUniform4fv(mUniformControlPtDrawColor, 1, &glm::vec4(0.2f, 0.7f, 0.9f, 1.0f)[0]);
        glDrawArrays(GL_POINTS, 0, 16);
    }

    if (mShowCage) {
        glUniform4fv(mUniformControlPtDrawColor, 1, &glm::vec4(0.7f, 0.9f, 0.2f, 1.0f)[0]);
        glDrawElements(GL_LINES, 48, GL_UNSIGNED_SHORT, NULL);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    mOverlay.render();
}

void CubicBezierScene::cleanup() {
}

void CubicBezierScene::onKeyPress(GLFWwindow * window, int key, int scanCode, int action, int mode) {
    switch (key) {
        case GLFW_KEY_C:
            mShowCage = !mShowCage;
            break;
        case GLFW_KEY_X:
            mShowPoints = !mShowPoints;
            break;
        case GLFW_KEY_W:
            mWireframe = !mWireframe;
            break;
        case GLFW_KEY_P:
            mPaused = !mPaused;
            break;
        default:
            break;
    }
}

void CubicBezierScene::onMouseMove(GLFWwindow *window, double xPos, double yPos) { }

void CubicBezierScene::onMouseWheelScroll(GLFWwindow *window, double xOffset, double yOffset) { }