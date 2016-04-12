#include "Plane.hpp"

Plane::Plane(float width, float depth, unsigned int subdivWidth, unsigned int subdivDepth) : mWidth(width), mDepth(depth), mSubdivWidth(subdivWidth), mSubdivDepth(subdivDepth) {
    unsigned int numVertices = (subdivWidth + 1) * (subdivDepth + 1);
    mVertexData.resize(numVertices * 3);
    mNormalData.resize(numVertices * 3);
    mUVData.resize(numVertices * 2);
    mIndexData.resize(subdivWidth * subdivDepth * 3 * 2);
    std::vector<GLfloat>::iterator verticesIt = mVertexData.begin();
    std::vector<GLfloat>::iterator normalsIt = mNormalData.begin();
    std::vector<GLfloat>::iterator texcoordsIt = mUVData.begin();
    std::vector<GLushort>::iterator indicesIt = mIndexData.begin();
    for (int z = 0; z <= subdivDepth; z++) {
        for (int x = 0; x <= subdivWidth; x++) {
            double u = (double)x / subdivWidth;
            double v = (double)z / subdivDepth;
            *verticesIt++ = width * u - width * 0.5;
            *verticesIt++ = 0;
            *verticesIt++ = depth * v - depth * 0.5;

            *normalsIt++ = 0;
            *normalsIt++ = 1;
            *normalsIt++ = 0;

            *texcoordsIt++ = u;
            *texcoordsIt++ = v;
        }
    }

    unsigned int numVertsAcross = subdivWidth + 1;

    for (int z = 0; z < subdivDepth; z++) {
        for (int x = 0; x < subdivWidth; x++) {
            // Make triangle 1 of quad
            *indicesIt++ = (z + 0) * numVertsAcross + x;
            *indicesIt++ = (z + 1) * numVertsAcross + x;
            *indicesIt++ = (z + 0) * numVertsAcross + x + 1;

            // Make triangle 2 of quad
            *indicesIt++ = (z + 1) * numVertsAcross + x;
            *indicesIt++ = (z + 1) * numVertsAcross + x + 1;
            *indicesIt++ = (z + 0) * numVertsAcross + x + 1;
        }
    }

    glGenVertexArrays(1, &mVertexArrayId);
    glBindVertexArray(mVertexArrayId);

    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(GLfloat), &mVertexData[0], GL_STATIC_DRAW);

    glGenBuffers(1, &mNormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, mNormalData.size() * sizeof(GLfloat), &mNormalData[0], GL_STATIC_DRAW);

    glGenBuffers(1, &mUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, mUVData.size() * sizeof(GLfloat), &mUVData[0], GL_STATIC_DRAW);

    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexData.size() * sizeof(GLushort), &mIndexData[0], GL_STATIC_DRAW);
}

void Plane::render() {
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
    glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
    glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void *)0
    );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, mUVBuffer);
    glVertexAttribPointer(
            2,
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
    glDisableVertexAttribArray(2);
}

Plane::~Plane() {
    glDeleteVertexArrays(1, &mVertexArrayId);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mNormalBuffer);
    glDeleteBuffers(1, &mUVBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
}