#include "fog.h"
#include <SDL2/SDL_image.h>
#include "game.h" 

static SDL_Texture *fogTexture = NULL;

void loadFogTexture(SDL_Renderer *renderer, const char *path)
{
    SDL_Surface *surf = IMG_Load(path);
    if (!surf)
    {
        SDL_Log("Eroare la încărcarea texturii de ceață: %s", IMG_GetError());
        return;
    }
    fogTexture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

void deseneazaCeata(SDL_Renderer *renderer)
{
    if (fogTexture)
    {
        SDL_Rect ceata = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_SetTextureBlendMode(fogTexture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, fogTexture, NULL, &ceata);
    }
    else
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 100);
        SDL_Rect ceata = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &ceata);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
}

void freeFog()
{
    if (fogTexture)
    {
        SDL_DestroyTexture(fogTexture);
        fogTexture = NULL;
    }
}
