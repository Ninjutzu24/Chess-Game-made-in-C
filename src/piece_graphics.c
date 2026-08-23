#include <stdio.h>
#include "piece_graphics.h"
#include "SDL2/SDL.h"

void drawPiece(SDL_Renderer *renderer, Piece *piece, int x, int y, int size, SDL_Texture *textures[])
{
    SDL_Rect rect = {x * size, y * size, size, size};
    SDL_Texture *tex = NULL;

    // Selecteaza textura corecta în functie de tip și culoare
    if (piece->type != EMPTY)
    {
        int index = (piece->color == WHITE) ? 0 : 6;
        switch (piece->type)
        {
        case PAWN:
            tex = textures[index];
            break;
        case ROOK:
            tex = textures[index + 1];
            break;
        case KNIGHT:
            tex = textures[index + 2];
            break;
        case BISHOP:
            tex = textures[index + 3];
            break;
        case QUEEN:
            tex = textures[index + 4];
            break;
        case KING:
            tex = textures[index + 5];
            break;
        default:
            break;
        }
    }

    if (tex != NULL)
    {
        SDL_RenderCopy(renderer, tex, NULL, &rect);
    }
}