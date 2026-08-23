#include "ai.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

// Tabele scor-piesa pentru evaluarea pozitionala

// Values copied from simplified chess programming theory
const int pawnTable[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 10, 20, 30, 30, 20, 10, 10},
    {5, 5, 10, 25, 25, 10, 5, 5},
    {0, 0, 0, 20, 20, 0, 0, 0},
    {5, -5, -10, 0, 0, -10, -5, 5},
    {5, 10, 10, -20, -20, 10, 10, 5},
    {0, 0, 0, 0, 0, 0, 0, 0}};

const int knightTable[8][8] = {
    {-50, -40, -30, -30, -30, -30, -40, -50},
    {-40, -20, 0, 0, 0, 0, -20, -40},
    {-30, 0, 10, 15, 15, 10, 0, -30},
    {-30, 5, 15, 20, 20, 15, 5, -30},
    {-30, 0, 15, 20, 20, 15, 0, -30},
    {-30, 5, 10, 15, 15, 10, 5, -30},
    {-40, -20, 0, 5, 5, 0, -20, -40},
    {-50, -40, -30, -30, -30, -30, -40, -50}};

const int bishopTable[8][8] = {
    {-20, -10, -10, -10, -10, -10, -10, -20},
    {-10, 0, 0, 0, 0, 0, 0, -10},
    {-10, 0, 10, 10, 10, 10, 0, -10},
    {-10, 5, 5, 10, 10, 5, 5, -10},
    {-10, 0, 5, 10, 10, 5, 0, -10},
    {-10, 5, 5, 5, 5, 5, 5, -10},
    {-10, 0, 5, 0, 0, 5, 0, -10},
    {-20, -10, -10, -10, -10, -10, -10, -20}};

const int rookTable[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {5, 10, 10, 10, 10, 10, 10, 5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {0, 0, 0, 5, 5, 0, 0, 0}};

const int queenTable[8][8] = {
    {-20, -10, -10, -5, -5, -10, -10, -20},
    {-10, 0, 0, 0, 0, 0, 0, -10},
    {-10, 0, 5, 5, 5, 5, 0, -10},
    {-5, 0, 5, 5, 5, 5, 0, -5},
    {0, 0, 5, 5, 5, 5, 0, -5},
    {-10, 5, 5, 5, 5, 5, 0, -10},
    {-10, 0, 5, 0, 0, 0, 0, -10},
    {-20, -10, -10, -5, -5, -10, -10, -20}};

const int kingMiddleGameTable[8][8] = {
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-20, -30, -30, -40, -40, -30, -30, -20},
    {-10, -20, -20, -20, -20, -20, -20, -10},
    {20, 20, 0, 0, 0, 0, 20, 20},
    {20, 30, 10, 0, 0, 10, 30, 20}};

const int kingEndGameTable[8][8] = {
    {-50, -40, -30, -20, -20, -30, -40, -50},
    {-30, -20, -10, 0, 0, -10, -20, -30},
    {-30, -10, 20, 30, 30, 20, -10, -30},
    {-30, -10, 30, 40, 40, 30, -10, -30},
    {-30, -10, 30, 40, 40, 30, -10, -30},
    {-30, -10, 20, 30, 30, 20, -10, -30},
    {-30, -30, 0, 0, 0, 0, -30, -30},
    {-50, -30, -30, -30, -30, -30, -30, -50}};

// Valoarea materiala a fiecarei piese
int evalueazaPiesa(PieceType type)
{
    switch (type)
    {
    case PAWN:
        return 100;
    case KNIGHT:
        return 320;
    case BISHOP:
        return 330;
    case ROOK:
        return 500;
    case QUEEN:
        return 900;
    case KING:
        return 20000;
    default:
        return 0;
    }
}

