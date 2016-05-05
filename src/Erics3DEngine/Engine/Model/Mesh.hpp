#ifndef ERICS3DENGINE_MESH_HPP
#define ERICS3DENGINE_MESH_HPP

// Code taken from https://github.com/JoeyDeVries/LearnOpenGL/blob/master/includes/learnopengl/mesh.h
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/types.h>
#include "../Program.hpp"

struct Vertex {
    // Position
    glm::vec3 position;
    // Normal
    glm::vec3 normal;
    // Texcoord
    glm::vec2 texcoords;
    // Tangent
    glm::vec3 tangent;
    // Bitangent
    glm::vec3 bitangent;
};

struct MeshTexture {
    GLuint id;
    string type;
    aiString path;
};

class Mesh {
    public:
        Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<MeshTexture> textures);
        void render(Program * program);

    private:
        // Mesh data
        vector<Vertex> mVertices;
        vector<GLuint> mIndices;
        vector<MeshTexture> mTextures;
        GLuint mVAO;

        GLuint mVBO;
        GLuint mEBO;

        void setupMesh();
};

#endif //ERICS3DENGINE_MESH_HPP
