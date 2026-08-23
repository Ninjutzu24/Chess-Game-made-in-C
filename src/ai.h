#ifndef AI_H
#define AI_H

#include "board.h"
#include "moves.h"
#include "piece.h"

int evalueazaPiesa(PieceType type);
Move gasesteCeaMaiBunaMutare(Board board, PieceColor computerColor, int depth);
int evalueazaTabla(Board board, PieceColor computerColor);

#endif