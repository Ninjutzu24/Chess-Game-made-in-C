#ifndef PIECE_H
#define PIECE_H

#include "SDL2/SDL.h"

typedef enum
{
    EMPTY,
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
    WALL
} PieceType;

typedef enum
{
    NONE,
    WHITE,
    BLACK
} PieceColor;

typedef struct
{
    int x, y;
} Pozitie;

typedef struct
{
    PieceType type;
    PieceColor color;
    SDL_Texture *texture; // Textura piesei
} Piece;

#endif