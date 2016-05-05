#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "Exception.hpp"

/**
 * A shader is a program that is run on the GPU. There are 4 main types of
 * shaders as defined in the ShaderType enum below. A group of shaders can be
 * linked together to form a program that can be used to shade pixels.
 */
class Shader {
    public:
        enum ShaderType {
            VERTEX = GL_VERTEX_SHADER,
            FRAGMENT = GL_FRAGMENT_SHADER,
            GEOMETRY = GL_GEOMETRY_SHADER,
            COMPUTE = GL_COMPUTE_SHADER,
            TESS_CONTROL = GL_TESS_CONTROL_SHADER,
            TESS_EVAL = GL_TESS_EVALUATION_SHADER
        };

    public:
        Shader(std::string filename, ShaderType shaderType);
        ~Shader();
        bool compile();
        GLuint getShaderHandle();
        ShaderType getShaderType();

    private:
        ShaderType mShaderType;
        std::string mFilename;
        GLuint mShaderHandle;
        bool mCompiled;
};

#endif //SHADER_HPP