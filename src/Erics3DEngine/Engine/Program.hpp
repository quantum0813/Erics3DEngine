#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <GL/glew.h>
#include <vector>
#include <string>
#include "Shader.hpp"

class Program {
    public:
        Program(std::vector<Shader> shaders);
        ~Program();
        bool link();
        void use();
        GLuint getProgramHandle();
        GLint getAttribLocation(std::string attrib);
        GLint getUniformLocation(std::string uniform);
        std::vector<Shader> & getShaders();

    private:
        GLuint mProgramHandle;
        std::vector<Shader> mShaders;
};

#endif //PROGRAM_HPP