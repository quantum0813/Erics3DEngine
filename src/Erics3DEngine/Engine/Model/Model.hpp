#ifndef ERICS3DENGINE_MODEL_HPP
#define ERICS3DENGINE_MODEL_HPP

// Code taken from https://github.com/JoeyDeVries/LearnOpenGL/blob/master/includes/learnopengl/model.h
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../ThirdParty/stb_image/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Program.hpp"
#include "Mesh.hpp"

class Model {
    public:
        Model(const string & path, bool gamma = false);
        void render(Program * program);

    private:
        vector<MeshTexture> mTexturesLoaded;
        vector<Mesh> mMeshes;
        string mDirectory;
        bool mGammaCorrection;

        void loadModel(string path);
        void processNode(aiNode * node, const aiScene * scene);
        Mesh processMesh(aiMesh * mesh, const aiScene * scene);
        vector<MeshTexture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName);
        GLuint textureFromFile(const char * path, string directory, bool gamma = false);
};

#endif //ERICS3DENGINE_MODEL_HPP