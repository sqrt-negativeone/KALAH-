#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Button.h"
#include "Bool.h"

typedef struct MenuStack MenuStack;

typedef enum
{
    FIRST_FRAME ,
    LOAD_GAME_MENU,
    NEW_GAME_MENU,
    CHOOSE_FIRST_PLAYER,
    CHOOSE_SECOND_PLAYER,
    CHOOSE_NUMBER_OF_PIECES,
    HOME_MENU,
    MAIN_GAME,
    WINNER,
    LOADING
} MenuType;

typedef struct
{
    MenuType type;
    Texture* background;
    int numberOfButtons;
    Button* buttons;
} Menu;

struct MenuStack
{
    Menu* top;
    MenuStack* next;
};

//handling Menus
Menu* createMenu(MenuType type);
void destroyMenu(Menu* menu);
void renderMenu(Menu* menu);

//Menus Queue handling
void insert(Menu* menu);
void delete();
void empty();
bool handleClick(Button* button);
bool handleButtonClickEvent(Menu* menu);

bool handleTextInputEvent(SDL_Event e);
