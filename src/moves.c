
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "moves.h"
#include "promovare.h"

int verificaPromovarePion(Board board, Move mutare)
{
    // Pionul alb a ajuns pe ultimul rand (7)
    if (mutare.piesa.color == WHITE && mutare.toX == 7)
    {
        return 1;
    }
    // Pionul negru a ajuns pe primul rand (0)
    if (mutare.piesa.color == BLACK && mutare.toX == 0)
    {
        return 1;
    }
    return 0;
}

int esteMutarePionValida(Board board, Move mutare)
{
    int directie = (mutare.piesa.color == WHITE) ? 1 : -1;
    int randStart = (mutare.piesa.color == WHITE) ? 1 : 6;

    // Mișcare înainte
    if (mutare.fromY == mutare.toY)
    {
        // Un pas
        if (mutare.toX == mutare.fromX + directie && board[mutare.toX][mutare.toY].type == EMPTY)
        {
            return true;
        }
        // Doi pași de la poziția inițială
        if (mutare.fromX == randStart && mutare.toX == mutare.fromX + 2 * directie &&
            board[mutare.fromX + directie][mutare.fromY].type == EMPTY &&
            board[mutare.toX][mutare.toY].type == EMPTY)
        {
            return true;
        }
    }
    // Captură
    else if (abs(mutare.toY - mutare.fromY) == 1 && mutare.toX == mutare.fromX + directie &&
             board[mutare.toX][mutare.toY].type != EMPTY &&
             board[mutare.toX][mutare.toY].color != mutare.piesa.color)
    {
        return true;
    }
    return false;
}

int esteMutareTuraValida(Board board, Move mutare)
{
    // Mișcare pe linie dreaptă
    if (mutare.fromX != mutare.toX && mutare.fromY != mutare.toY)
        return false;

    int pas;
    if (mutare.fromX == mutare.toX)
    { // Orizontal
        pas = (mutare.toY > mutare.fromY) ? 1 : -1;
        for (int y = mutare.fromY + pas; y != mutare.toY; y += pas)
        {
            if (board[mutare.fromX][y].type != EMPTY)
                return false;
        }
    }
    else
    { // Vertical
        pas = (mutare.toX > mutare.fromX) ? 1 : -1;
        for (int x = mutare.fromX + pas; x != mutare.toX; x += pas)
        {
            if (board[x][mutare.fromY].type != EMPTY)
                return false;
        }
    }
    return true;
}

