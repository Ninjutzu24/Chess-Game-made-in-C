#include "board.h"
#include "piece.h"

int canMovePawn(Board board, int startRow, int startCol, int endRow, int endCol)
{
    if (startCol == endCol && board[startRow][endCol].type == EMPTY)
    {
        return 1;
    }
    return 0;
}

int canMoveRook(Board board, int startRow, int startCol, int endRow, int endCol)
{

    if (startCol == endCol)
    {

        for (int row = startRow + 1; row < endRow; row++)
        {
            if (board[row][startCol].type != EMPTY)
                return 0;
        }
    }
    else if (startRow == endRow)
    {

        for (int col = startCol + 1; col < endCol; col++)
        {
            if (board[startRow][col].type != EMPTY)
                return 0;
        }
    }
    return 1;
}

int canMoveBishop(Board board, int startRow, int startCol, int endRow, int endCol)
{
    int row = startRow;
    int col = startCol;

    while (row != endRow && col != endCol)
    {
        row += (endRow > startRow) ? 1 : -1;
        col += (endCol > startCol) ? 1 : -1;

        if (board[row][col].type != EMPTY)
            return 0;
    }
    return 1;
}
