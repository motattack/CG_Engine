#ifndef CG_ENGINE_TEXTURE_H
#define CG_ENGINE_TEXTURE_H

#include <assimp/scene.h>
#include <GL/glew.h>
#include <iostream>
#include <utility>

#include <stb_image.h>

class Texture {
public:
    Texture(std::string dir, std::string path, aiTextureType type)
            : dir(std::move(dir)), path(std::move(path)), type(type) {
        generate();
    }

    void generate() {
        glGenTextures(1, &id);
    };

    void load(bool flip = true) const {
        stbi_set_flip_vertically_on_load(flip);

        int width, height, nChannels;

        unsigned char *data = stbi_load((dir + "/" + path).c_str(), &width, &height, &nChannels, 0);

        GLenum colorMode = GL_RGB;
        switch (nChannels) {
            case 1:
                colorMode = GL_RED;
                break;
            case 4:
                colorMode = GL_RGBA;
                break;
        };

        if (data) {
            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        } else {
            std::cout << "Image not loaded at " << path << std::endl;
        }

        stbi_image_free(data);
    };

    void bind() const {
        glBindTexture(GL_TEXTURE_2D, id);
    };

    // texture object
    unsigned int id{};
    aiTextureType type;
    std::string dir;
    std::string path;
};

#endif //CG_ENGINE_TEXTURE_H
