#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

typedef enum{false,true} bool;

typedef enum{
    FIRST_FRAME=0,
    LOAD_GAME_MENU=1,
    NEW_GAME_MENU=2,
    CHOOSE_FIRST_PLAYER=3,
    CHOOSE_SECOND_PLAYER=4,
    CHOOSE_NUMBER_OF_PIECES=5,
    HOME_MENU=6,
    MAIN_GAME=7,
    WINNER=8,
    LOADING
} MenuType;


typedef enum{
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

typedef struct {
    char name[20];
    int board[7];
} Player;

typedef struct  {
    int player_turn;
    int is_last_move_at_kalah,is_last_move_at_empty, is_all_empty;
} state;

typedef struct stack {
    int numb_games;
    state* s;
    Player* player[2];
    struct stack* prec;
} stack;

typedef struct{
    char name[20];
    int numb_pieces;
    stack* game;
} API;

typedef struct{
    SDL_Texture* texture;
    char* src;
    int w,h;
} Texture;

typedef struct{
    SDL_Rect* container;
    Texture* texture;
    ButtonType t;
} Button;




typedef struct {
    MenuType type;
    Texture* background;
    int numberOfButtons;
    Button* buttons;
} Menu;

typedef struct MenuStack MenuStack;

struct MenuStack{
    Menu* top;
    MenuStack* next;
};

int SCREEN_WIDTH ;
int SCREEN_HEIGHT ;



//The window we'll be rendering to
SDL_Window* window;

//The window renderer
SDL_Renderer* renderer ;

//text input holder
char* textInput;
//Globally used font
TTF_Font *font ;
SDL_Rect* default_container;


//The game API
API* api;
API* tmpAPI;
// rander flag
bool render;

//The Menu stack
MenuStack* q;

//initializing game
bool init(); //done

// loadings and savings
void loadAPI(char* name); //done
void saveAPI(); //done

//Game logic
void play(int move); //done

//handling mouse 
int isMouseInsideContainer(SDL_Rect* container); //done
void handleClick(Button* button); //done

//handling Menus
Menu* createMenu(MenuType type);//done
void destroyMenu(Menu* menu);//done
void renderMenu(Menu* menu);//done?


//Menus Queue handling
void insert(Menu *menu);
void delete();
void empty();

//deallocate memory
void freeAPI();//done
void quit();


//Texture utilities
SDL_Texture* loadTexture( char* path ); //done
Texture* LTextureText( char* textureText, SDL_Color textColor ); //done
void loadFont(char* src,int size);//done
void destroyTexture(Texture* texture);
//input text manupulations
char* deleteChar(char* str);
char* addChar(char* str,char c);

//rendering pile and kalah pieces
void renderPieces(SDL_Rect* container,int nombrePieces);


