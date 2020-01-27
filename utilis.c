#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define KALAH 6


//------------------------------------------play-------------------------------------------------
stack* newGame(){
    
    stack* game = (stack*) malloc(sizeof (stack));
    state* s = (state*) malloc (sizeof(state));
    game->numb_games=api->game->numb_games+1;
    s->player_turn=api->game->s->player_turn;
    game->s= s;
    game->player[0]=(Player*) malloc (sizeof(Player));
    game->player[1]=(Player*) malloc (sizeof(Player));
    strcpy(game->player[0]->name,api->game->player[0]->name);
    strcpy(game->player[1]->name,api->game->player[1]->name);
    for (int i=0 ; i<7 ;i++){
        game->player[0]->board[i]= api->game->player[0]->board[i];
        game->player[1]->board[i]= api->game->player[1]->board[i];
    }
    return game;
}


void play(int move){
    saveAPI();

    stack* game = newGame(api);
    
    int player = game->s->player_turn;

    int pieces = game->player[player]->board[move];
    game->player[player]->board[move]=0;
    int currPlayer=player;
    while (pieces--){
    ignore :
        move++;
        
        if (move==7){
            move=0;
            currPlayer=1-currPlayer;
        }
        if (move==KALAH && currPlayer!=player) {
            goto ignore;
        }
        game->player[currPlayer]->board[move] ++;
    }

    if (move == KALAH) game->s->is_last_move_at_kalah=1;
    if (currPlayer==player && game->player[player]->board[move]==1 && move != KALAH) game->s->is_last_move_at_empty=1;
    int is_all_empty=1;
    
    if (game->s->is_last_move_at_kalah){
        game->s->player_turn=player;
    }
    else {
        game->s->player_turn=1-player;
    }

    if (game->s->is_last_move_at_empty){
        game->player[player]->board[KALAH]+= 1 + game->player[1-player]->board[5-move];
        game->player[player]->board[move]=0;
        game->player[1-player]->board[5-move]=0;
    }
    for (int i=0 ; i<6 && is_all_empty ; i++){
        if (game->player[game->s->player_turn]->board[i]!=0) is_all_empty=0;
    }
    game->s->is_all_empty= is_all_empty;
    game->prec=api->game;
    api->game =game;
}

//-----------------------------------------------------------------------------------------------

//-------------------------------------------------save------------------------------------------
void savePlayer(Player* p , FILE* file){
    int len = strlen(p->name);
    fwrite(&len, sizeof(int), 1, file);
    fwrite(p->name,sizeof(char), len,file);
    fwrite(p->board,sizeof(int),7,file);
}
void saveState(state* s, FILE* file){
    fwrite(&(s->is_all_empty),sizeof(int),1,file);
    fwrite(&(s->is_last_move_at_empty),sizeof(int),1,file);
    fwrite(&(s->is_last_move_at_kalah),sizeof(int),1,file);
    fwrite(&(s->player_turn),sizeof(int),1,file);
}
void saveAPI(){

    char dir[]="saves\\";
    strcat(dir,api->name);
    strcat(dir,".bin");
    
    FILE* file= fopen(dir,"wb");

    if (file == NULL){
        system("mkdir saves");
        file = fopen(dir,"wb");
        if (file == NULL) {
            printf("couldn't save\n");
            return;
        }
    }

    int len = strlen(api->name);
    fwrite(&len,sizeof(int),1,file);
    fwrite(api->name,sizeof(char),len,file);
    fwrite(&(api->numb_pieces),sizeof(int),1,file);
    stack* game= api->game;
    fwrite(&(game->numb_games),sizeof(int),1,file);
    while (game!= NULL){
        saveState(game->s,file);
        savePlayer(game->player[0],file);
        savePlayer(game->player[1],file);
        game=game->prec;
    }
    fclose(file);
}
//-----------------------------------------------------------------------------------------------
//----------------------------------------------load---------------------------------------------
Player* loadPlayer(FILE* file){
    Player* p = (Player*) malloc(sizeof(Player));
    int len ;
    fread(&len, sizeof(int), 1 , file);
    fread(p->name, sizeof(char), len, file);
    fread(p->board, sizeof(int),7,file);
    return p;
}
state* loadState(FILE* file){
    state* s = (state*) malloc (sizeof(state));
    fread(&(s->is_all_empty),sizeof(int),1,file);
    fread(&(s->is_last_move_at_empty),sizeof(int),1,file);
    fread(&(s->is_last_move_at_kalah),sizeof(int),1,file);
    fread(&(s->player_turn),sizeof(int),1,file);
    return s;
}

