#include "Texture.h"
#include "Global.h"

//what it does is clear from its name lol
void loadFont(char* src, int size)
{

    //Open the font
    font = TTF_OpenFont(src, size);
    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        exit(-1);
    }
}

//load texture from text
Texture* LTextureText(char* textureText, SDL_Color textColor)
{

    Texture* texture = malloc(sizeof(Texture));
    texture->src = textureText;
    SDL_Surface* textSurface = NULL;
    //Render text surface

    if (textureText == "") textSurface = TTF_RenderText_Solid(font, " ", textColor);
    else
    {
        textSurface = TTF_RenderText_Solid(font, textureText, textColor);
    }

    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return NULL;
    }
    else
    {
        //Create texture from surface pixels
        texture->texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            texture->w = textSurface->w;
            texture->h = textSurface->h;
        }
        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
    return texture;
}

//load texture from image in path
SDL_Texture* loadTexture(char* path)
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path

    SDL_Surface* loadedSurface = IMG_Load(path);

    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}


void destroyTexture(Texture* texture)
{
    SDL_DestroyTexture(texture->texture);
    free(texture);
}
