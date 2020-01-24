#include <utils.h>


int main(){
    if (!init()){
        printf("could not initialize!\n");
    }
    else {

        //events handler
        SDL_Event e;

        //main loop flag
        bool quit=false;

        
        while (!quit){

            //render the menu to display
            renderMenu(q->top);


            //handel events based on the time of the menu
            switch(q->top->type){
                // the menus with only buttons 
                case FIRST_FRAME:
                case CHOOSE_NUMBER_OF_PIECES:
                case HOME_MENU:
                case MAIN_GAME:
                case WINNER:{
                    while (SDL_PollEvent(&e)){
                        //if quit request
                        if (e.type==SDL_QUIT) {
                            quit=true;
                        }

                        if (e.type == SDL_MOUSEBUTTONDOWN){
                            //loop over all the buttons in the menu
                            for (int i=0 ; i< q->top->numberOfButtons ; i++){
                                //if the mouse was inside the countainer of the button then activate it
                                if (isMouseInsideContainer(q->top->buttons[i].container)){
                                    handleClick(&(q->top->buttons[i]));
                                }
                            }
                        }

                    }
                    break;
                }
                case LOAD_GAME_MENU :{
                    //TODO : edit how to load a game
                    break;
                }
                default :{ //Menus with input field
                    SDL_StartTextInput();
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                            quit = true;
                        }
                        //Special key input
                        else if( e.type == SDL_KEYDOWN )
                        {
                            //Handle backspace
                            if( e.key.keysym.sym == SDLK_BACKSPACE  )
                            {
                                //lop off character
                                textInput=deleteChar(textInput);
                            }
                            //Handle copy
                            else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
                            {
                                SDL_SetClipboardText( textInput );
                            }
                            //Handle paste
                            else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
                            {
                                textInput = SDL_GetClipboardText();
                            }
                        }
                        //Special text input event
                        else if( e.type == SDL_TEXTINPUT )
                        {
                            //Not copy or pasting
                            if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) )
                            {
                                //Append character
                                addChar(textInput,e.text.text[0]);
                            }
                        }
                        else if (e.type== SDL_MOUSEBUTTONDOWN){
                            //loop over all the buttons in the menu
                            for (int i=0 ; i< q->top->numberOfButtons ; i++){
                                //if the mouse was inside the countainer of the button then activate it
                                if (isMouseInsideContainer(q->top->buttons[i].container)){
                                    handleClick(&(q->top->buttons[i]));
                                }
                            }
                        }
                    }
                    SDL_StopTextInput();
                }
            }
            //clear screen
            SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear( renderer );

            SDL_RenderPresent( renderer );
        }
    }

    quit();
    return 0;
}
