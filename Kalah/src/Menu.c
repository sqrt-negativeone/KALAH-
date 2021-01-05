#include "Global.h"
#include "dirent.h"

//helpful functions
//functions to manipulate the characters
char* deleteChar(char* str)
{
    if (str == "") return "";
    int n = strlen(str);
    char* newstr = malloc(n * sizeof(char));
    for (int i = 0; i < n - 1; i++) newstr[i] = str[i];
    newstr[n - 1] = '\0';
    return newstr;
}
char* addChar(char* str, char c)
{
    char* newstr;
    int n = strlen(str);
    newstr = malloc((n + 2) * sizeof(char));
    for (int i = 0; i < n; i++) newstr[i] = str[i];
    newstr[n] = c;
    newstr[n + 1] = '\0';
    return newstr;
}

int isMouseInsideContainer(SDL_Rect* container)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return ((container->x < x&& x < (container->x + container->w)) && (container->y < y&& y < (container->y + container->h)));
}

//creating a menu based on thier types
/*
    create background
    create buttons
*/
Menu* createMenu(MenuType type)
{
    Menu* menu = malloc(sizeof(Menu));
    menu->type = type;
    Texture* background = malloc(sizeof(Texture));
    switch (type)
    {
    case FIRST_FRAME:
    {

        background->src = "assets/Images/homescreen/firstframe.jpg";
        background->texture = loadTexture(background->src);
        menu->background = background;

        Button* button = malloc(sizeof(Button));

        button->t = PLAY; // the button type
        button->container = malloc(sizeof(SDL_Rect));
        button->container->x = 3 * SCREEN_WIDTH / 8;
        button->container->y = 4 * SCREEN_HEIGHT / 5;
        button->container->h = SCREEN_HEIGHT / 5;
        button->container->w = 2 * SCREEN_WIDTH / 8;

        button->texture = malloc(sizeof(Texture));

        button->texture->src = "assets/Images/homescreen/play.png";

        button->texture->texture = loadTexture(button->texture->src);

        menu->numberOfButtons = 1;
        menu->buttons = button;

        break;
    }
    case LOAD_GAME_MENU:
    {
        background->src = "assets/Images/loadgame/loadgame.jpg";
        background->texture = loadTexture(background->src);
        menu->background = background;

        Button* buttons = malloc(2 * sizeof(Button));

        buttons[0].container = malloc(sizeof(SDL_Rect));
        buttons[0].texture = malloc(sizeof(Texture));
        buttons[0].t = PREVIOUS;

        buttons[0].container->x = 0;
        buttons[0].container->y = 2 * SCREEN_HEIGHT / 3;
        buttons[0].container->h = SCREEN_HEIGHT / 3;
        buttons[0].container->w = SCREEN_WIDTH / 4;

        buttons[0].texture->src = "assets/Images/play/undo.png";
        buttons[0].texture->texture = loadTexture("assets/Images/play/undo.png");

        buttons[1].t = NEXT; // the button type
        buttons[1].container = malloc(sizeof(SDL_Rect));
        buttons[1].texture = malloc(sizeof(Texture));

        buttons[1].container->x = 3 * SCREEN_WIDTH / 4;
        buttons[1].container->y = 2 * SCREEN_HEIGHT / 3;
        buttons[1].container->h = SCREEN_HEIGHT / 3;
        buttons[1].container->w = SCREEN_WIDTH / 4;

        buttons[1].texture->src = "assets/Images/newgame/next.png";
        buttons[1].texture->texture = loadTexture(buttons[1].texture->src);
        menu->numberOfButtons = 2;
        menu->buttons = buttons;
        break;
    }
    case NEW_GAME_MENU:
    {
        background->src = "assets/Images/newgame/gamename.jpg";
        background->texture = loadTexture(background->src);
        menu->background = background;

    }
    case CHOOSE_FIRST_PLAYER:
    {
        if (type == CHOOSE_FIRST_PLAYER)
        {
            background->src = "assets/Images/newgame/player1.jpg";
            background->texture = loadTexture(background->src);
            menu->background = background;
        }
    }
    case CHOOSE_SECOND_PLAYER:
    {
        if (type == CHOOSE_SECOND_PLAYER)
        {
            background->src = "assets/Images/newgame/player2.jpg";
            background->texture = loadTexture(background->src);
            menu->background = background;
        }

        Button* button = malloc(2 * sizeof(Button));

        button[0].t = NEXT; // the button type
        button->container = malloc(sizeof(SDL_Rect));
        button->container->x = 3 * SCREEN_WIDTH / 4;
        button->container->y = 2 * SCREEN_HEIGHT / 3;
        button->container->h = SCREEN_HEIGHT / 3;
        button->container->w = SCREEN_WIDTH / 4;
        button->texture = malloc(sizeof(Texture));
        button->texture->src = "assets/Images/newgame/next.png";
        button->texture->texture = loadTexture(button->texture->src);

        button[1].t = PREVIOUS;
        button[1].container = malloc(sizeof(SDL_Rect));
        button[1].container->x = 0;
        button[1].container->y = 2 * SCREEN_HEIGHT / 3;
        button[1].container->h = SCREEN_HEIGHT / 3;
        button[1].container->w = SCREEN_WIDTH / 4;
        button[1].texture = malloc(sizeof(Texture));
        button[1].texture->src = "assets/Images/play/undo.png";
        button[1].texture->texture = loadTexture(button[1].texture->src);

        menu->numberOfButtons = 2;
        menu->buttons = button;
        break;
    }
    case CHOOSE_NUMBER_OF_PIECES:
    {
        background->src = "assets/Images/newgame/pieceschoice.jpg";
        background->texture = loadTexture(background->src);
        menu->background = background;

        Button* buttons = malloc(4 * sizeof(Button));

        for (int i = 0; i < 3; i++)
        {
            buttons[i].container = malloc(sizeof(SDL_Rect));
            buttons[i].texture = malloc(sizeof(Texture));
            buttons[i].t = NUMBER_OF_PIECES;

            buttons[i].container->y = SCREEN_HEIGHT / 2;
            buttons[i].container->x = (i + 1) * SCREEN_WIDTH / 5;
            buttons[i].container->h = SCREEN_HEIGHT / 5;
            buttons[i].container->w = SCREEN_WIDTH / 5;

            switch (i)
            {
            case 0:buttons[i].texture->src = "assets/Images/newgame/4.png"; break;
            case 1:buttons[i].texture->src = "assets/Images/newgame/5.png"; break;
            case 2:buttons[i].texture->src = "assets/Images/newgame/6.png"; break;
            }
            buttons[i].texture->texture = loadTexture(buttons[i].texture->src);
        }
        buttons[3].container = malloc(sizeof(SDL_Rect));
        buttons[3].texture = malloc(sizeof(Texture));
        buttons[3].t = PREVIOUS;

        buttons[3].container->x = 0;
        buttons[3].container->y = 2 * SCREEN_HEIGHT / 3;
        buttons[3].container->h = SCREEN_HEIGHT / 3;
        buttons[3].container->w = SCREEN_WIDTH / 4;

        buttons[3].texture->src = "assets/Images/play/undo.png";
        buttons[3].texture->texture = loadTexture(buttons[3].texture->src);


        menu->numberOfButtons = 4;
        menu->buttons = buttons;
        break;
    }
    case HOME_MENU:
    {
        background->src = "assets/Images/background.jpg";
        background->texture = loadTexture(background->src);
        menu->background = background;
        Button* buttons;

        if (q == NULL) buttons = (Button*)malloc(2 * sizeof(Button));
        else buttons = (Button*)malloc(3 * sizeof(Button));

        buttons[0].t = NEW_GAME_BUTTON;
        buttons[1].t = LOAD_GAME_BUTTON;

        if (!(q == NULL)) buttons[2].t = PREVIOUS;

        buttons[0].container = malloc(sizeof(SDL_Rect));
        buttons[0].texture = malloc(sizeof(Texture));
        buttons[0].container->x = 3 * SCREEN_WIDTH / 10;
        buttons[0].container->y = 1 * SCREEN_HEIGHT / 12;
        buttons[0].container->h = 2 * SCREEN_HEIGHT / 6;
        buttons[0].container->w = 2 * SCREEN_WIDTH / 5;

        buttons[0].texture->src = "assets/Images/homescreen/newgame.png";

        buttons[0].texture->texture = loadTexture(buttons[0].texture->src);

        buttons[1].container = malloc(sizeof(SDL_Rect));
        buttons[1].texture = malloc(sizeof(Texture));
        buttons[1].container->x = 3 * SCREEN_WIDTH / 10;
        buttons[1].container->y = 5 * SCREEN_HEIGHT / 12;
        buttons[1].container->h = 2 * SCREEN_HEIGHT / 6;
        buttons[1].container->w = 2 * SCREEN_WIDTH / 5;

        buttons[1].texture->src = "assets/Images/homescreen/loadgame.png";
        buttons[1].texture->texture = loadTexture(buttons[1].texture->src);

        if (!(q == NULL))
        {
            buttons[2].container = malloc(sizeof(SDL_Rect));
            buttons[2].texture = malloc(sizeof(Texture));
            buttons[2].container->x = 3 * SCREEN_WIDTH / 10;
            buttons[2].container->y = 9 * SCREEN_HEIGHT / 12;

            buttons[2].container->h = 3 * SCREEN_HEIGHT / 12;
            buttons[2].container->w = 2 * SCREEN_WIDTH / 5;

            buttons[2].texture->src = "assets/Images/homescreen/reprendre.png";
            buttons[2].texture->texture = loadTexture(buttons[2].texture->src);
        }
        if (q == NULL) menu->numberOfButtons = 2;
        else menu->numberOfButtons = 3;
        menu->buttons = buttons;
        break;
    }
    case MAIN_GAME:
    {
        background->src = "assets/Images/play/game.jpg";
        background->texture = loadTexture(background->src);
        menu->background = background;
        Button* buttons = malloc(14 * sizeof(Button));

        buttons[0].t = HOME;
        buttons[0].container = malloc(sizeof(SDL_Rect));

        buttons[0].container->x = 35 * SCREEN_WIDTH / 40;
        buttons[0].container->y = SCREEN_HEIGHT / 24;
        buttons[0].container->h = 3 * SCREEN_HEIGHT / 24;
        buttons[0].container->w = 3 * SCREEN_WIDTH / 24;

        buttons[0].texture = malloc(sizeof(Texture));
        buttons[0].texture->src = "assets/Images/play/homebutton.png";
        buttons[0].texture->texture = loadTexture(buttons[0].texture->src);


        buttons[1].t = UNDO;
        buttons[1].container = malloc(sizeof(SDL_Rect));

        buttons[1].container->x = 0;
        buttons[1].container->y = 9 * SCREEN_HEIGHT / 12;
        buttons[1].container->h = 3 * SCREEN_HEIGHT / 12;
        buttons[1].container->w = SCREEN_WIDTH / 4;

        buttons[1].texture = malloc(sizeof(Texture));
        buttons[1].texture->src = "assets/Images/play/undo.png";
        buttons[1].texture->texture = loadTexture(buttons[1].texture->src);


        for (int i = 2; i < 14; i++)
        {

            buttons[i].t = PILE;
            buttons[i].container = malloc(sizeof(SDL_Rect));
            buttons[i].texture = malloc(sizeof(Texture));
            if (i <= 7)
            {
                buttons[i].container->x = i * SCREEN_WIDTH / 10;
                buttons[i].container->y = 15 * SCREEN_HEIGHT / 48;
            }
            else
            {
                buttons[i].container->x = (i - 6) * SCREEN_WIDTH / 10;
                buttons[i].container->y = (48 - 15) * SCREEN_HEIGHT / 48 - 2 * SCREEN_HEIGHT / 15;
            }
            buttons[i].container->h = 2 * SCREEN_HEIGHT / 15;
            buttons[i].container->w = SCREEN_WIDTH / 10;

            buttons[i].texture->src = "assets/Images/play/pile.png";
            buttons[i].texture->texture = loadTexture(buttons[i].texture->src);
        }
        menu->numberOfButtons = 14;
        menu->buttons = buttons;
        break;
    }
    case WINNER:
    {
        background->src = "assets/Images/winner.jpg";
        background->texture = loadTexture(background->src);
        menu->background = background;
        menu->numberOfButtons = 1;
        Button* buttons = malloc(sizeof(Button));

        buttons[0].t = HOME;
        buttons[0].container = malloc(sizeof(SDL_Rect));

        buttons[0].container->x = 35 * SCREEN_WIDTH / 40;
        buttons[0].container->y = SCREEN_HEIGHT / 24;
        buttons[0].container->h = 3 * SCREEN_HEIGHT / 24;
        buttons[0].container->w = 3 * SCREEN_WIDTH / 24;

        buttons[0].texture = malloc(sizeof(Texture));
        buttons[0].texture->src = "assets/Images/play/homebutton.png";
        buttons[0].texture->texture = loadTexture(buttons[0].texture->src);

        menu->buttons = buttons;
        break;
    }
    case LOADING:
    {
        background->src = "assets/Images/loading.jpg";
        background->texture = loadTexture(background->src);
        menu->background = background;
        menu->numberOfButtons = 0;
        menu->buttons = NULL;
    }
    }
    return menu;
}


