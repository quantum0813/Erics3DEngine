#include "Sphere.hpp"
#include <cmath>

// Sphere creation code taken from: http://stackoverflow.com/questions/5988686/creating-a-3d-sphere-in-opengl-using-visual-c
Sphere::Sphere(float radius, unsigned int slices, unsigned int stacks) : mRadius(radius), mSlices(slices), mStacks(stacks) {
    double startLatRadians = 0;
    double endLatRadians = M_PI;
    double startLongRadians = 0;
    double endLongRadians = M_PI * 2;

    double latRange = endLatRadians - startLatRadians;
    double longRange = endLongRadians - startLongRadians;

    unsigned int numVertices = (slices + 1) * (stacks + 1);

    mVertexData.resize(numVertices * 3);
    mNormalData.resize(numVertices * 3);
    mUVData.resize(numVertices * 2);
    mIndexData.resize(slices * stacks * 2 * 3);
    std::vector<GLfloat>::iterator verticesIt = mVertexData.begin();
    std::vector<GLfloat>::iterator normalsIt = mNormalData.begin();
    std::vector<GLfloat>::iterator texcoordsIt = mUVData.begin();
    std::vector<GLushort>::iterator indicesIt = mIndexData.begin();
    for (int y = 0; y <= stacks; y++) {
        for (int x = 0; x <= slices; x++) {
            // Generate a vertex based on its spherical coordinates
            double u = (double)x / slices;
            double v = (double)y / stacks;
            double theta = longRange * u;
            double phi = latRange * v;
            double sinTheta = sin(theta);
            double cosTheta = cos(theta);
            double sinPhi = sin(phi);
            double cosPhi = cos(phi);
            double ux = cosTheta * sinPhi;
            double uy = cosPhi;
            double uz = sinTheta * sinPhi;

            *verticesIt++ = radius * ux;
            *verticesIt++ = radius * uy;
            *verticesIt++ = radius * uz;

            *normalsIt++ = ux;
            *normalsIt++ = uy;
            *normalsIt++ = uz;

            *texcoordsIt++ = 1 - u;
            *texcoordsIt++ = v;
        }
    }

    int numVertsAround = slices + 1;
    for (int x = 0; x < slices; x++) {
        for (int y = 0; y < stacks; y++) {
            // Make triangle 1 of quad
            *indicesIt++ = (y + 0) * numVertsAround + x;
            *indicesIt++ = (y + 0) * numVertsAround + x + 1;
            *indicesIt++ = (y + 1) * numVertsAround + x;

            // Make triangle 2 of quad
            *indicesIt++ = (y + 1) * numVertsAround + x;
            *indicesIt++ = (y + 0) * numVertsAround + x + 1;
            *indicesIt++ = (y + 1) * numVertsAround + x + 1;
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

Sphere::Sphere(float radius, unsigned int slices, unsigned int stacks, float startLatRadians, float endLatRadians, float startLongRadians, float endLongRadians) {

}

void Sphere::render() {
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

Sphere::~Sphere() {
    glDeleteVertexArrays(1, &mVertexArrayId);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mNormalBuffer);
    glDeleteBuffers(1, &mUVBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
}