// src/Texture.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Texture.h"
#include <GL/freeglut.h>
#include <cstdio>

unsigned int texture_load(const char* path) {
    // OpenGL expects row 0 at the bottom; image files store row 0 at the top.
    stbi_set_flip_vertically_on_load(true);

    int width, height, sourceChannels;
    unsigned char* data = stbi_load(path, &width, &height, &sourceChannels, 4); // force RGBA

    if (!data) {
        std::fprintf(stderr, "texture_load: failed to load '%s'\n", path);
        return 0;
    }

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Single mip level (no glGenerateMipmap — GL 3.0+, needs GLEW/GLAD;
    // no gluBuild2DMipmaps — not declared in this toolchain's GLU header).
    // Plain bilinear filtering is fully correct for 3D, just slightly more
    // shimmer on distant floor tiles than mipmapping would give.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    return textureId;
}