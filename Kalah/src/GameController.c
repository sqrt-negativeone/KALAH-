#include "GameController.h"
#include "Global.h"


//-------------------------------------------------save------------------------------------------
void savePlayer(Player* p, FILE* file)
{
    int len = strlen(p->name); p->name[len] = '\0'; len++;
    fwrite(&len, sizeof(int), 1, file);
    fwrite(p->name, sizeof(char), len, file);
    fwrite(p->board, sizeof(int), 7, file);
}
void saveState(state* s, FILE* file)
{
    fwrite(&(s->is_all_empty), sizeof(int), 1, file);
    fwrite(&(s->is_last_move_at_empty), sizeof(int), 1, file);
    fwrite(&(s->is_last_move_at_kalah), sizeof(int), 1, file);
    fwrite(&(s->player_turn), sizeof(int), 1, file);
}
void saveGameController()
{

    char dir[50] = "saves\\";
    strcat(dir, gameController->name);

    FILE* file = fopen(dir, "wb");

    if (file == NULL)
    {
        system("mkdir saves");
        file = fopen(dir, "wb");
        if (file == NULL)
        {
            printf("Couldn't save\n");
            return;
        }
    }

    int len = strlen(gameController->name);
    gameController->name[len] = '\0'; len++;
    fwrite(&len, sizeof(int), 1, file);
    fwrite(gameController->name, sizeof(char), 20, file);
    fwrite(&(gameController->numb_pieces), sizeof(int), 1, file);
    stack* game = gameController->game;
    fwrite(&(game->numb_games), sizeof(int), 1, file);
    while (game != NULL)
    {
        saveState(game->s, file);
        savePlayer(game->player[0], file);
        savePlayer(game->player[1], file);
        game = game->prec;
    }
    fclose(file);
}
//-----------------------------------------------------------------------------------------------

//----------------------------------------------load---------------------------------------------
Player* loadPlayer(FILE* file)
{
    Player* p = (Player*)malloc(sizeof(Player));
    int len;
    fread(&len, sizeof(int), 1, file);
    fread(p->name, sizeof(char), len, file);
    fread(p->board, sizeof(int), 7, file);
    return p;
}
state* loadState(FILE* file)
{
    state* s = (state*)malloc(sizeof(state));
    fread(&(s->is_all_empty), sizeof(int), 1, file);
    fread(&(s->is_last_move_at_empty), sizeof(int), 1, file);
    fread(&(s->is_last_move_at_kalah), sizeof(int), 1, file);
    fread(&(s->player_turn), sizeof(int), 1, file);
    return s;
}

void loadGameController(char* name)
{

    printf("Loading game... \n");
    char dir[50] = "saves\\";
    strcat(dir, name);

    FILE* file = fopen(dir, "rb");
    if (file == NULL)
    {
        printf("File not found..\n");
        return;
    }
    gameController = (GameController*)malloc(sizeof(gameController));

    int len;
    fread(&len, sizeof(int), 1, file);
    fread(gameController->name, sizeof(char), 20, file);
    fread(&(gameController->numb_pieces), sizeof(int), 1, file);
    int cnt;
    fread(&cnt, sizeof(int), 1, file);
    stack** games = malloc(cnt * sizeof(stack*));
    for (int i = 0; i < cnt; i++)
    {
        games[i] = (stack*)malloc(sizeof(stack));
        games[i]->numb_games = cnt - i;
        games[i]->s = loadState(file);
        games[i]->player[0] = loadPlayer(file);
        games[i]->player[1] = loadPlayer(file);
    }
    fclose(file);
    for (int i = 0; i < cnt - 1; i++)
    {
        games[i]->prec = games[i + 1];
    }
    games[cnt - 1]->prec = NULL;
    gameController->game = games[0];
    //free(games);
}
//-----------------------------------------------------------------------------------------------
void free_game(stack* game)
{
    if (game == NULL) return;
    free_game(game->prec);
    free(game->s);
    game->s = NULL;
    free(game);
}
void freeGameController()
{
    if (gameController == NULL) return;
    free_game(gameController->game);
    free(gameController);
}

