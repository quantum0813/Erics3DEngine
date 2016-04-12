#ifndef ERICS3DENGINE_TEXTURE_HPP
#define ERICS3DENGINE_TEXTURE_HPP

#include <string>
#include <GL/glew.h>
#include <map>

class Texture {
    public:
        Texture(/*GLenum target, */std::string filename/*, bool hasAlpha*/);
        ~Texture();
        void bind();
        /*GLuint getTextureId();
        GLenum getTextureUnit();
        void setTarget(GLenum target);
        void setMipmapLevel(GLint level);
        void setInternalFormat(GLenum format);
        bool load(std::string filename);
        void use();
        void unuse();

        static const int MAX_TEXTURE_UNITS = 32;
        static const std::map<GLenum, bool> usedTextures;
        static GLenum getNextUnusedTextureUnit();*/

    private:
        GLuint mTexture;
        /*GLenum mTarget = GL_INVALID_ENUM;
        GLint mMipmapLevel = 0;
        GLenum mInternalFormat;
        std::string mFilename;
        GLuint mTextureId;
        GLenum mTextureUnit;*/
};

#endif //ERICS3DENGINE_TEXTURE_HPP