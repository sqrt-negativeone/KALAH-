#pragma once

#include "Menu.h"
#include "GameController.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define KALAH 6

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

//The window we'll be rendering to
SDL_Window* window;

//The window renderer
SDL_Renderer* renderer;

//text input holder
char* textInput;
//Globally used font
TTF_Font* font;
SDL_Rect* default_container;


MenuStack* q;

GameController* gameController;
GameController* tmpGameController;