// Obține scorul pozițional pentru o piesă aflată pe o poziție dată
int getPositionalScore(PieceType type, int row, int col, int isEndgame, PieceColor color)
{
    // Inverseaza pozitia pentru piesele negre
    if (color == BLACK)
    {
        row = 7 - row;
        col = 7 - col;
    }

    switch (type)
    {
    case PAWN:
        return pawnTable[row][col];
    case KNIGHT:
        return knightTable[row][col];
    case BISHOP:
        return bishopTable[row][col];
    case ROOK:
        return rookTable[row][col];
    case QUEEN:
        return queenTable[row][col];
    case KING:
        if (isEndgame)
            return kingEndGameTable[row][col];
        else
            return kingMiddleGameTable[row][col];
    default:
        return 0;
    }
}

// Calculează valoarea totală a materialului de pe tablă pentru a determina dacă suntem în final de joc (endgame)
int evaluateGamePhase(Board board)
{
    int material = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j].type != EMPTY && board[i][j].type != KING)
            {
                material += evalueazaPiesa(board[i][j].type);
            }
        }
    }

    // Considerăm că suntem în endgame dacă valoarea totală a materialului este sub un anumit prag
    // Aceasta este:  material total < valoarea a 3 regine

    return material < 2700;
}

//Functie de evaluare imbunatatita pentru tabla
int evalueazaTabla(Board board, PieceColor computerColor)
{
    int scor = 0;
    int isEndgame = evaluateGamePhase(board);

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j].type != EMPTY)
            {
                // Material value
                int valoarePiesa = evalueazaPiesa(board[i][j].type);

                // Positional value
                int valoarePozitionala = getPositionalScore(board[i][j].type, i, j, isEndgame, board[i][j].color);

                // Add both values with the appropriate sign
                if (board[i][j].color == computerColor)
                {
                    scor += valoarePiesa + valoarePozitionala;
                }
                else
                {
                    scor -= valoarePiesa + valoarePozitionala;
                }
            }
        }
    }

    return scor;
}

// Genereaza toate mutarile posibile pentru un jucator
void genereazaMutariPosibile(Board board, PieceColor color, Move mutari[], int *numMutari)
{
    *numMutari = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j].type != EMPTY && board[i][j].color == color)
            {
                for (int x = 0; x < BOARD_SIZE; x++)
                {
                    for (int y = 0; y < BOARD_SIZE; y++)
                    {
                        Move mutare = {i, j, x, y, board[i][j], board[x][y], 0};

                        if (esteMutareValida(board, mutare, 0))
                        {
                            mutari[*numMutari] = mutare;
                            (*numMutari)++;
                        }
                    }
                }
            }
        }
    }
}

// Inbunatatim eficienta algoritmului Minimax
void ordoneazaMutari(Move mutari[], int numMutari)
{
    // Folosim un simplu bubble-sort pentru a prioritiza mutarile
    for (int i = 0; i < numMutari - 1; i++)
    {
        for (int j = 0; j < numMutari - i - 1; j++)
        {
            if (mutari[j].piesaCapturata.type == EMPTY &&
                mutari[j + 1].piesaCapturata.type != EMPTY)
            {
                Move temp = mutari[j];
                mutari[j] = mutari[j + 1];
                mutari[j + 1] = temp;
            }
        }
    }
}

