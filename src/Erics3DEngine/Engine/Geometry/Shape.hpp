#ifndef ERICS3DENGINE_SHAPE_HPP
#define ERICS3DENGINE_SHAPE_HPP

#include <GL/glew.h>

class Shape {
    public:
        Shape();
        virtual ~Shape();
        virtual void render() = 0;

    protected:
        GLuint mVertexArrayId;
        GLuint mVertexBuffer;
        GLuint mNormalBuffer;
        GLuint mUVBuffer;
        GLuint mIndexBuffer;
};

#endif //ERICS3DENGINE_SHAPE_HPP