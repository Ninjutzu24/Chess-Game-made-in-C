#ifndef FOG_H
#define FOG_H

#include <SDL2/SDL.h>

// Inițializează textura de ceata (daca ai una externa)
void loadFogTexture(SDL_Renderer *renderer, const char *path);

// Deseneaza efectul de ceata peste tot ecranul
void deseneazaCeata(SDL_Renderer *renderer);

// Elibereaza memoria alocata pentru ceata
void freeFog();

#endif
