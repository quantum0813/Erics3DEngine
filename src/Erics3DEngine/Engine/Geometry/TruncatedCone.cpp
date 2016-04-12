#include "TruncatedCone.hpp"
#include <cmath>

TruncatedCone::TruncatedCone(float bottomRadius, float topRadius, float height, unsigned int radialSubdiv, unsigned int verticalSubdiv, bool topCap, bool bottomCap) :
    mBottomRadius(bottomRadius), mTopRadius(topRadius),
    mRadialSubdiv(radialSubdiv), mVerticalSubdiv(verticalSubdiv),
    mTopCap(topCap), mBottomCap(bottomCap) {
    unsigned int extra = (topCap ? 2 : 0) + (bottomCap ? 2 : 0);

    unsigned int numVertices = (radialSubdiv + 1) * (verticalSubdiv + 1 + extra);
    mVertexData.resize(numVertices * 3);
    mNormalData.resize(numVertices * 3);
    mUVData.resize(numVertices * 2);
    mIndexData.resize(radialSubdiv * (verticalSubdiv + extra) * 3 * 2);
    std::vector<GLfloat>::iterator verticesIt = mVertexData.begin();
    std::vector<GLfloat>::iterator normalsIt = mNormalData.begin();
    std::vector<GLfloat>::iterator texcoordsIt = mUVData.begin();
    std::vector<GLushort>::iterator indicesIt = mIndexData.begin();

    unsigned int vertsAroundEdge = radialSubdiv + 1;

    // The slant of the cone is constant across its surface
    double slant = atan2(bottomRadius - topRadius, height);
    double cosSlant = cos(slant);
    double sinSlant = sin(slant);

    int start = topCap ? -2 : 0;
    int end = verticalSubdiv + (bottomCap ? 2 : 0);

    for (int yy = start; yy <= end; ++yy) {
        double v = (double)yy / verticalSubdiv;
        double y = height * v;
        double ringRadius;
        if (yy < 0) {
            y = 0;
            v = 1;
            ringRadius = bottomRadius;
        } else if (yy > verticalSubdiv) {
            y = height;
            v = 1;
            ringRadius = topRadius;
        } else {
            ringRadius = bottomRadius + (topRadius - bottomRadius) * ((double)yy / verticalSubdiv);
        }
        y -= height / 2;
        for (int ii = 0; ii < vertsAroundEdge; ++ii) {
            double sinII = sin(ii * M_PI * 2 / radialSubdiv);
            double cosII = cos(ii * M_PI * 2 / radialSubdiv);
            *verticesIt++ = sinII * ringRadius;
            *verticesIt++ = y;
            *verticesIt++ = cosII * ringRadius;

            *normalsIt++ = (yy < 0 || yy > verticalSubdiv) ? 0 : (sinII * cosSlant);
            *normalsIt++ = (yy < 0) ? -1 : (yy > verticalSubdiv ? 1 : sinSlant);
            *normalsIt++ = (yy < 0 || yy > verticalSubdiv) ? 0 : (cosII * cosSlant);

            *texcoordsIt++ = ((double)ii / radialSubdiv);
            *texcoordsIt++ = 1 - v;
        }
    }

    for (int yy = 0; yy < verticalSubdiv + extra; ++yy) {
        for (int ii = 0; ii < radialSubdiv; ++ii) {
            *indicesIt++ = vertsAroundEdge * (yy + 0) + 0 + ii;
            *indicesIt++ = vertsAroundEdge * (yy + 0) + 1 + ii;
            *indicesIt++ = vertsAroundEdge * (yy + 1) + 1 + ii;
            *indicesIt++ = vertsAroundEdge * (yy + 0) + 0 + ii;
            *indicesIt++ = vertsAroundEdge * (yy + 1) + 1 + ii;
            *indicesIt++ = vertsAroundEdge * (yy + 1) + 0 + ii;
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

void TruncatedCone::render() {
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

TruncatedCone::~TruncatedCone() {
    glDeleteVertexArrays(1, &mVertexArrayId);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mNormalBuffer);
    glDeleteBuffers(1, &mUVBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
}