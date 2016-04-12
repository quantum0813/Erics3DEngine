#ifndef ERICS3DENGINE_TEXTURESAMPLESCENE_HPP
#define ERICS3DENGINE_TEXTURESAMPLESCENE_HPP

#include "../Erics3DEngine/Engine/Scene.hpp"
#include "../Erics3DEngine/Engine/Shader.hpp"
#include "../Erics3DEngine/Engine/Program.hpp"
#include "../Erics3DEngine/Engine/Texture.hpp"
#include "../Erics3DEngine/Engine/Geometry/Geometry.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class TextureSampleScene : public Scene {
    public:
        TextureSampleScene() : Scene() {}
        ~TextureSampleScene();

        void initScene();
        void render();
        void cleanup();

    private:
        GLuint mMatrixId;
        GLuint mTextureId;
        glm::mat4 mProjection;
        glm::mat4 mView;
        glm::mat4 mModel;
        glm::mat4 mMVP;

        Cube * mCube;
        Sphere * mSphere;
        Plane * mPlane;
        TruncatedCone * mTCone;

        Texture * mTexture;
        Program * mDefaultProgram;

        const float DEBUG_PRINT_FREQ = 2.0f;
        bool shouldPrint = false;
};

#endif //ERICS3DENGINE_TEXTURESAMPLESCENE_HPP