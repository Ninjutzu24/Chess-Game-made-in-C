#ifndef SPECIAL_H
#define SPECIAL_H

#include "board.h"
#include "moves.h"


// Inițializeaza tabla pentru modul special (ex: pereti pe tabla de joc)
void initializeazaSpecialTabla(Board *board, SDL_Renderer *renderer, SDL_Texture *textures[]);

// Verifica daca exista un perete într-o anumita pozitie
int estePerete(Board *board, int row, int col);

// Verifica daca mutarea este valida in modul special
int esteMutareValidaSpecial(Board *board, const Move *move, int ignoraSah);

#endif
