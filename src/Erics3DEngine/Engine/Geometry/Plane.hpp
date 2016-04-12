#ifndef ERICS3DENGINE_PLANE_HPP
#define ERICS3DENGINE_PLANE_HPP

#include "Shape.hpp"
#include <GLFW/glfw3.h>
#include <vector>

class Plane : public Shape {
    public:
        Plane(float width, float depth, unsigned int subdivWidth, unsigned int subdivDepth);
        ~Plane();

        void render();

    private:
        std::vector<GLfloat> mVertexData;
        std::vector<GLfloat> mNormalData;
        std::vector<GLfloat> mUVData;
        std::vector<GLushort> mIndexData;

        float mWidth;
        float mDepth;
        unsigned int mSubdivWidth;
        unsigned int mSubdivDepth;
};

#endif //ERICS3DENGINE_PLANE_HPP