void loadAPI(char* name){

    printf("loading game... \n");
    char dir[] = "saves\\";
    strcat(dir,name);
    strcat(dir,".bin");

    FILE* file = fopen(dir, "rb");

    if(file == NULL){
        printf("file not found..\n");
        return ;
    }
    api = (API*) malloc (sizeof(api));

    int len ;
    fread(&len,sizeof(int),1,file);
    fread(api->name,sizeof(char),len,file);
    fread(&(api->numb_pieces),sizeof(int),1,file);
    int cnt;
    fread(&cnt,sizeof(int),1,file);
    stack* games[cnt];
    for (int i=0 ; i<cnt ;i++){
        games[i]=(stack*) malloc(sizeof(stack));
        games[i]->numb_games=cnt-i;
        games[i]->s = loadState(file);
        games[i]->player[0]=loadPlayer(file);
        games[i]->player[1]=loadPlayer(file);
    }
    fclose(file);
    for (int i=0 ; i<cnt-1; i++){
        games[i]->prec= games[i+1];
    }
    games[cnt-1]->prec=NULL;
    api->game=games[0];
}
//-----------------------------------------------------------------------------------------------



//-------------------------------------handling mouse-------------------------------------------
int isMouseInsideContainer(SDL_Rect* container){
    int x,y;
    SDL_GetMouseState(&x,&y);

    return ((container->x < x && x< (container->x+container->w)) && (container->y < y && y< (container->y+container->h)));
}
void handleClick(Button* button){
    switch (button->t){

        case PLAY :{
            delete();
        }
        case HOME : {
            Menu* menu = createMenu(HOME_MENU);
            insert(menu);
            break;
        }
        case NEW_GAME_BUTTON :{
            Menu* menu = createMenu(NEW_GAME_MENU);
            insert(menu);
            break;
        }
        case LOAD_GAME_BUTTON :{
            Menu* menu = createMenu(LOAD_GAME_MENU);
            insert(menu); 
            break;
        }
        case UNDO :{
            if (api->game->prec!=NULL) api->game=api->game->prec;
            break;
        }
        case PILE: {
            int x;
            x= button->container->x;
            int p = button->container->y < SCREEN_HEIGHT/2;
            if (p!=api->game->s->player_turn) break;
            int move = 10*x/SCREEN_WIDTH;
            move-=2;

            play(move);
            break;
        }
        case NUMBER_OF_PIECES:{
            int x = button->container->x;
            int pieces= 5*x/SCREEN_WIDTH;
            pieces+=3;
            api->numb_pieces = pieces*12;
            Menu* menu = createMenu (CHOOSE_FIRST_PLAYER);
            insert(menu);
            break;
        }
        case NEXT:{
            switch (q->top->type){
                case NEW_GAME_MENU :  {
                    printf("hi\n");
                    if (textInput=="") break;
                    if (api==NULL){
                        printf(".\n");
                        api=malloc (sizeof(API));
                        api->game=malloc(sizeof(stack));
                        api->game->s=malloc(sizeof(state));
                        api->game->prec=NULL;
                        api->game->player[0]=malloc(sizeof(Player));
                        api->game->player[1]=malloc(sizeof(Player));
                    } 
                    srand(time(NULL));
                    api->game->s->player_turn = rand()%2; // chose a random player to start the game
                    api->game->s->is_all_empty=api->game->s->is_last_move_at_empty=api->game->s->is_last_move_at_kalah=0;
                    strcpy(api->name,textInput);
                    textInput="";
                    Menu* menu = createMenu(CHOOSE_NUMBER_OF_PIECES);
                    insert(menu);
                    printf("menu created\n");
                    break;
                }
                case CHOOSE_FIRST_PLAYER : {
                    if (textInput=="") break;
                    strcpy(api->game->player[0]->name,textInput);
                    for (int i=0; i<KALAH;i++) api->game->player[0]->board[i]=api->numb_pieces/12;
                    api->game->player[0]->board[KALAH]=0;
                    textInput="";
                    Menu* menu = createMenu(CHOOSE_SECOND_PLAYER);
                    insert(menu);
                    break;
                }
                case CHOOSE_SECOND_PLAYER :{
                    if (textInput=="") break;
                    strcpy(api->game->player[1]->name,textInput);
                    for (int i=0; i<KALAH;i++) api->game->player[1]->board[i]=api->numb_pieces/12;
                    api->game->player[1]->board[KALAH]=0;
                    textInput="";
                    Menu* menu = createMenu(MAIN_GAME);
                    insert(menu);
                    break;
                }
            }
            break;
        }
        case PREVIOUS :{
            delete();
        }
    }
}
//-----------------------------------------------------------------------------------------------
//---------------------------------------handleing Menus-----------------------------------------

