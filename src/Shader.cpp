#include <vector>
#include "../include/Shader.hpp"

Shader::Shader(std::string filename, ShaderType shaderType) :
       mFilename(filename), mShaderType(shaderType), mShaderHandle(0), mCompiled(false) { }

/**
 * Delete the shader upon destruction of the class
 */
Shader::~Shader() {}

/**
 * This function attempts to compile a shader from the file path provided by mFilename
 * and the shader type provided by mShaderType
 */
bool Shader::compile() {
    std::ifstream file(mFilename, std::ios_base::in);
    if (!file.good())
        throw Exception("Specified file is invalid.");

    // Read the file into a string with a single line
    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const char * cStrSource = source.c_str();

    file.close();

    mShaderHandle = glCreateShader(mShaderType);

    // If glCreateShader fails for whatever reason, throw an exception
    if (!mShaderHandle)
        throw Exception("Failed to create shader.");

    // Set the shader source to the file we read into the string above
    glShaderSource(mShaderHandle, 1, &cStrSource, nullptr);

    // Compile the shader
    glCompileShader(mShaderHandle);

    // Now check for errors
    GLint result = GL_FALSE;
    int infoLogLength;

    glGetShaderiv(mShaderHandle, GL_COMPILE_STATUS, &result);
    glGetShaderiv(mShaderHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (result == GL_FALSE) {
        if (infoLogLength > 0) {
            std::vector<char> errorMsg(infoLogLength + 1);
            std::string filename;
            size_t idx = mFilename.find_last_of('/');
            if (idx != std::string::npos)
                filename = mFilename.substr(idx + 1);
            else
                filename = mFilename;
            glGetShaderInfoLog(mShaderHandle, infoLogLength, nullptr, &errorMsg[0]);
            printf("Error compiling shader '%s': %s\n", filename.c_str(), &errorMsg[0]);
            return false;
        } else {
            std::string filename;
            size_t idx = mFilename.find_last_of('/');
            if (idx != std::string::npos)
                filename = mFilename.substr(idx + 1);
            else
                filename = mFilename;
            printf("Error compiling shader '%s'\n", filename.c_str());
            return false;
        }
    }

    return true;
}

GLuint Shader::getShaderHandle() {
    return mShaderHandle;
}

Shader::ShaderType Shader::getShaderType() {
    return mShaderType;
}