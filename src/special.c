#include "special.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "moves.h"
#include "board.h"
#include "mod_special.h"
#include "graphics.h"

void amestecaPiese(Piece *piese, int numarPiese)
{
    for (int i = 0; i < numarPiese - 1; i++)
    {
        int j = i + rand() % (numarPiese - i);
        Piece temp = piese[j];
        piese[j] = piese[i];
        piese[i] = temp;
    }
}

void initializeazaSpecialTabla(Board *board, SDL_Renderer *renderer, SDL_Texture *textures[])
{
    SetariModSpecial setari = getSetariModSpecial();

    srand(time(NULL));

    // Curatare tabla
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            (*board)[i][j].type = EMPTY;
            (*board)[i][j].color = NONE;
            (*board)[i][j].texture = NULL;
        }
    }

    // Piesele de start
    Piece pieseAlbe[16] = {
        {ROOK, WHITE, textures[1]},
        {KNIGHT, WHITE, textures[2]},
        {BISHOP, WHITE, textures[3]},
        {QUEEN, WHITE, textures[4]},
        {KING, WHITE, textures[5]},
        {BISHOP, WHITE, textures[3]},
        {KNIGHT, WHITE, textures[2]},
        {ROOK, WHITE, textures[1]},
        {PAWN, WHITE, textures[0]},
        {PAWN, WHITE, textures[0]},
        {PAWN, WHITE, textures[0]},
        {PAWN, WHITE, textures[0]},
        {PAWN, WHITE, textures[0]},
        {PAWN, WHITE, textures[0]},
        {PAWN, WHITE, textures[0]},
        {PAWN, WHITE, textures[0]}};

    Piece pieseNegre[16] = {
        {ROOK, BLACK, textures[7]},
        {KNIGHT, BLACK, textures[8]},
        {BISHOP, BLACK, textures[9]},
        {QUEEN, BLACK, textures[10]},
        {KING, BLACK, textures[11]},
        {BISHOP, BLACK, textures[9]},
        {KNIGHT, BLACK, textures[8]},
        {ROOK, BLACK, textures[7]},
        {PAWN, BLACK, textures[6]},
        {PAWN, BLACK, textures[6]},
        {PAWN, BLACK, textures[6]},
        {PAWN, BLACK, textures[6]},
        {PAWN, BLACK, textures[6]},
        {PAWN, BLACK, textures[6]},
        {PAWN, BLACK, textures[6]},
        {PAWN, BLACK, textures[6]}};

    // Daca avem selectata optiunea de amestecare
    if (setari.amestecaAlb)
    {
        amestecaPiese(pieseAlbe, 16);
    }

    if (setari.amestecaNegru)
    {
        amestecaPiese(pieseNegre, 16);
    }

    // Plasare piese albe pe primele 2 rânduri
    // Primul rând pentru piesele principale, al doilea pentru pioni
    // Piesele principale (primele 8 din array)
    for (int j = 0; j < BOARD_SIZE; j++)
    {
        (*board)[0][j] = pieseAlbe[j];
    }

    // Pionii (următoarele 8 din array)
    for (int j = 0; j < BOARD_SIZE; j++)
    {
        (*board)[1][j] = pieseAlbe[j + 8];
    }

    // Plasare piese negre pe ultimele 2 rânduri
    // Pionii pe rândul 6, piesele principale pe rândul 7
    // Pionii (ultimele 8 din array)
    for (int j = 0; j < BOARD_SIZE; j++)
    {
        (*board)[6][j] = pieseNegre[j + 8];
    }

    // Piesele principale (primele 8 din array)
    for (int j = 0; j < BOARD_SIZE; j++)
    {
        (*board)[7][j] = pieseNegre[j];
    }

    // Plasare pereti dacă este activa optiunea
    if (setari.adaugaPereti)
    {
        int wallsPlaced = 0;
        while (wallsPlaced < setari.nrPereti)
        {
            int row = 2 + rand() % 4; // poziții 2-5
            int col = rand() % BOARD_SIZE;

            if ((*board)[row][col].type == EMPTY)
            {
                (*board)[row][col].type = WALL;
                (*board)[row][col].color = NONE;
                (*board)[row][col].texture = NULL;
                wallsPlaced++;
            }
        }
    }

    // Activare optiune efect de ceata
    if (setari.efectCeata)
    {
        activeazaEfectCeata(renderer);
    }
}

int estePerete(Board *board, int row, int col)
{
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
    {
        return 0;
    }
    return (*board)[row][col].type == WALL;
}

int esteMutareValidaSpecial(Board *board, const Move *move, int ignoraSah)
{
    // Verificam prima data daca destinatia este un perete
    if ((*board)[move->toX][move->toY].type == WALL)
        return false;

    // Caii pot sa sara peste perete
    if (move->piesa.type == KNIGHT)
        return esteMutareValida(*board, *move, ignoraSah);

    // Pentru restul de piese, verificam daca mai avem si alte piese in cale
    int dx = (move->toX > move->fromX) ? 1 : (move->toX < move->fromX) ? -1
                                                                       : 0;
    int dy = (move->toY > move->fromY) ? 1 : (move->toY < move->fromY) ? -1
                                                                       : 0;

    int x = move->fromX;
    int y = move->fromY;

    //Verifica fiecare patrat din cale, mai putin cel de start
    x += dx;
    y += dy;

    while (x != move->toX || y != move->toY)
    {
        if ((*board)[x][y].type == WALL)
            return false;
        x += dx;
        y += dy;
    }

    // In final, cum stim ca nu avem perete, verifica daca e valida mutarea, conform regulilor de sah
    return esteMutareValida(*board, *move, ignoraSah);
}
