#include "Mesh.hpp"

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<MeshTexture> textures) : mVertices(vertices), mIndices(indices), mTextures(textures) {
    this->setupMesh();
}

void Mesh::render(Program * program) {
    // Bind appropriate textures
    GLuint diffuseNum = 1;
    GLuint specularNum = 1;
    GLuint normalNum = 1;
    GLuint heightNum = 1;
    for (GLuint i = 0; i < this->mTextures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
        // Retrieve texture number (the N in diffuse_textureN)
        stringstream ss;
        string number;
        string name = this->mTextures[i].type;
        if (name == "texture_diffuse")
            ss << diffuseNum++;
        else if (name == "texture_specular")
            ss << specularNum++;
        else if (name == "tetxure_normal")
            ss << normalNum++;
        else if (name == "texture_height")
            ss << heightNum++;
        number = ss.str();
        // Now set the sampler to the correct texture unit
        glUniform1i(program->getUniformLocation(name + number), i);
        // And finally bind the texture
        glBindTexture(GL_TEXTURE_2D, this->mTextures[i].id);
    }

    // Draw the mesh
    glBindVertexArray(this->mVAO);
    glDrawElements(GL_TRIANGLES, this->mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Always good practice to set everything back to defaults once configured.
    for (GLuint i = 0; i < mTextures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Mesh::setupMesh() {
    // Create buffers/arrays
    glGenVertexArrays(1, &this->mVAO);
    glGenBuffers(1, &this->mVBO);
    glGenBuffers(1, &this->mEBO);

    glBindVertexArray(this->mVAO);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->mVBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, this->mVertices.size() * sizeof(Vertex), &this->mVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mIndices.size() * sizeof(GLuint), &this->mIndices[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, texcoords));
    // Vertex Tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, tangent));
    // Vertex Bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
}