Menu* createMenu(MenuType type){
    Menu* menu = malloc (sizeof(Menu));
    menu->type=type;
    Texture* background = malloc (sizeof(Texture));
    switch (type){
        case FIRST_FRAME :{

            background->src="images/homescreen/firstframe.jpg";
            background->texture=loadTexture(background->src);
            menu->background=background;

            Button* button = malloc (sizeof(Button));

            button->t = PLAY; // the button type
            button->container=malloc(sizeof(SDL_Rect));
            button->container->x=3*SCREEN_WIDTH/8;
            button->container->y=4*SCREEN_HEIGHT/5;
            button->container->h=SCREEN_HEIGHT/5;
            button->container->w=2*SCREEN_WIDTH/8;

            button->texture=malloc(sizeof(Texture));

            button->texture->src="images/homescreen/play.png";
            button->texture->texture=loadTexture(button->texture->src);

            menu->numberOfButtons=1;
            menu->buttons=button;

            break;
        }
        case LOAD_GAME_MENU :{
            background->src="images/loadgame/loadgame.jpg";
            background->texture=loadTexture(background->src);
            menu->background=background;
            
            Button* button = malloc(sizeof(Button));

            button->t = PREVIOUS; // the button type

            button->container=malloc(sizeof(SDL_Rect));
            button->container->x=0;
            button->container->y=2*SCREEN_HEIGHT/3;
            button->container->h=SCREEN_HEIGHT/3;
            button->container->w=SCREEN_WIDTH/8;
            
            button->texture=malloc (sizeof(Texture));
            button->texture->src="images/play/undo.png";
            button->texture->texture=loadTexture(button->texture->src);
            
            menu->numberOfButtons=1;
            menu->buttons=button;
            break;
        }
        case NEW_GAME_MENU :{
            background->src="images/newgame/gamename.jpg";
            background->texture=loadTexture(background->src);
            menu->background=background;

        }
        case CHOOSE_FIRST_PLAYER :{
            if (type == CHOOSE_FIRST_PLAYER){
                background->src="images/newgame/player1.jpg";
                background->texture=loadTexture(background->src);
                menu->background=background;
            }
            

            
        }
        case CHOOSE_SECOND_PLAYER : {
            if (type==CHOOSE_SECOND_PLAYER){
                background->src="images/newgame/player2.jpg";
                background->texture=loadTexture(background->src);
                menu->background=background;
            }

            Button* button = malloc(2*sizeof(Button));

            button[0].t = NEXT; // the button type
            button->container=malloc(sizeof(SDL_Rect));
            button->container->x=3*SCREEN_WIDTH/4;
            button->container->y=2*SCREEN_HEIGHT/3;
            button->container->h=SCREEN_HEIGHT/3;
            button->container->w=SCREEN_WIDTH/4;
            button->texture=malloc (sizeof(Texture));
            button->texture->src="images/newgame/next.png";
            button->texture->texture=loadTexture(button->texture->src);
            
            button[1].t=PREVIOUS;
            button[1].container=malloc(sizeof(SDL_Rect));
            button[1].container->x=0;
            button[1].container->y=2*SCREEN_HEIGHT/3;
            button[1].container->h=SCREEN_HEIGHT/3;
            button[1].container->w=SCREEN_WIDTH/4;
            button[1].texture=malloc (sizeof(Texture));
            button[1].texture->src="images/play/undo.png";
            button[1].texture->texture=loadTexture(button[1].texture->src);

            menu->numberOfButtons=2;
            menu->buttons=button;
            break;
        }
        case CHOOSE_NUMBER_OF_PIECES :{
            background->src="images/newgame/pieceschoice.jpg";
            background->texture=loadTexture(background->src);
            menu->background=background;

            Button* buttons = malloc(4*sizeof(Button));
            
            for (int i=0 ; i<3 ; i++){
                buttons[i].container = malloc (sizeof (SDL_Rect));
                buttons[i].texture = malloc(sizeof (Texture));
                buttons[i].t=NUMBER_OF_PIECES;
                
                buttons[i].container->y = SCREEN_HEIGHT/2;
                buttons[i].container->x = (i+1)*SCREEN_WIDTH/5;
                buttons[i].container->h=SCREEN_HEIGHT/5;
                buttons[i].container->w=SCREEN_WIDTH/5;
                
                switch (i){
                    case 0 :buttons[i].texture->src="images/newgame/4.png";break;
                    case 1 :buttons[i].texture->src="images/newgame/5.png";break;
                    case 2 :buttons[i].texture->src="images/newgame/6.png";break;
                }
                buttons[i].texture->texture=loadTexture(buttons[i].texture->src);
            }
            buttons[3].container = malloc (sizeof (SDL_Rect));
            buttons[3].texture = malloc(sizeof (Texture));
            buttons[3].t=PREVIOUS;

            buttons[3].container->x=0;
            buttons[3].container->y=2*SCREEN_HEIGHT/3;
            buttons[3].container->h=SCREEN_HEIGHT/3;
            buttons[3].container->w=SCREEN_WIDTH/4;

            buttons[3].texture->src="images/play/undo.png";
            buttons[3].texture->texture=loadTexture(buttons[3].texture->src);

            
            menu->numberOfButtons=4;
            menu->buttons=buttons;
            break;
        }
        case HOME_MENU :{
            background->src="images/background.jpg";
            background->texture=loadTexture(background->src);
            menu->background=background;
            Button* buttons;
            
            if (q==NULL ) buttons=(Button*) malloc (2*sizeof(Button));
            else buttons=(Button*) malloc (3*sizeof(Button));
            
            buttons[0].t= NEW_GAME_BUTTON;
            buttons[1].t= LOAD_GAME_BUTTON;

            if (!(q==NULL)) buttons[2].t=PREVIOUS;

            buttons[0].container = malloc (sizeof (SDL_Rect));
            buttons[0].texture = malloc(sizeof (Texture));
            buttons[0].container->x=3*SCREEN_WIDTH/10;
            buttons[0].container->y=1*SCREEN_HEIGHT/12;
            buttons[0].container->h=2*SCREEN_HEIGHT/6;
            buttons[0].container->w=2*SCREEN_WIDTH/5;
            
            buttons[0].texture->src= "images/homescreen/newgame.png";
            buttons[0].texture->texture=loadTexture(buttons[0].texture->src);

            buttons[1].container = malloc (sizeof (SDL_Rect));
            buttons[1].texture = malloc(sizeof (Texture));
            buttons[1].container->x=3*SCREEN_WIDTH/10;
            buttons[1].container->y=5*SCREEN_HEIGHT/12;
            buttons[1].container->h=2*SCREEN_HEIGHT/6;
            buttons[1].container->w=2*SCREEN_WIDTH/5;
            
            buttons[1].texture->src= "images/homescreen/loadgame.png";
            buttons[1].texture->texture=loadTexture(buttons[1].texture->src);

            if (!(q==NULL)){
                buttons[2].container = malloc (sizeof (SDL_Rect));
                buttons[2].texture = malloc(sizeof (Texture));
                buttons[2].container->x=3*SCREEN_WIDTH/10;
                buttons[2].container->y=9*SCREEN_HEIGHT/12;

                buttons[2].container->h=3*SCREEN_HEIGHT/12;
                buttons[2].container->w=2*SCREEN_WIDTH/5;
                
                buttons[2].texture->src="images/homescreen/reprendre.png";
                buttons[2].texture->texture=loadTexture(buttons[2].texture->src);
            }
            if (q==NULL ) menu->numberOfButtons=2;
            else menu->numberOfButtons=3;
            menu->buttons=buttons;
            break;
        }
        case MAIN_GAME:{
            background->src="images/play/game.jpg";
            background->texture=loadTexture(background->src);
            menu->background=background;
            Button* buttons = malloc (14*sizeof (Button));

            buttons[0].t = HOME;
            buttons[0].container = malloc (sizeof (SDL_Rect));
            
            buttons[0].container->x=35*SCREEN_WIDTH/40;
            buttons[0].container->y=SCREEN_HEIGHT/24;
            buttons[0].container->h=3*SCREEN_HEIGHT/24;
            buttons[0].container->w=3*SCREEN_WIDTH/24;
            
            buttons[0].texture = malloc(sizeof (Texture));
            buttons[0].texture->src="images/play/homebutton.png";
            buttons[0].texture->texture=loadTexture(buttons[0].texture->src);
            
            
            buttons[1].t = UNDO;
            buttons[1].container = malloc (sizeof (SDL_Rect));

            buttons[1].container->x=0;
            buttons[1].container->y=9*SCREEN_HEIGHT/12;
            buttons[1].container->h=3*SCREEN_HEIGHT/12;
            buttons[1].container->w=SCREEN_WIDTH/4;

            buttons[1].texture =malloc(sizeof(Texture));
            buttons[1].texture->src="images/play/undo.png";
            buttons[1].texture->texture=loadTexture(buttons[1].texture->src);
            
            
            for (int i=2 ; i<14 ; i++){

                buttons[i].t = PILE;
                buttons[i].container = malloc (sizeof (SDL_Rect));
                buttons[i].texture = malloc(sizeof (Texture));
                if (i<=7){
                    buttons[i].container->x=i*SCREEN_WIDTH/10;
                    buttons[i].container->y=15*SCREEN_HEIGHT/48;
                }
                else {
                    buttons[i].container->x=(i-6)*SCREEN_WIDTH/10;
                    buttons[i].container->y=(48-15)*SCREEN_HEIGHT/48-2*SCREEN_HEIGHT/15;
                }
                buttons[i].container->h=2*SCREEN_HEIGHT/15;
                buttons[i].container->w=SCREEN_WIDTH/10;
                
                buttons[i].texture->src="images/play/pile.png";
                buttons[i].texture->texture=loadTexture(buttons[i].texture->src);
            }
            menu->numberOfButtons=14;
            menu->buttons=buttons;
            break;
        }
        case WINNER : {
            background->src="images/winner.jpg";
            background->texture=loadTexture(background->src);
            menu->background=background;
            menu->numberOfButtons=1;
            Button* button = malloc (sizeof(Button));

            button->t = HOME; // the button type

            SDL_Rect* container = malloc (sizeof (SDL_Rect)); //the button container
            container->x=37*SCREEN_WIDTH/40;
            container->y=SCREEN_HEIGHT/12;
            container->h=SCREEN_HEIGHT/12;
            container->w=SCREEN_WIDTH/20;
            button->container=container;

            Texture* texture = malloc(sizeof(Texture)); // the button texture

            texture->src="images/play/homebutton.png";
            texture->texture=loadTexture(texture->src);

            button->texture=texture;
            menu->buttons=button;
        }
    }
    return menu;
}



