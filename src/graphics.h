#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL2/SDL.h"
#include "piece.h"
#include "game.h"

#define NUM_FOGS 50
#define UI_PADDING 20

typedef struct
{
    float x, y;
    float dx, dy;
    int size;    // Dimensiunea ceții
    int opacity; // Opacitatea ceții
    SDL_Texture *texture;
} Fog;

extern Fog fogsGlobal[NUM_FOGS];
extern SDL_Texture *fogTexturesGlobal[3];
extern int ceataActiva;

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *path);
void initFog(SDL_Renderer *renderer, Fog fogs[], SDL_Texture *fogTextures[]);
void updateFog(Fog fogs[]);
void drawFog(SDL_Renderer *renderer, Fog fogs[]);

#endif
