#ifndef MOVES_H
#define MOVES_H

#include "board.h"

typedef struct
{
    int fromX, fromY;     // Pozitia de plecare (rand, coloana)
    int toX, toY;         // Pozitia destinatie (rand, coloana)
    Piece piesa;          // Piesa care se mută
    Piece piesaCapturata; // Piesa capturată (daca există)
    int score;
} Move;

// Functii pentru gestionarea mutarilor
int esteMutareValida(Board board, Move mutare, int ignoraSah);
void executaMutare(Board board, Move mutare, SDL_Texture *textures[]);
void anuleazaMutare(Board board, Move mutare);

// Functii pentru verificarea sahului
int esteSah(Board board, PieceColor culoareJucator);
int esteSahMat(Board board, PieceColor culoareJucator);

#endif