//render the menu based on the type
/*
    render background
    render buttons
    if the menu is with input field render the text
    the render process is different for the case of the MAIN_GAME and WINNER menus as there are other elements to render but buttons

*/
void renderPieces(SDL_Rect* container, int nombrePieces)
{
    Texture* piece = malloc(sizeof(Texture));
    piece->src = "assets/Images/play/sphere.png";
    piece->texture = loadTexture(piece->src);
    srand(3);
    SDL_Rect* cont = malloc(sizeof(SDL_Rect));
    cont->h = 17;
    cont->w = 17;
    for (int i = 0; i < nombrePieces; i++)
    {
        int x, y;
        x = rand() % (7 * container->w / 16);
        y = rand() % (7 * container->h / 16);
        cont->x = container->x + 3 * container->w / 16 + x;
        cont->y = container->y + 3 * container->h / 16 + y;
        SDL_RenderSetViewport(renderer, cont);
        SDL_RenderCopy(renderer, piece->texture, NULL, NULL);
    }
    free(cont);
    destroyTexture(piece);
}

void renderMenu(Menu* menu)
{
    SDL_RenderSetViewport(renderer, default_container);
    SDL_RenderCopy(renderer, menu->background->texture, NULL, NULL);
    if (menu->type != MAIN_GAME)
    {
        for (int i = 0; i < menu->numberOfButtons; i++)
        {
            SDL_RenderSetViewport(renderer, menu->buttons[i].container);
            SDL_RenderCopy(renderer, menu->buttons[i].texture->texture, NULL, NULL);
        }
    }
    switch (menu->type)
    {
    case CHOOSE_FIRST_PLAYER:
    case CHOOSE_SECOND_PLAYER:
    case LOAD_GAME_MENU:
    case NEW_GAME_MENU:
    {
        SDL_Rect cont;
        Texture* texture;
        SDL_Color textColor = { 0,0,0 };
        if (menu->type == LOAD_GAME_MENU)
        {
            texture = malloc(sizeof(Texture));
            cont.x = SCREEN_WIDTH / 8;
            cont.y = SCREEN_HEIGHT / 4;
            cont.w = 3 * SCREEN_WIDTH / 4;
            cont.h = SCREEN_HEIGHT / 5;
            texture->src = "assets/Images/loadgame/bar.png";
            texture->texture = loadTexture(texture->src);
            SDL_RenderSetViewport(renderer, &cont);
            SDL_RenderCopy(renderer, texture->texture, NULL, NULL);
            SDL_DestroyTexture(texture->texture);
            loadFont("assets/Fonts/arialbd.ttf", 18);
            DIR* d;
            struct dirent* dir;
            cont.x = 3 * SCREEN_WIDTH / 8;
            cont.y = 33 * SCREEN_HEIGHT / 64;
            d = opendir(".\\saves");
            if (d)
            {
                while ((dir = readdir(d)) != NULL)
                {

                    if (dir->d_type == DT_REG)
                    {
                        texture = LTextureText(dir->d_name, textColor);
                        cont.h = texture->h;
                        cont.w = texture->w;
                        SDL_RenderSetViewport(renderer, &cont);
                        SDL_RenderCopy(renderer, texture->texture, NULL, NULL);
                        SDL_DestroyTexture(texture->texture);
                        cont.y += cont.h;
                    }
                }
                closedir(d);
            }

        }
        if (strlen(textInput) == 0) break;
        loadFont("assets/Fonts/goudysto.ttf", 20);
        if (menu->type == LOAD_GAME_MENU) cont.y = 5 * SCREEN_HEIGHT / 16;
        else cont.y = SCREEN_HEIGHT / 2;
        cont.x = SCREEN_WIDTH / 2;
        texture = LTextureText(textInput, textColor);
        cont.h = texture->h;
        cont.w = texture->w;
        cont.x -= cont.w / 2;
        SDL_RenderSetViewport(renderer, &cont);
        SDL_RenderCopy(renderer, texture->texture, NULL, NULL);
        SDL_DestroyTexture(texture->texture);
        free(texture);
        break;
    }
    case WINNER:
    {
        loadFont("assets/Fonts/blowbrush.ttf", 70);
        SDL_Rect cont;
        Texture* texture = malloc(sizeof(Texture));
        SDL_Color textColor = { 0,0,0 };
        cont.x = SCREEN_WIDTH / 3;
        cont.y = SCREEN_HEIGHT / 2;

        for (int i = 0; i < KALAH; i++)
        {
            gameController->game->player[0]->board[KALAH] += gameController->game->player[0]->board[i];
            gameController->game->player[0]->board[i] = 0;
            gameController->game->player[1]->board[KALAH] += gameController->game->player[1]->board[i];
            gameController->game->player[1]->board[i] = 0;
        }

        if (gameController->game->player[0]->board[KALAH] > gameController->game->player[1]->board[KALAH])
        {
            texture = LTextureText(gameController->game->player[0]->name, textColor);
        }
        else
        {
            texture = LTextureText(gameController->game->player[1]->name, textColor);
        }

        cont.h = texture->h;
        cont.w = texture->w;
        SDL_RenderSetViewport(renderer, &cont);
        SDL_RenderCopy(renderer, texture->texture, NULL, NULL);

        destroyTexture(texture);
        break;
    }
    case MAIN_GAME:
    {
        for (int i = 0; i < 2; i++)
        {
            SDL_RenderSetViewport(renderer, menu->buttons[i].container);
            SDL_RenderCopy(renderer, menu->buttons[i].texture->texture, NULL, NULL);
        }

        //render the board
        SDL_Rect cont;
        cont.x = 3 * SCREEN_WIDTH / 40;
        cont.y = 3 * SCREEN_HEIGHT / 40;
        cont.h = 17 * SCREEN_HEIGHT / 20;
        cont.w = 17 * SCREEN_WIDTH / 20;
        SDL_RenderSetViewport(renderer, &cont);
        Texture* texture = malloc(sizeof(Texture));
        texture->src = "assets/Images/play/board.png";
        texture->texture = loadTexture(texture->src);
        SDL_RenderCopy(renderer, texture->texture, NULL, NULL);
        SDL_DestroyTexture(texture->texture);

        //render left and right kalah
        cont.x = 3 * SCREEN_WIDTH / 128;
        cont.y = SCREEN_HEIGHT / 3;
        cont.h = SCREEN_HEIGHT / 3;
        cont.w = 5 * SCREEN_WIDTH / 20;
        SDL_RenderSetViewport(renderer, &cont);
        texture->src = "assets/Images/play/leftkalah.png";
        texture->texture = loadTexture(texture->src);
        SDL_RenderCopy(renderer, texture->texture, NULL, NULL);

        SDL_DestroyTexture(texture->texture);

        cont.x = 58 * SCREEN_WIDTH / 80;
        SDL_RenderSetViewport(renderer, &cont);

        texture->src = "assets/Images/play/rightkalah.png";
        texture->texture = loadTexture(texture->src);
        SDL_RenderCopy(renderer, texture->texture, NULL, NULL);
        SDL_DestroyTexture(texture->texture);

        cont.x = 10 * SCREEN_WIDTH / 128;
        cont.y = SCREEN_HEIGHT / 3;
        cont.h = SCREEN_HEIGHT / 3;
        cont.w = 3 * SCREEN_WIDTH / 20;
        int player1KALAH = gameController->game->player[1]->board[KALAH];
        renderPieces(&cont, player1KALAH);

        cont.x = 62 * SCREEN_WIDTH / 80;
        int player2KALAH = gameController->game->player[0]->board[KALAH];
        renderPieces(&cont, player2KALAH);

        for (int i = 2; i < menu->numberOfButtons; i++)
        {
            //rendering piles
            SDL_RenderSetViewport(renderer, menu->buttons[i].container);
            SDL_RenderCopy(renderer, menu->buttons[i].texture->texture, NULL, NULL);
            //rendering pieces
            int nombrePieces;
            if (i < 8) nombrePieces = gameController->game->player[1]->board[5 - (i - 2)];
            else  nombrePieces = gameController->game->player[0]->board[i - 8];
            renderPieces(menu->buttons[i].container, nombrePieces);
        }

        //render the texts

        SDL_Color textColor = { 0,0,0 };
        // render players names
        loadFont("assets/Fonts/unisansbolditalic.ttf", 28);

        cont.x = 2 * SCREEN_WIDTH / 10;
        cont.y = SCREEN_HEIGHT / 12;
        cont.h = SCREEN_HEIGHT / 12;
        cont.w = 3 * SCREEN_WIDTH / 10;
        if (gameController->game->s->player_turn == 1) textColor.r = 0xff;

        destroyTexture(texture);
        char* txt;
        char holder[50];
        txt = strcpy(holder, gameController->game->player[1]->name);
        texture = LTextureText(txt, textColor);

        textColor.r = 0;
        cont.h = texture->h;
        cont.w = texture->w;
        SDL_RenderSetViewport(renderer, &cont);
        SDL_RenderCopy(renderer, texture->texture, NULL, NULL);

        cont.x = 7 * SCREEN_WIDTH / 10;
        cont.y = 5 * SCREEN_HEIGHT / 6;

        if (gameController->game->s->player_turn == 0) textColor.r = 0xff;

        destroyTexture(texture);
        txt = strcpy(holder, gameController->game->player[0]->name);
        texture = LTextureText(txt, textColor);

        textColor.r = 0;
        cont.h = texture->h;
        cont.w = texture->w;
        SDL_RenderSetViewport(renderer, &cont);
        SDL_RenderCopy(renderer, texture->texture, NULL, NULL);

        loadFont("assets/Fonts/goudysto.ttf", 24);
        cont.h = SCREEN_HEIGHT / 15;
        cont.w = SCREEN_WIDTH / 10;

        for (int i = 0; i < 6; i++)
        {
            cont.x = (i + 2) * SCREEN_WIDTH / 10 + 15;
            cont.y = 13 * SCREEN_HEIGHT / 64;
            
            char* buffer = _itoa(gameController->game->player[1]->board[5 - i], holder, 10);
            destroyTexture(texture); texture = NULL;
            texture = LTextureText(buffer, textColor);
            cont.h = texture->h;
            cont.w = texture->w;
            SDL_RenderSetViewport(renderer, &cont);
            SDL_RenderCopy(renderer, texture->texture, NULL, NULL);
        }
        for (int i = 0; i < 6; i++)
        {
            cont.x = (i + 2) * SCREEN_WIDTH / 10 + 15;
            cont.y = (64 - 18) * SCREEN_HEIGHT / 64;
            char holder[50];
            char* buffer = _itoa(gameController->game->player[0]->board[i], holder, 10);
            destroyTexture(texture); texture = NULL;
            texture = LTextureText(buffer, textColor);
            cont.h = texture->h;
            cont.w = texture->w;
            SDL_RenderSetViewport(renderer, &cont);
            SDL_RenderCopy(renderer, texture->texture, NULL, NULL);
        }

        //render first player KALAH value
        textColor.b = textColor.g = textColor.r = 0xff;
        char* buffer = _itoa(gameController->game->player[0]->board[KALAH], holder, 10);

        destroyTexture(texture); texture = NULL;
        texture = LTextureText(buffer, textColor);
        cont.x = 8 * SCREEN_WIDTH / 10 + 15;
        cont.y = (64 - 18) * SCREEN_HEIGHT / 64;
        cont.h = texture->h;
        cont.w = texture->w;
        SDL_RenderSetViewport(renderer, &cont);
        SDL_RenderCopy(renderer, texture->texture, NULL, NULL);

        //render second player KALAH value
        cont.x = 5 * SCREEN_WIDTH / 40;
        cont.y = 13 * SCREEN_HEIGHT / 64;
        buffer = _itoa(gameController->game->player[1]->board[KALAH], holder, 10);

        destroyTexture(texture); texture = NULL;
        texture = LTextureText(buffer, textColor);
        cont.h = texture->h;
        cont.w = texture->w;
        SDL_RenderSetViewport(renderer, &cont);
        SDL_RenderCopy(renderer, texture->texture, NULL, NULL);
        destroyTexture(texture);
        break;
    }

    }
    SDL_RenderSetViewport(renderer, default_container);
}


