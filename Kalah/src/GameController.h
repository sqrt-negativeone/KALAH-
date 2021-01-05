#pragma once
#include "Bool.h"

typedef struct
{
    char name[20];
    int board[7];
} Player;

typedef struct
{
    int player_turn;
    int is_last_move_at_kalah, is_last_move_at_empty, is_all_empty;
} state;

typedef struct stack
{
    int numb_games;
    state* s;
    Player* player[2];
    struct stack* prec;
} stack;

typedef struct
{
    char name[20];
    int numb_pieces;
    stack* game;
} GameController;

void play(int move);

void loadGameController(char* name);
void saveGameController();
void freeGameController();
void quit();

bool init_app();