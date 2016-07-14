//
// Created by max on 13.07.16.
//

#ifndef SDL2_CUBE_UTILS_H
#define SDL2_CUBE_UTILS_H

#include <SDL2/SDL_opengl.h>

void draw_text(TTF_Font *font) {
    // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
    SDL_Color White = {255, 255, 255};

    // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    SDL_Surface *surfaceMessage = TTF_RenderText_Blended(font, "put your text here", White);

    int w = 0;
    int h = 0;

    if (TTF_SizeText(font, "SDL_ttf is awesome!", &w, &h) == -1) {
        std::cout << "Font size error: " << SDL_GetError() << std::endl;
    }

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surfaceMessage->w, surfaceMessage->h, 0, GL_BGRA, GL_UNSIGNED_BYTE,
                 surfaceMessage->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(surfaceMessage);

    glEnable(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    const GLfloat quadVertices[] = { -1.0f, 1.0f, 0.0f,
                                     1.0f, 1.0f, 0.0f,
                                     1.0f,-1.0f, 0.0f,
                                     -1.0f,-1.0f, 0.0f
    };

    glVertexPointer(3, GL_FLOAT, 0, quadVertices);
    glDrawArrays(GL_QUADS, 0, 4);

    glDisable(GL_TEXTURE_2D);
    // TTF_CloseFont(font);
}

// Convert an SDL_Surface to SDL_Texture. We've done this before, so I'll keep it short
SDL_Texture *SurfaceToTexture(SDL_Renderer *renderer, SDL_Surface *surf) {
    SDL_Texture *text;

    text = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_FreeSurface(surf);

    return text;
}

#endif //SDL2_CUBE_UTILS_H
