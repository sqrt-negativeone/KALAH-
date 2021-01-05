#include "Global.h"
#undef main

int main(int argc, char** argv)
{
    if (!init_app())
    {
        printf("could not initialize!\n");
    }
    else
    {
        //events handler
        SDL_Event e;

        //main loop flag
        bool quit = false;
        bool render = true;

        //main game loop
        while (!quit)
        {
            //render the menu to display
            //SDL_RenderClear(renderer);
            if (render)
            {
                SDL_RenderClear(renderer);
                renderMenu(q->top);
                render = false;
            } 
            //handel events based on the type of the menu
            switch (q->top->type)
            {
                // the menus with only buttons 
            case MAIN_GAME:
            {
                if (gameController->game->s->is_all_empty)
                {
                    Menu* menu = createMenu(WINNER);
                    insert(menu);
                    render = true;
                    break;
                }
            }
            case FIRST_FRAME:
            case CHOOSE_NUMBER_OF_PIECES:
            case HOME_MENU:
            case WINNER:
            {
                while (SDL_PollEvent(&e))
                {
                    //if quit request
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    if (e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        render = handleButtonClickEvent(q->top);
                    }
                }
                break;
            }
            case LOADING:
            {
                empty();
                saveGameController();
                Menu* menu = createMenu(MAIN_GAME);
                insert(menu);
                render = true;
                break;
            }
            default:
            { //Menus with input field
                SDL_StartTextInput();
                while (SDL_PollEvent(&e) != 0)
                {
                    //User requests quit
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    //Special key input
                    else if (e.type == SDL_KEYDOWN || e.type == SDL_TEXTINPUT)
                    {
                        render = handleTextInputEvent(e);
                    }
                    else if (e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        //loop over all the buttons in the menu
                        render = handleButtonClickEvent(q->top);
                    }
                }
                SDL_StopTextInput();
            }
            }
            //clear screen
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            SDL_RenderPresent(renderer);
        }
    }

    quit();
    return 0;
}