int minimax(Board board, int depth, int alpha, int beta, PieceColor jucator, PieceColor computerColor)
{
    if (depth == 0)
    {
        return evalueazaTabla(board, computerColor);
    }

    Move mutari[218]; // Este maximul de mutari posibile in sah 
    int numMutari = 0;
    genereazaMutariPosibile(board, jucator, mutari, &numMutari);

    // Verifica daca e sah-mat sau remiza
    if (numMutari == 0)
    {
        if (esteSah(board, jucator))
        {
            // Sah-mat – cel mai rau rezultat posibil daca e randul calculatorului
            return (jucator == computerColor) ? -30000 : 30000;
        }
        else
        {
            // Remiza – niciun jucator nu poate castiga
            return 0;
        }
    }

    // Ordoneaza mutarile pentru eficienta
    ordoneazaMutari(mutari, numMutari);

    if (jucator == computerColor)
    {
        int bestVal = INT_MIN;
        for (int i = 0; i < numMutari; i++)
        {
            // Fa mutare
            Piece capturata = board[mutari[i].toX][mutari[i].toY];
            board[mutari[i].toX][mutari[i].toY] = board[mutari[i].fromX][mutari[i].fromY];
            board[mutari[i].fromX][mutari[i].fromY].type = EMPTY;

            // Evaluare recursiva
            int val = minimax(board, depth - 1, alpha, beta, (jucator == WHITE) ? BLACK : WHITE, computerColor);

            // Revino la mutare(UNDO Move)
            board[mutari[i].fromX][mutari[i].fromY] = board[mutari[i].toX][mutari[i].toY];
            board[mutari[i].toX][mutari[i].toY] = capturata;

            bestVal = (val > bestVal) ? val : bestVal;
            alpha = (alpha > bestVal) ? alpha : bestVal;

            if (beta <= alpha)
                break; // Taiere Beta
        }
        return bestVal;
    }
    else
    {
        int bestVal = INT_MAX;
        for (int i = 0; i < numMutari; i++)
        {
            // Fa mutare
            Piece capturata = board[mutari[i].toX][mutari[i].toY];
            board[mutari[i].toX][mutari[i].toY] = board[mutari[i].fromX][mutari[i].fromY];
            board[mutari[i].fromX][mutari[i].fromY].type = EMPTY;

            // Evaluare recursiva
            int val = minimax(board, depth - 1, alpha, beta, (jucator == WHITE) ? BLACK : WHITE, computerColor);

            // Undo la mutare
            board[mutari[i].fromX][mutari[i].fromY] = board[mutari[i].toX][mutari[i].toY];
            board[mutari[i].toX][mutari[i].toY] = capturata;

            bestVal = (val < bestVal) ? val : bestVal;
            beta = (beta < bestVal) ? beta : bestVal;

            if (beta <= alpha)
                break; // Taire partea alpha
        }
        return bestVal;
    }
}

// Gaseste cea mai buna mutare folosind minimax cu taiere alpha-beta
Move gasesteCeaMaiBunaMutare(Board board, PieceColor computerColor, int depth)
{
    Move bestMove = {-1, -1, -1, -1, {EMPTY, WHITE}, {EMPTY, WHITE}, INT_MIN};
    int bestVal = INT_MIN;
    int alpha = INT_MIN;
    int beta = INT_MAX;

    Move mutari[218]; // Maximul posibil din sah, am scris si mai sus
    int numMutari = 0;
    genereazaMutariPosibile(board, computerColor, mutari, &numMutari);

    // Order moves to improve pruning
    ordoneazaMutari(mutari, numMutari);

    // Daca nu exista nicio mutare valida, returneaza o mutare invalida
    if (numMutari == 0)
    {
        return bestMove;
    }

    for (int i = 0; i < numMutari; i++)
    {
        // Fa mutare
        Piece capturata = board[mutari[i].toX][mutari[i].toY];
        board[mutari[i].toX][mutari[i].toY] = board[mutari[i].fromX][mutari[i].fromY];
        board[mutari[i].fromX][mutari[i].fromY].type = EMPTY;

        // Evaluaeaza mutare
        int moveVal = minimax(board, depth - 1, alpha, beta, (computerColor == WHITE) ? BLACK : WHITE, computerColor);

        // Undo la mutare
        board[mutari[i].fromX][mutari[i].fromY] = board[mutari[i].toX][mutari[i].toY];
        board[mutari[i].toX][mutari[i].toY] = capturata;

        // Actualizeaza cea mai buna mutare, daca este cazul
        if (moveVal > bestVal)
        {
            bestVal = moveVal;
            bestMove = mutari[i];
            bestMove.score = bestVal;
        }

        alpha = (alpha > bestVal) ? alpha : bestVal;
    }

    return bestMove;
}