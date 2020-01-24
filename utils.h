#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

typedef enum{false,true} bool;

typedef enum{
    FIRST_FRAME,
    LOAD_GAME_MENU,
    NEW_GAME_MENU,
    CHOOSE_FIRST_PLAYER,
    CHOOSE_SECOND_PLAYER,
    CHOOSE_NUMBER_OF_PIECES,
    HOME_MENU,
    MAIN_GAME,
    WINNER,
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
    SDL_Rect* container;
    Texture* texture;
    ButtonType t;
} Button;

typedef struct{
    SDL_Texture* texture;
    char* src;
} Texture;


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

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



//The window we'll be rendering to
SDL_Window* window = NULL;

//The window renderer
SDL_Renderer* renderer = NULL;

//text input holder
char* textInput;
//Globally used font
TTF_Font *font = NULL;
SDL_Rect* default_container;


//The game API
API* api;

//The Menu Queue
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
int isMenuStackEmpty(){ 
    return q==NULL; 
}
void insert(Menu *menu){
    MenuStack* newQ = malloc(sizeof(MenuStack));
    newQ->top=menu;
    newQ->next = q;
    q=newQ;
}
void delete(){
    if (q==NULL) return;
    MenuStack* tmp = q;
    destroyMenu(tmp->top);
    tmp->top=NULL;
    q=tmp->next;
    free(tmp);
    tmp=NULL;
}
void empty(){
    if (q==NULL) return;
    delete();
    empty();
}
//deallocate memory
void freeAPI();//done
void quit();

SDL_Texture* loadTexture( char* path ,int size); //done
Texture* LTextureText( char* textureText, SDL_Color textColor ); //done
void loadFont(char* src);//done
char* deleteChar(char* str);
char* addChar(char* str,char c);