void renderMenu(Menu* menu){
    SDL_RenderSetViewport(renderer,default_container);
    SDL_RenderCopy(renderer,menu->background->texture,NULL,NULL);
    if (menu->type!=MAIN_GAME){
        for (int i=0 ;i<menu->numberOfButtons; i++){
            SDL_RenderSetViewport(renderer,menu->buttons[i].container);
            SDL_RenderCopy(renderer,menu->buttons[i].texture->texture,NULL,NULL);
        }
    }
    switch (menu->type){
        case CHOOSE_FIRST_PLAYER:
        case CHOOSE_SECOND_PLAYER:
        case NEW_GAME_MENU:{
            loadFont("fonts/goudysto.ttf",20);
            SDL_Rect cont;
            Texture* texture = malloc (sizeof(Texture));
            SDL_Color textColor = {0,0,0};
            cont.x=SCREEN_WIDTH/3;
            cont.y=SCREEN_HEIGHT/2;
            
            texture->src=textInput;
            if (textInput!="") texture=LTextureText(texture->src,textColor);
            else {
                texture=LTextureText(" ",textColor);
            }
            cont.h=texture->h;
            cont.w=texture->w;
            SDL_RenderSetViewport(renderer,&cont);
            SDL_RenderCopy(renderer,texture->texture,NULL,NULL);
            break;
        }
        case WINNER :{
            loadFont("fonts/blowbrush.ttf",48);
            SDL_Rect cont;
            Texture* texture = malloc (sizeof(Texture));
            SDL_Color textColor = {0,0,0};
            cont.x=SCREEN_WIDTH/10;
            cont.y=SCREEN_HEIGHT/4;
            
            for (int i=0; i<KALAH; i++){
                api->game->player[0]->board[KALAH]+=api->game->player[0]->board[i];
                api->game->player[0]->board[i]=0;
                api->game->player[1]->board[KALAH]+=api->game->player[1]->board[i];
                api->game->player[1]->board[i]=0;
            }
            if (api->game->player[0]->board[KALAH]> api->game->player[1]->board[KALAH]) {
                texture=LTextureText(api->game->player[0]->name,textColor);
            }
            else {
                texture=LTextureText(api->game->player[1]->name,textColor);
            }
            cont.h=texture->h;
            cont.w=texture->w;
            SDL_RenderSetViewport(renderer,&cont);
            SDL_RenderCopy(renderer,texture->texture,NULL,NULL);
            break;
        }
        case MAIN_GAME :{
            for (int i=0 ;i<2; i++){
                SDL_RenderSetViewport(renderer,menu->buttons[i].container);
                SDL_RenderCopy(renderer,menu->buttons[i].texture->texture,NULL,NULL);
            }
            //render the board
            SDL_Rect cont;
            cont.x=3*SCREEN_WIDTH/40;
            cont.y=3*SCREEN_HEIGHT/40;
            cont.h=17*SCREEN_HEIGHT/20;
            cont.w=17*SCREEN_WIDTH/20;
            SDL_RenderSetViewport(renderer,&cont);
            Texture* texture = malloc (sizeof(Texture));
            texture->src="images/play/board.png";
            texture->texture=loadTexture(texture->src);
            SDL_RenderCopy(renderer,texture->texture,NULL,NULL);

            //render left and right kalah
            cont.x=3*SCREEN_WIDTH/128;
            cont.y=SCREEN_HEIGHT/3;
            cont.h=SCREEN_HEIGHT/3;
            cont.w=5*SCREEN_WIDTH/20;
            SDL_RenderSetViewport(renderer,&cont);
            texture->src="images/play/leftkalah.png";
            texture->texture=loadTexture(texture->src);
            SDL_RenderCopy(renderer,texture->texture,NULL,NULL);
            
            cont.x=58*SCREEN_WIDTH/80;
            SDL_RenderSetViewport(renderer,&cont);
            
            texture->src="images/play/rightkalah.png";
            texture->texture=loadTexture(texture->src);
            SDL_RenderCopy(renderer,texture->texture,NULL,NULL);
            
            
            for (int i=2 ;i<menu->numberOfButtons; i++){
                SDL_RenderSetViewport(renderer,menu->buttons[i].container);
                SDL_RenderCopy(renderer,menu->buttons[i].texture->texture,NULL,NULL);
            }

            //render the texts

            SDL_Color textColor = {0,0,0};
            // render players names
            loadFont("fonts/unisansbolditalic.ttf",28);
            
            cont.x=2*SCREEN_WIDTH/10;
            cont.y=SCREEN_HEIGHT/12;
            cont.h=SCREEN_HEIGHT/12;
            cont.w=3*SCREEN_WIDTH/10;

            texture=LTextureText(api->game->player[1]->name,textColor);
            cont.h=texture->h;
            cont.w=texture->w;
            SDL_RenderSetViewport(renderer,&cont);
            SDL_RenderCopy(renderer,texture->texture,NULL,NULL);
            
            cont.x=7*SCREEN_WIDTH/10;
            cont.y=5*SCREEN_HEIGHT/6;

            texture=LTextureText(api->game->player[0]->name,textColor);
            cont.h=texture->h;
            cont.w=texture->w;
            SDL_RenderSetViewport(renderer,&cont);
            SDL_RenderCopy(renderer,texture->texture,NULL,NULL);

            loadFont("fonts/goudysto.ttf",24);
            cont.h=SCREEN_HEIGHT/15;
            cont.w=SCREEN_WIDTH/10;

            for (int i=0 ; i<6; i++){
                cont.x=(i+2)*SCREEN_WIDTH/10+15;
                cont.y=13*SCREEN_HEIGHT/64;

                itoa(api->game->player[0]->board[5-i],texture->src,10);
                texture=LTextureText(texture->src,textColor);
                cont.h=texture->h;
                cont.w=texture->w;
                SDL_RenderSetViewport(renderer,&cont);
                SDL_RenderCopy(renderer,texture->texture,NULL,NULL);
            }
            for (int i=0 ; i<6; i++){
                cont.x= (i+2)*SCREEN_WIDTH/10+15;
                cont.y=(64-18)*SCREEN_HEIGHT/64;
                itoa(api->game->player[0]->board[i],texture->src,10);
                texture=LTextureText(texture->src,textColor);
                cont.h=texture->h;
                cont.w=texture->w;
                SDL_RenderSetViewport(renderer,&cont);
                SDL_RenderCopy(renderer,texture->texture,NULL,NULL);
            }

            //render first player KALAH value
            textColor.b=textColor.g=textColor.r=0xff;
            itoa(api->game->player[0]->board[KALAH],texture->src,10);
            texture=LTextureText(texture->src,textColor);
            cont.x=8*SCREEN_WIDTH/10+15;
            cont.y=(64-18)*SCREEN_HEIGHT/64;
            cont.h=texture->h;
            cont.w=texture->w;
            SDL_RenderSetViewport(renderer,&cont);
            SDL_RenderCopy(renderer,texture->texture,NULL,NULL);

            //render second player KALAH value
            cont.x=5*SCREEN_WIDTH/40;
            cont.y=13*SCREEN_HEIGHT/64;
            itoa(api->game->player[1]->board[KALAH],texture->src,10);
            texture=LTextureText(texture->src,textColor);
            cont.h=texture->h;
            cont.w=texture->w;
            SDL_RenderSetViewport(renderer,&cont);
            SDL_RenderCopy(renderer,texture->texture,NULL,NULL);

            break;
        }
    }
    SDL_RenderSetViewport(renderer,default_container);
}