bool init_app()
{
    //Initialization flag
    SCREEN_HEIGHT = 480;
    SCREEN_WIDTH = 640;

    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!\n");
        }

        //Create window
        window = SDL_CreateWindow("KALAH", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }


                //Initialize SDL_ttf
                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }

    textInput = "";
    gameController = NULL;
    q = NULL;
    tmpGameController = NULL;

    Menu* menu = createMenu(FIRST_FRAME);


    insert(menu);

    default_container = malloc(sizeof(SDL_Rect));
    default_container->x = default_container->y = 0;
    default_container->h = SCREEN_HEIGHT;
    default_container->w = SCREEN_WIDTH;


    return success;
}


void play(int move)
{
    //save the current API
    saveGameController();

    //get the current player
    int player = gameController->game->s->player_turn;
    //cancel play if it's on an empty pile
    if (gameController->game->player[player]->board[move] == 0) return;

    //push a new game to the stack
    stack* holder = gameController->game;
    gameController->game = malloc(sizeof(stack));

    gameController->game->prec = holder;
    gameController->game->numb_games = gameController->game->prec->numb_games + 1;
    gameController->game->player[0] = malloc(sizeof(Player));
    gameController->game->player[1] = malloc(sizeof(Player));

    strcpy(gameController->game->player[0]->name, gameController->game->prec->player[0]->name);
    strcpy(gameController->game->player[1]->name, gameController->game->prec->player[1]->name);

    gameController->game->s = malloc(sizeof(state));

    gameController->game->s->is_all_empty = gameController->game->s->is_last_move_at_empty = gameController->game->s->is_last_move_at_kalah = 0;
    gameController->game->s->player_turn = gameController->game->prec->s->player_turn;

    for (int i = 0; i < 7; i++)
    {
        gameController->game->player[0]->board[i] = gameController->game->prec->player[0]->board[i];
        gameController->game->player[1]->board[i] = gameController->game->prec->player[1]->board[i];
    }

    //play the turn
    int pieces = gameController->game->player[player]->board[move];
    gameController->game->player[player]->board[move] = 0;

    int currPlayer = player;
    while (pieces--)
    {
    ignore:
        move++;
        if (move == 7)
        {
            move = 0;
            currPlayer = 1 - currPlayer;
        }
        if (move == KALAH && currPlayer != player)
        {
            goto ignore;
        }
        gameController->game->player[currPlayer]->board[move]++;
    }

    //update the flag for the case if the last move at kalah
    if (move == KALAH) gameController->game->s->is_last_move_at_kalah = 1;

    //update the flag for the case if the last move was at an empty pile
    if (currPlayer == player && gameController->game->player[player]->board[move] == 1 && move != KALAH) gameController->game->s->is_last_move_at_empty = 1;

    int is_all_empty = 1;

    //update the next player turn
    if (gameController->game->s->is_last_move_at_kalah)
    {
        gameController->game->s->player_turn = player;
    }
    else
    {
        gameController->game->s->player_turn = 1 - player;
    }
    //handle the case if last move was at empty
    if (gameController->game->s->is_last_move_at_empty)
    {
        gameController->game->player[player]->board[KALAH] += 1 + gameController->game->player[1 - player]->board[5 - move];
        gameController->game->player[player]->board[move] = 0;
        gameController->game->player[1 - player]->board[5 - move] = 0;
    }
    //check for end game 
    for (int i = 0; i < 6 && is_all_empty; i++)
    {
        if (gameController->game->player[gameController->game->s->player_turn]->board[i] != 0) is_all_empty = 0;
    }
    gameController->game->s->is_all_empty = is_all_empty;
}

void quit()
{

    //destroy Menus
    empty();
    q = NULL;

    //free the API
    freeGameController();
    gameController = NULL;

    TTF_CloseFont(font);
    font = NULL;

    //Destroy window	
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

}