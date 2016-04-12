#include "Cube.hpp"

Cube::Cube(float size) : mSize(size) {
    glGenVertexArrays(1, &mVertexArrayId);
    glBindVertexArray(mVertexArrayId);

    mVertexData = {
        // front
        -size, -size,  size,
        size, -size,  size,
        size,  size,  size,
        -size,  size,  size,
        // top
        -size,  size,  size,
        size,  size,  size,
        size,  size, -size,
        -size,  size, -size,
        // back
        size, -size, -size,
        -size, -size, -size,
        -size,  size, -size,
        size,  size, -size,
        // bottom
        -size, -size, -size,
        size, -size, -size,
        size, -size,  size,
        -size, -size,  size,
        // left
        -size, -size, -size,
        -size, -size,  size,
        -size,  size,  size,
        -size,  size, -size,
        // right
        size, -size,  size,
        size, -size, -size,
        size,  size, -size,
        size,  size,  size,
    };

    mUVData = {
        // Front
        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        // Back
        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        // Top
        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        // Bottom
        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        // Right
        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        // Left
        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
    };

    mIndexData = {
        // Front
        0,  1,  2,
        2,  3,  0,
        // Top
        4,  5,  6,
        6,  7,  4,
        // Back
        8,  9, 10,
        10, 11,  8,
        // Bottom
        12, 13, 14,
        14, 15, 12,
        // Left
        16, 17, 18,
        18, 19, 16,
        // Right
        20, 21, 22,
        22, 23, 20,
    };

    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(GLfloat), &mVertexData[0], GL_STATIC_DRAW);

    glGenBuffers(1, &mUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, mUVData.size() * sizeof(GLfloat), &mUVData[0], GL_STATIC_DRAW);

    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexData.size() * sizeof(GLushort), &mIndexData[0], GL_STATIC_DRAW);
}

void Cube::render() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void *)0
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, mUVBuffer);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void *)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glDrawElements(GL_TRIANGLES, mIndexData.size(), GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &mVertexArrayId);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mUVBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
}