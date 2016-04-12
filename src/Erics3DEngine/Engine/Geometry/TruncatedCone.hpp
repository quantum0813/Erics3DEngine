#ifndef ERICS3DENGINE_TRUNCATEDCONE_HPP
#define ERICS3DENGINE_TRUNCATEDCONE_HPP

#include "Shape.hpp"
#include <GLFW/glfw3.h>
#include <vector>

class TruncatedCone : public Shape {
    public:
        TruncatedCone(float bottomRadius, float topRadius, float height, unsigned int radialSubdiv, unsigned int verticalSubdiv, bool topCap = true, bool bottomCap = true);
        ~TruncatedCone();

        void render();

    private:
        std::vector<GLfloat> mVertexData;
        std::vector<GLfloat> mNormalData;
        std::vector<GLfloat> mUVData;
        std::vector<GLushort> mIndexData;

        float mBottomRadius;
        float mTopRadius;
        float mHeight;
        unsigned int mRadialSubdiv;
        unsigned int mVerticalSubdiv;
        bool mTopCap;
        bool mBottomCap;
};

#endif //ERICS3DENGINE_TRUNCATEDCONE_HPP