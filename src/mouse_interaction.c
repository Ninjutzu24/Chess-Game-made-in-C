#include "mouse_interaction.h"
#include "moves.h"
#include <stdbool.h>

void obtineMutariValide(Board board, int rand, int coloana, int mutariValide[BOARD_SIZE][BOARD_SIZE])
{
    if (rand < 0 || rand >= BOARD_SIZE || coloana < 0 || coloana >= BOARD_SIZE)
        return;

    if (board[rand][coloana].type == EMPTY)
        return;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            mutariValide[i][j] = 0;
            Move mutare = {rand, coloana, i, j, board[rand][coloana], board[i][j]};
            if (esteMutareValida(board, mutare, 0))
            {
                mutariValide[i][j] = 1;
            }
        }
    }
}

void evidentiazaMutariValide(SDL_Renderer *renderer, Board board, int rand, int coloana)
{
    if (rand < 0 || rand >= BOARD_SIZE || coloana < 0 || coloana >= BOARD_SIZE)
        return;

    if (board[rand][coloana].type == EMPTY)
        return;

    // Highlight la piesa selectata
    evidentiazaPatrat(renderer, rand, coloana, 100, 255, 100, 128);

    // Highlight la mutarile valide
    int mutariValide[BOARD_SIZE][BOARD_SIZE];
    obtineMutariValide(board, rand, coloana, mutariValide);

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (mutariValide[i][j])
            {
                if (board[i][j].type == EMPTY)
                {
                    // Daca r patrat gol - highlight albastru
                    evidentiazaPatrat(renderer, i, j, 100, 200, 255, 128);
                }
                else
                {
                    // Daca putem captura ceva - highlight rosu
                    evidentiazaPatrat(renderer, i, j, 255, 100, 100, 128);
                } 
            }
        }
    }
}