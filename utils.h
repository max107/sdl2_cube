#ifndef SDL2_CUBE_UTILS
#define SDL2_CUBE_UTILS

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <iostream>

GLuint loadTexture(std::string path) {
    // Load an OpenGL texture.
    // We could directly use a sf::Texture as an OpenGL texture (with its Bind() member function),
    // but here we want more control on it (generate mipmaps, ...) so we create a new one from an image
    GLuint texture = 0;

    // You should probably use CSurface::OnLoad ... ;)
    //-- and make sure the Surface pointer is good!
    SDL_Surface *surf = IMG_Load(path.c_str());

    int imageMode = GL_RGB;
    if (surf->format->BytesPerPixel == 4) {
        imageMode = GL_RGBA;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, imageMode, surf->w, surf->h, 0, imageMode, GL_UNSIGNED_BYTE,
                 surf->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    return texture;
}

#endif
