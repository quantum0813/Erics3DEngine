#ifndef ERICS3DENGINE_CUBE_HPP
#define ERICS3DENGINE_CUBE_HPP

#include "Shape.hpp"
#include <GLFW/glfw3.h>
#include <vector>

class Cube : public Shape {
    public:
        Cube(float size);
        ~Cube();

        void render();

    private:
        std::vector<GLfloat> mVertexData;
        std::vector<GLfloat> mUVData;
        std::vector<GLushort> mIndexData;
        float mSize;
};

#endif //ERICS3DENGINE_CUBE_HPP