int esteMutareCalValida(Board board, Move mutare)
{
    int dx = abs(mutare.toX - mutare.fromX);
    int dy = abs(mutare.toY - mutare.fromY);
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

int esteMutareNebunValida(Board board, Move mutare)
{
    if (abs(mutare.toX - mutare.fromX) != abs(mutare.toY - mutare.fromY))
        return false;

    int pasX = (mutare.toX > mutare.fromX) ? 1 : -1;
    int pasY = (mutare.toY > mutare.fromY) ? 1 : -1;

    for (int x = mutare.fromX + pasX, y = mutare.fromY + pasY;
         x != mutare.toX;
         x += pasX, y += pasY)
    {
        if (board[x][y].type != EMPTY)
            return false;
    }
    return true;
}

int esteMutareReginaValida(Board board, Move mutare)
{
    return esteMutareTuraValida(board, mutare) || esteMutareNebunValida(board, mutare);
}

int esteMutareRegeValida(Board board, Move mutare)
{
    int dx = abs(mutare.toX - mutare.fromX);
    int dy = abs(mutare.toY - mutare.fromY);
    return (dx <= 1 && dy <= 1);
}

int esteMutareValida(Board board, Move mutare, int ignoraSah)
{
    // Verifică limitele tablei
    if (mutare.fromX < 0 || mutare.fromX >= 8 || mutare.fromY < 0 || mutare.fromY >= 8 ||
        mutare.toX < 0 || mutare.toX >= 8 || mutare.toY < 0 || mutare.toY >= 8)
        return false;

    Piece piesa = board[mutare.fromX][mutare.fromY];
    Piece tinta = board[mutare.toX][mutare.toY];

    // Nu poți muta un spațiu gol sau captura piesa ta
    if (piesa.type == EMPTY || (tinta.type != EMPTY && tinta.color == piesa.color))
        return false;

    // Verifică regulile specifice piesei
    int valida = 0;
    switch (piesa.type)
    {
    case PAWN:
        valida = esteMutarePionValida(board, mutare);
        break;
    case ROOK:
        valida = esteMutareTuraValida(board, mutare);
        break;
    case KNIGHT:
        valida = esteMutareCalValida(board, mutare);
        break;
    case BISHOP:
        valida = esteMutareNebunValida(board, mutare);
        break;
    case QUEEN:
        valida = esteMutareReginaValida(board, mutare);
        break;
    case KING:
        valida = esteMutareRegeValida(board, mutare);
        break;
    default:
        valida = 0;
    }

    if (!valida)
        return false;

    // Verificare specială pentru șah
    if (!ignoraSah)
    {
        // Facem mutarea temporar
        Piece capturata = board[mutare.toX][mutare.toY];
        board[mutare.toX][mutare.toY] = piesa;
        board[mutare.fromX][mutare.fromY].type = EMPTY;

        // Verificam dacă regele este în sah
        int inSah = esteSah(board, piesa.color);

        // Anulam mutarea temporara
        board[mutare.fromX][mutare.fromY] = piesa;
        board[mutare.toX][mutare.toY] = capturata;

        if (inSah)
        {
            return false;
        }
    }

    return true;
}

void executaMutare(Board board, Move mutare, SDL_Texture *textures[])
{
    if (!esteMutareValida(board, mutare, 0))
        return;

    mutare.piesaCapturata = board[mutare.toX][mutare.toY];
    board[mutare.toX][mutare.toY] = mutare.piesa;
    board[mutare.fromX][mutare.fromY].type = EMPTY;

    // Verifica promovare pion
    if (mutare.piesa.type == PAWN && verificaPromovarePion(board, mutare))
    {
        // Implicit devine regina, dar poți implementa interfata pentru alegere
        promoveazaPion(board, mutare.toX, mutare.toY, mutare.piesa.color, PROMOVARE_REGINA, textures);
    }
}

void anuleazaMutare(Board board, Move mutare)
{
    board[mutare.fromX][mutare.fromY] = mutare.piesa;
    board[mutare.toX][mutare.toY] = mutare.piesaCapturata;
}

int esteSah(Board board, PieceColor culoareJucator)
{
    int randRege = -1, coloanaRege = -1;

    // Gaseste pozitia regelui
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j].type == KING && board[i][j].color == culoareJucator)
            {
                randRege = i;
                coloanaRege = j;
                break;
            }
        }
    }

    if (randRege == -1)
        return false;

    // Verifica daca vreo piesa adversa poate ataca regele
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j].type != EMPTY && board[i][j].color != culoareJucator)
            {
                Move atac = {i, j, randRege, coloanaRege, board[i][j], board[randRege][coloanaRege]};
                if (esteMutareValida(board, atac, 1))
                { // Ignora sahul pentru a evita recursivitate infinită
                    return true;
                }
            }
        }
    }
    return false;
}

int esteSahMat(Board board, PieceColor culoareJucator)
{
    if (!esteSah(board, culoareJucator))
        return false;

    // Verific toate mutarile posibile
    for (int deLaRand = 0; deLaRand < 8; deLaRand++)
    {
        for (int deLaColoana = 0; deLaColoana < 8; deLaColoana++)
        {
            if (board[deLaRand][deLaColoana].color != culoareJucator)
                continue;

            for (int laRand = 0; laRand < 8; laRand++)
            {
                for (int laColoana = 0; laColoana < 8; laColoana++)
                {
                    Move mutare = {deLaRand, deLaColoana, laRand, laColoana,
                                   board[deLaRand][deLaColoana], board[laRand][laColoana]};
                    if (esteMutareValida(board, mutare, 0))
                    {
                        // Am gasit cel putin o mutare valida care scapa de sah
                        return false;
                    }
                }
            }
        }
    }

    // Nicio mutare nu scapă de sah
    return true;
}
