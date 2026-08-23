#ifndef MOUSE_INTERACTION_H
#define MOUSE_INTERACTION_H

#include "SDL2/SDL.h"
#include "board.h"

void obtineMutariValide(Board board, int rand, int coloana, int mutariValide[8][8]);
void evidentiazaMutariValide(SDL_Renderer *renderer, Board board, int rand, int coloana);

#endif