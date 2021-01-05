#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


typedef struct
{
    SDL_Texture* texture;
    char* src;
    int w, h;
} Texture;


SDL_Texture* loadTexture(char* path);
Texture* LTextureText(char* textureText, SDL_Color textColor);

void loadFont(char* src, int size);
void destroyTexture(Texture* texture);