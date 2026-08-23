#ifndef PROMOVARE_H
#define PROMOVARE_H

#include "SDL2/SDL.h"
#include "piece.h" // pentru enum TipPromovare
#include "board.h"

typedef enum
{
    PROMOVARE_ANULATA = -1, // cand jucătorul inchide fereastra fara sa aleaga
    PROMOVARE_REGINA,
    PROMOVARE_TURA,
    PROMOVARE_CAL,
    PROMOVARE_NEBUN
} TipPromovare;

// Afisează meniul și returneaza optiunea aleasa
TipPromovare afiseazaMeniuPromovare(SDL_Renderer *renderer, SDL_Texture *textures[12], PieceColor culoare);

// Promovare Pion la alegere-Regina, tura, cal, nebun
void promoveazaPion(Board board, int rand, int coloana, PieceColor culoare, TipPromovare tip, SDL_Texture *textures[]);

#endif
