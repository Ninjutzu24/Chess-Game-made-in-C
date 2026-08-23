#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include "piece.h"
#include "piece_graphics.h"

#define BOARD_SIZE 8
#define TILE_SIZE 75

typedef Piece Board[BOARD_SIZE][BOARD_SIZE];

void initializeazaTabla(Board board);
void punePiesePeTabla(Board *board, SDL_Renderer *renderer, SDL_Texture *textures[]);

void deseneazaTabla(SDL_Renderer *renderer, Board *board, SDL_Texture *textures[]);
void deseneazaTablaSpeciala(SDL_Renderer *renderer, Board *board, SDL_Texture *textures[], SDL_Texture *wallTexture);

void evidentiazaPatrat(SDL_Renderer *renderer, int rand, int coloana, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void copiazaTabla(Board sursa, Board destinatie);

#endif