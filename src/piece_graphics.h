#ifndef PIECE_GRAPHICS_H
#define PIECE_GRAPHICS_H

#include "SDL2/SDL.h"
#include "piece.h"

void drawPiece(SDL_Renderer *renderer, Piece *piece, int x, int y, int size, SDL_Texture *textures[]);

#endif