void destroyTexture(Texture* texture){
    SDL_DestroyTexture(texture->texture);
    free(texture->src);
    texture->src=NULL;
    texture->texture=NULL;
    free(texture);
}

void destroyMenu(Menu* menu){
    destroyTexture(menu->background);
    menu->background=NULL;
    free(menu->buttons);
    menu->buttons=NULL;
    free(menu);
    menu=NULL;
}

void loadFont(char* src,int size )
{

    //Open the font
    font = TTF_OpenFont( src, size );
    if( font == NULL )
    {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
    }
}

//TODO : fix the empty string case
Texture* LTextureText( char* textureText, SDL_Color textColor )
{
    
    Texture* texture=malloc(sizeof(Texture));
    texture->src=textureText;
    SDL_Surface* textSurface;
    //Render text surface
    
    if (textureText == "") textSurface= TTF_RenderText_Solid( font, " ", textColor );
    else{
        textSurface = TTF_RenderText_Solid(font , textureText , textColor);
    } 
    
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        return NULL;
    }
    else
    {
        //Create texture from surface pixels
        texture->texture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( texture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else {
            texture->w=textSurface->w;
            texture->h=textSurface->h;
        }
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    return texture;
}


SDL_Texture* loadTexture( char* path ){
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    
    SDL_Surface* loadedSurface = IMG_Load(path);
    
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}


bool init()
{
	//Initialization flag
    SCREEN_HEIGHT=480;
    SCREEN_WIDTH=640;
    
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!\n" );
		}

		//Create window
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( renderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
                
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

                
				 //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

    textInput="";
    api=NULL;
    q=NULL;
    //loadAPI("game");
    /*
    strcpy(api->name,"a");
    api->numb_pieces=4*12;
    api->game=malloc(sizeof(stack));
    api->game->numb_games=1;
    api->game->prec=NULL;
    api->game->player[0]=malloc(sizeof(Player));
    api->game->player[1]=malloc(sizeof(Player));
    strcpy(api->game->player[1]->name,"lol");
    strcpy(api->game->player[0]->name,"lol2");
    api->game->s=malloc(sizeof(state));*/

    Menu* menu = createMenu(FIRST_FRAME);
    
    
    insert(menu);

    default_container = malloc (sizeof(SDL_Rect));
    default_container->x=default_container->y=0;
    default_container->h=SCREEN_HEIGHT;
    default_container->w=SCREEN_WIDTH;
    
    return success;
}


void free_game(stack* game){
    if (game==NULL) return;
    free_game(game->prec);
    free(game->s);
    game->s=NULL;
    free(game);
}

void freeAPI(){
    free_game(api->game);
    api->game=NULL;
    free(api);
    api=NULL;
}


void quit(){

    //destroy Menus
    empty();
    q=NULL;

    //free the API
    freeAPI();
    api=NULL;

    TTF_CloseFont( font );
    font=NULL;

    //Destroy window	
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

}
char* deleteChar(char* str){
    if (str=="") return str;
    int n=strlen(str);
    char* newstr= malloc(n*sizeof(char));
    for (int i=0; i<n-1; i++) newstr[i]=str[i];
    newstr[n-1]='\0';
    return newstr;
}
char* addChar(char* str,char c){
    char* newstr;
    int n= strlen(str);
    newstr=malloc((n+2)*sizeof(char));
    for (int i=0 ;i<n; i++) newstr[i]=str[i];
    newstr[n]=c;
    newstr[n+1]='\0';
    return newstr;
}
void insert(Menu *menu){
    MenuStack* newQ = malloc(sizeof(MenuStack));
    newQ->top=menu;
    newQ->next = q;
    q=newQ;
}
void delete(){
    if (q==NULL) return;
    destroyMenu(q->top);
    q=q->next;
}
void empty(){
    if (q==NULL) return;
    delete();
    empty();
}