void deleteButtons(Button* buttons, int button_cnt)
{
    for (int i = 0; i < button_cnt; i++)
    {
        free(buttons[i].container);
        destroyTexture(buttons[i].texture);
    }
    free(buttons);
}

void destroyMenu(Menu* menu)
{
    destroyTexture(menu->background);
    menu->background = NULL;
    deleteButtons(menu->buttons, menu->numberOfButtons);
    free(menu);
    menu = NULL;
}

void insert(Menu* menu)
{
    MenuStack* newQ = malloc(sizeof(MenuStack));
    newQ->top = menu;
    newQ->next = q;
    q = newQ;
}
void delete()
{
    if (q == NULL) return;
    destroyMenu(q->top);
    q = q->next;
}
void empty()
{
    if (q == NULL) return;
    delete();
    empty();
}

//handle the mouse clicks based on the type of the button the menu currently dispalyed
bool handleClick(Button* button)
{
    bool render = true;
    switch (button->t)
    {

    case PLAY:
    {
        delete();
    }
    case HOME:
    {
        Menu* menu = createMenu(HOME_MENU);
        insert(menu);
        break;
    }
    case NEW_GAME_BUTTON:
    {
        Menu* menu = createMenu(NEW_GAME_MENU);
        insert(menu);
        break;
    }
    case LOAD_GAME_BUTTON:
    {
        Menu* menu = createMenu(LOAD_GAME_MENU);
        insert(menu);
        break;
    }
    case UNDO:
    {
        if (gameController->game->prec != NULL)
        {
            gameController->game = gameController->game->prec;
        }
        else
        {
            render = false;
            return;
        }
        break;
    }
    case PILE:
    {
        int x;
        x = button->container->x;
        int p = button->container->y < SCREEN_HEIGHT / 2;
        if (p != gameController->game->s->player_turn)
        {
            render = false;
            return;
        }
        int move = 10 * x / SCREEN_WIDTH;
        move -= 2;
        if (p == 1) move = 5 - move;
        play(move);

        return;
    }
    case NUMBER_OF_PIECES:
    {
        int x = button->container->x;
        int pieces = 5 * x / SCREEN_WIDTH;
        pieces += 3;
        tmpGameController->numb_pieces = pieces * 12;
        Menu* menu = createMenu(CHOOSE_FIRST_PLAYER);
        insert(menu);
        break;
    }
    case NEXT:
    {
        switch (q->top->type)
        {
        case NEW_GAME_MENU:
        {
            if (textInput == "") break;
            //the creation of the new api is done in a temporary API so that the current API is not lost
            if (tmpGameController == NULL)
            {
                tmpGameController = malloc(sizeof(GameController));
                tmpGameController->game = malloc(sizeof(stack));
                tmpGameController->game->numb_games = 1;
                tmpGameController->game->s = malloc(sizeof(state));
                tmpGameController->game->prec = NULL;
                tmpGameController->game->player[0] = malloc(sizeof(Player));
                tmpGameController->game->player[1] = malloc(sizeof(Player));
            }
            srand(time(NULL));
            tmpGameController->game->s->player_turn = rand() % 2; // chose a random player to start the game
            tmpGameController->game->s->is_all_empty = tmpGameController->game->s->is_last_move_at_empty = tmpGameController->game->s->is_last_move_at_kalah = 0;
            strcpy(tmpGameController->name, textInput);
            textInput = "";
            Menu* menu = createMenu(CHOOSE_NUMBER_OF_PIECES);
            insert(menu);
            break;
        }
        case CHOOSE_FIRST_PLAYER:
        {
            if (textInput == "") break;
            strcpy(tmpGameController->game->player[0]->name, textInput);
            for (int i = 0; i < KALAH; i++) tmpGameController->game->player[0]->board[i] = tmpGameController->numb_pieces / 12;
            tmpGameController->game->player[0]->board[KALAH] = 0;
            textInput = "";
            Menu* menu = createMenu(CHOOSE_SECOND_PLAYER);
            insert(menu);
            break;
        }
        case CHOOSE_SECOND_PLAYER:
        {
            if (textInput == "") break;
            strcpy(tmpGameController->game->player[1]->name, textInput);
            for (int i = 0; i < KALAH; i++) tmpGameController->game->player[1]->board[i] = tmpGameController->numb_pieces / 12;
            tmpGameController->game->player[1]->board[KALAH] = 0;
            textInput = "";
            freeGameController(gameController);
            gameController = tmpGameController;
            tmpGameController = NULL;
            Menu* menu = createMenu(LOADING);
            insert(menu);
            break;
        }
        case LOAD_GAME_MENU:
        {
            char dir[50] = "saves\\";
            strcat(dir, textInput);
            FILE* file = fopen(dir, "r");
            if (file == NULL) break;
            fclose(file);
            freeGameController(gameController);
            loadGameController(textInput);
            textInput = "";
            Menu* menu = createMenu(MAIN_GAME);
            empty();
            insert(menu);
            break;
        }
        }
        break;
    }
    case PREVIOUS:
    {
        delete();
    }
    }
    return render;
}
//-----------------------------------------------------------------------------------------------

bool handleButtonClickEvent(Menu* menu)
{
    bool render = false;
    for (int i = 0; i < menu->numberOfButtons; i++)
    {
        //if the mouse was inside the countainer of the button then activate it
        if (isMouseInsideContainer(menu->buttons[i].container))
        {
            render = handleClick(&(menu->buttons[i]));
            break;
        }
    }
    return render;
}

bool handleTextInputEvent(SDL_Event e)
{
    bool render = false;
    if (e.type == SDL_KEYDOWN)
    {
        //Handle backspace
        if (e.key.keysym.sym == SDLK_BACKSPACE)
        {
            //lop off character
            if (strlen(textInput) == 0) return render;
            render = true;
            textInput = deleteChar(textInput);

        }
        //Handle copy
        else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
        {
            SDL_SetClipboardText(textInput);
        }
        //Handle paste
        else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
        {
            render = true;
            textInput = SDL_GetClipboardText();
        }
    }
    //Special text input event
    else if (e.type == SDL_TEXTINPUT)
    {
        //Not copy or pasting
        if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
        {
            render = true;
            //Append character
            textInput = addChar(textInput, e.text.text[0]);
        }
    }
    return render;
}