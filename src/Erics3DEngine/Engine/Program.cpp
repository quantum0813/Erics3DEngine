#include "Program.hpp"

Program::Program(std::vector<Shader> shaders) : mShaders(shaders) {
    if (mShaders.size() == 0)
        throw Exception("A program must contain at least 1 shader.");
}

Program::~Program() {}

bool Program::link() {
    mProgramHandle = glCreateProgram();

    if (!mProgramHandle)
        throw Exception("Error creating program handle");

    for (std::vector<Shader>::iterator it = mShaders.begin(); it != mShaders.end(); it++)
        glAttachShader(mProgramHandle, it->getShaderHandle());

    glLinkProgram(mProgramHandle);

    // Now check for errors
    GLint result = GL_FALSE;
    int infoLogLength;

    glGetProgramiv(mProgramHandle, GL_LINK_STATUS, &result);
    glGetProgramiv(mProgramHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (result == GL_FALSE) {
        if (infoLogLength > 0) {
            std::vector<char> errorMsg(infoLogLength + 1);
            glGetProgramInfoLog(mProgramHandle, infoLogLength, nullptr, &errorMsg[0]);
            printf("Error linking program: \n%s\n", &errorMsg[0]);
            return false;
        } else {
            printf("Error linking program\n");
            return false;
        }
    }

    for (std::vector<Shader>::iterator it = mShaders.begin(); it != mShaders.end(); it++) {
        glDetachShader(mProgramHandle, it->getShaderHandle());
        glDeleteShader(it->getShaderHandle());
    }

    return true;
}

void Program::use() {
    glUseProgram(mProgramHandle);
}

GLuint Program::getProgramHandle() {
    return mProgramHandle;
}

GLint Program::getAttribLocation(std::string attrib) {
    return glGetAttribLocation(mProgramHandle, attrib.c_str());
}

GLint Program::getUniformLocation(std::string uniform) {
    return glGetUniformLocation(mProgramHandle, uniform.c_str());
}

std::vector<Shader> & Program::getShaders() {
    return mShaders;
}