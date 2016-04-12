#ifndef ERICS3DENGINE_SPHERE_HPP
#define ERICS3DENGINE_SPHERE_HPP

#include "Shape.hpp"
#include <GLFW/glfw3.h>
#include <vector>

class Sphere : public Shape {
    public:
        Sphere(float radius, unsigned int slices, unsigned int stacks);
        Sphere(float radius, unsigned int slices, unsigned int stacks, float startLatRadians, float endLatRadians, float startLongRadians, float endLongRadians);
        ~Sphere();

        void render();

    private:
        std::vector<GLfloat> mVertexData;
        std::vector<GLfloat> mNormalData;
        std::vector<GLfloat> mUVData;
        std::vector<GLushort> mIndexData;

        float mRadius;
        unsigned int mSlices;
        unsigned int mStacks;
        float mStartLatRadius;
        float mEndLatRadius;
        float mStartLongRadius;
        float mEndLongRadius;
};

#endif //ERICS3DENGINE_SPHERE_HPP