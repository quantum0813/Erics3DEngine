#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <GL/gl.h>
#include <GL/glew.h>
#include <iostream>
#include <vector>

namespace Util {
    static void printFirstNDebugMessages(GLuint numMsgs) {
        std::vector<std::string> msgs(numMsgs);
        GLint maxMsgLen = 0;
        glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMsgLen);

        std::vector<GLchar> msgData(numMsgs * maxMsgLen);
        std::vector<GLenum> sources(numMsgs);
        std::vector<GLenum> types(numMsgs);
        std::vector<GLenum> severities(numMsgs);
        std::vector<GLuint> ids(numMsgs);
        std::vector<GLsizei> lengths(numMsgs);

        GLuint numFound = glGetDebugMessageLog(numMsgs, 10000, &sources[0], &types[0], &ids[0], &severities[0], &lengths[0], &msgData[0]);

        sources.resize(numFound);
        types.resize(numFound);
        severities.resize(numFound);
        ids.resize(numFound);
        lengths.resize(numFound);

        std::vector<std::string> messages;
        messages.reserve(numFound);

        std::vector<GLchar>::iterator currPos = msgData.begin();
        for(size_t msg = 0; msg < lengths.size(); ++msg) {
            messages.push_back(std::string(currPos, currPos + lengths[msg] - 1));
            currPos = currPos + lengths[msg];
        }

        std::cout << std::endl;
        std::cout << "===============DEBUG===============" << std::endl;
        for (std::string msg : messages)
            std::cout << msg << std::endl;
        std::cout << "===================================" << std::endl;
        std::cout << std::endl;
    }
}

#endif //DEBUG_HPP
