#include "TextureSampleScene.hpp"
#include "../Erics3DEngine/Engine/Debug.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void TextureSampleScene::initScene() {
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //mCube = new Cube(3.0f);
    //mSphere = new Sphere(1.0f, 12, 12);
    //mPlane = new Plane(1.0f, 4.0f, 1, 1);
    mTCone = new TruncatedCone(2.0f, 1.0f, 2.0f, 24, 24, true, true);

    // Create our shader program
    Shader vs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/texture.vs.glsl", Shader::ShaderType::VERTEX);
    Shader fs = Shader("/home/eric/ClionProjects/Erics3DEngine/shaders/texture.fs.glsl", Shader::ShaderType::FRAGMENT);
    vs.compile();
    fs.compile();

    std::vector<Shader> shaders;
    shaders.push_back(vs);
    shaders.push_back(fs);

    mDefaultProgram = new Program(shaders);
    mDefaultProgram->link();
    mDefaultProgram->use();

    mMatrixId = mDefaultProgram->getUniformLocation("mvp");
    mProjection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    mView = glm::lookAt(
            glm::vec3(4, 4, 3),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
    );
    mModel = glm::mat4(1.0f);
    mMVP = mProjection * mView * mModel;

    /*mP = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));//glm::lookAt(glm::vec3(4.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model = glm::mat4(1.0f);
    mMV = view * model;

    mVertexPosAttribId = mDefaultProgram->getAttribLocation("vertexPos");
    glEnableVertexAttribArray(mVertexPosAttribId);

    mTexcoordAttribId = mDefaultProgram->getAttribLocation("texCoord");
    glEnableVertexAttribArray(mTexcoordAttribId);

    mTexUniformId = mDefaultProgram->getUniformLocation("mSampler");

    mPMatrixUniformId = mDefaultProgram->getUniformLocation("pMatrix");
    mMVMatrixUniformId = mDefaultProgram->getUniformLocation("mvMatrix");*/

    /*mMatrixId = mDefaultProgram->getUniformLocation("mvp");

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));//glm::lookAt(glm::vec3(4.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model = glm::mat4(1.0f);

    mMVP = projection * view * model;*/

    mTexture = new Texture("/home/eric/ClionProjects/Erics3DEngine/src/Scenes/data/textures/moon.jpg");
    mTexture->bind();

    mTextureId = mDefaultProgram->getUniformLocation("mTextureSampler");

    Util::printFirstNDebugMessages(10);
}

void TextureSampleScene::render() {
    static double lastPrintTime = glfwGetTime();
    if (glfwGetTime() - lastPrintTime >= DEBUG_PRINT_FREQ) {
        lastPrintTime = glfwGetTime();
        shouldPrint = true;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mDefaultProgram->use();

    glUniformMatrix4fv(mMatrixId, 1, GL_FALSE, &mMVP[0][0]);

    mTexture->bind();
    glUniform1i(mTextureId, 0);

    //mCube->render();
    //mSphere->render();
    //mPlane->render();
    mTCone->render();

    if (shouldPrint)
        Util::printFirstNDebugMessages(10);

    shouldPrint = false;
}

void TextureSampleScene::cleanup() {
    glDeleteProgram(mDefaultProgram->getProgramHandle());
    for (std::vector<Shader>::iterator it = mDefaultProgram->getShaders().begin(); it != mDefaultProgram->getShaders().end(); it++)
        glDeleteShader(it->getShaderHandle());
    delete mCube;
    delete mTexture;
    delete mDefaultProgram;
}

TextureSampleScene::~TextureSampleScene() { }