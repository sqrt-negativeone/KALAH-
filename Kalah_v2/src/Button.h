#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Texture.h"

typedef enum
{
    PLAY,
    NEW_GAME_BUTTON,
    LOAD_GAME_BUTTON,
    NUMBER_OF_PIECES,
    UNDO,
    HOME,
    NEXT,
    PREVIOUS,
    PILE,
} ButtonType;

typedef struct
{
    SDL_Rect* container;
    Texture* texture;
    ButtonType t;
} Button;


