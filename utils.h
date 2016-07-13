//
// Created by max on 13.07.16.
//

#ifndef SDL2_CUBE_UTILS_H
#define SDL2_CUBE_UTILS_H

#include <SDL2/SDL_ttf.h>

void draw_text(SDL_Renderer * renderer) {
    //this opens a font style and sets a size
    TTF_Font* Sans = TTF_OpenFont("Ubuntu-R.ttf", 24);

    // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
    SDL_Color White = {255, 255, 255};

    // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "put your text here", White);

    //now you can convert it into a texture
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 10;  //controls the rect's x coordinate
    Message_rect.y = 10; // controls the rect's y coordinte
    Message_rect.w = 100; // controls the width of the rect
    Message_rect.h = 100; // controls the height of the rect

    //Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understance
    //Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

    //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
}

#endif //SDL2_CUBE_UTILS_H
