#ifndef CG_ENGINE_FRAMEBUFFER_H
#define CG_ENGINE_FRAMEBUFFER_H

#include <GL/glew.h>
#include <iostream>

class FrameBuffer {
private:
    GLuint textureID;
    GLuint frameBuffer;
    GLuint RenderBuffer;
public:
    FrameBuffer() : frameBuffer(0), RenderBuffer(0), textureID(0) {}

    FrameBuffer(GLsizei width, GLsizei height) {
        glGenTextures(1, &textureID);
        glGenFramebuffers(1, &frameBuffer);
        glGenRenderbuffers(1, &RenderBuffer);
        setSize(width, height);
    }

    ~FrameBuffer() {
        glDeleteRenderbuffers(1, &RenderBuffer);
        glDeleteFramebuffers(1, &frameBuffer);
        glDeleteTextures(1, &textureID);
    }

    void setSize(GLsizei width, GLsizei height) {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindRenderbuffer(GL_RENDERBUFFER, RenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderBuffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Error:: fragment buff NOT COMPLETE!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    }

    void unBind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void clear() {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    GLuint getTextureID() const {
        return textureID;
    }
};


#endif //CG_ENGINE_FRAMEBUFFER_H
