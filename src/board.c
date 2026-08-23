#include <stdio.h>
#include "board.h"
#include "moves.h"
#include "game.h"

// Initializeaza tabla cu spatii goale
void initializeazaTabla(Board board)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j].type = EMPTY;
            board[i][j].color = NONE;
            board[i][j].texture = NULL;
        }
    }
}

// Plaseaza piesele pe pozitiile initiale
void punePiesePeTabla(Board *board, SDL_Renderer *renderer, SDL_Texture *texturi[])
{
    // Piese albe
    (*board)[0][0] = (Piece){ROOK, WHITE, texturi[1]};   // Tura albă (a1)
    (*board)[0][1] = (Piece){KNIGHT, WHITE, texturi[2]}; // Cal alb (b1)
    (*board)[0][2] = (Piece){BISHOP, WHITE, texturi[3]}; // Nebun alb (c1)
    (*board)[0][4] = (Piece){QUEEN, WHITE, texturi[4]};  // Regina albă (d1) - pe câmp alb!
    (*board)[0][3] = (Piece){KING, WHITE, texturi[5]};   // Regele alb (e1) - pe câmp negru!
    (*board)[0][5] = (Piece){BISHOP, WHITE, texturi[3]}; // Nebun alb (f1)
    (*board)[0][6] = (Piece){KNIGHT, WHITE, texturi[2]}; // Cal alb (g1)
    (*board)[0][7] = (Piece){ROOK, WHITE, texturi[1]};   // Tura albă (h1)

    // Pioni albi
    for (int j = 0; j < BOARD_SIZE; j++)
        (*board)[1][j] = (Piece){PAWN, WHITE, texturi[0]};

    // Piese negre
    (*board)[7][0] = (Piece){ROOK, BLACK, texturi[7]};   // Tura neagră (a8)
    (*board)[7][1] = (Piece){KNIGHT, BLACK, texturi[8]}; // Cal negru (b8)
    (*board)[7][2] = (Piece){BISHOP, BLACK, texturi[9]}; // Nebun negru (c8)
    (*board)[7][4] = (Piece){QUEEN, BLACK, texturi[10]}; // Regina neagră (d8) - pe câmp negru!
    (*board)[7][3] = (Piece){KING, BLACK, texturi[11]};  // Regele negru (e8) - pe câmp alb!
    (*board)[7][5] = (Piece){BISHOP, BLACK, texturi[9]}; // Nebun negru (f8)
    (*board)[7][6] = (Piece){KNIGHT, BLACK, texturi[8]}; // Cal negru (g8)
    (*board)[7][7] = (Piece){ROOK, BLACK, texturi[7]};   // Tura neagră (h8)

    // Pioni negri
    for (int j = 0; j < BOARD_SIZE; j++)
        (*board)[6][j] = (Piece){PAWN, BLACK, texturi[6]};
}

// Deseneaza tabla si piesele
void deseneazaTabla(SDL_Renderer *renderer, Board *board, SDL_Texture *texturi[])
{
    // Calculeaza spațiul vertical disponibil pentru tabla, tinand cont de status bar
    int boardAreaHeight = SCREEN_HEIGHT - STATUS_BAR_HEIGHT;
    int verticalPadding = (boardAreaHeight - (BOARD_SIZE * TILE_SIZE)) / 2;

    // Deseneaza patratele tablei
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            SDL_Rect patrat = {
                j * TILE_SIZE + UI_PADDING,
                i * TILE_SIZE + verticalPadding,
                TILE_SIZE,
                TILE_SIZE};

            SDL_SetRenderDrawColor(renderer,
                                   (i + j) % 2 == 0 ? LIGHT_CREAM_R : TROLLEY_GREY_R,
                                   (i + j) % 2 == 0 ? LIGHT_CREAM_G : TROLLEY_GREY_G,
                                   (i + j) % 2 == 0 ? LIGHT_CREAM_B : TROLLEY_GREY_B,
                                   255);
            SDL_RenderFillRect(renderer, &patrat);
        }
    }

    // Deseneaza piesele
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if ((*board)[i][j].type != EMPTY)
            {
                SDL_Rect pieceRect = {
                    j * TILE_SIZE + UI_PADDING,
                    i * TILE_SIZE + verticalPadding,
                    TILE_SIZE,
                    TILE_SIZE};
                SDL_RenderCopy(renderer, (*board)[i][j].texture, NULL, &pieceRect);
            }
        }
    }
}

// Evidentiază un patrat cu transparenta
void evidentiazaPatrat(SDL_Renderer *renderer, int rand, int coloana, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    // Calculeaza spatiul vertical disponibil pentru tabla, tinand cont de status bar
    int boardAreaHeight = SCREEN_HEIGHT - STATUS_BAR_HEIGHT;
    int verticalPadding = (boardAreaHeight - (BOARD_SIZE * TILE_SIZE)) / 2;

    SDL_Rect patrat = {
        coloana * TILE_SIZE + UI_PADDING,
        rand * TILE_SIZE + verticalPadding,
        TILE_SIZE,
        TILE_SIZE};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // Activează transparenta
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &patrat);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE); // Dezactiveaza transparenta
}

void copiazaTabla(Board sursa, Board destinatie)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            destinatie[i][j] = sursa[i][j];
        }
    }
}

void deseneazaTablaSpeciala(SDL_Renderer *renderer, Board *board, SDL_Texture *textures[], SDL_Texture *wallTexture)
{
    // Calculeaza spatiul vertical disponibil pentru tabla, tinand cont de status bar
    int boardAreaHeight = SCREEN_HEIGHT - STATUS_BAR_HEIGHT;
    int verticalPadding = (boardAreaHeight - (BOARD_SIZE * TILE_SIZE)) / 2;

    // Deseneaza tabla speciala - cu tema de întuneric pentru modul Teroarea Nocturna
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            SDL_Rect rect = {
                j * TILE_SIZE + UI_PADDING,
                i * TILE_SIZE + verticalPadding,
                TILE_SIZE,
                TILE_SIZE};

            // Tema cu nuante de negru și rosu inchis
            if ((i + j) % 2 == 0)
            {
                SDL_SetRenderDrawColor(renderer, 40, 20, 25, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 15, 10, 12, 255);
            }

            SDL_RenderFillRect(renderer, &rect);

            // Contur intunecat pentru patratele tablei
            SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &rect);

            // Conturul albastru pentru piesele albe
            if ((*board)[i][j].type != EMPTY && (*board)[i][j].color == WHITE)
            {
                // Contur albastru mai subtil pentru pătratele cu piese albe
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 50, 100, 255, 120); // Albastru semi-transparent
                SDL_RenderDrawRect(renderer, &rect);
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            }

            if ((*board)[i][j].type == WALL)
            {
              
                SDL_SetRenderDrawColor(renderer, 120, 30, 30, 255); 
                SDL_RenderFillRect(renderer, &rect);

                //Contur maro-roscat pentru perete
                SDL_SetRenderDrawColor(renderer, 170, 50, 30, 255);
                SDL_RenderDrawRect(renderer, &rect);

               //Efect de caramida 
                SDL_SetRenderDrawColor(renderer, 90, 20, 20, 255); 

                // Adaugare linii-orizontale pentru caramizile peretilor
                int lineSpacing = TILE_SIZE / 4;
                for (int line = 1; line < 4; line++)
                {
                    SDL_RenderDrawLine(
                        renderer,
                        rect.x, rect.y + line * lineSpacing,
                        rect.x + rect.w, rect.y + line * lineSpacing);
                }

                // Linii verticale pentru cărămizi 
                for (int line = 0; line < 2; line++)
                {
                    // Prima linie verticală la 1/3
                    SDL_RenderDrawLine(
                        renderer,
                        rect.x + (line * 2 + 1) * TILE_SIZE / 3, rect.y,
                        rect.x + (line * 2 + 1) * TILE_SIZE / 3, rect.y + TILE_SIZE / 4);

                    // A doua linie verticală la 2/3 dar în a doua secțiune
                    SDL_RenderDrawLine(
                        renderer,
                        rect.x + (line + 1) * TILE_SIZE / 3, rect.y + TILE_SIZE / 4,
                        rect.x + (line + 1) * TILE_SIZE / 3, rect.y + TILE_SIZE / 2);

                    SDL_RenderDrawLine(
                        renderer,
                        rect.x + (line * 2 + 1) * TILE_SIZE / 3, rect.y + TILE_SIZE / 2,
                        rect.x + (line * 2 + 1) * TILE_SIZE / 3, rect.y + 3 * TILE_SIZE / 4);

                    SDL_RenderDrawLine(
                        renderer,
                        rect.x + (line + 1) * TILE_SIZE / 3, rect.y + 3 * TILE_SIZE / 4,
                        rect.x + (line + 1) * TILE_SIZE / 3, rect.y + TILE_SIZE);
                }
            }
        }
    }

    //Deseneaza piesele speciale
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if ((*board)[i][j].type != EMPTY && (*board)[i][j].type != WALL)
            {
                SDL_Rect pieceRect = {
                    j * TILE_SIZE + UI_PADDING,
                    i * TILE_SIZE + verticalPadding,
                    TILE_SIZE,
                    TILE_SIZE};

                if ((*board)[i][j].texture != NULL)
                {
                    if ((*board)[i][j].color == WHITE)
                    {
                        // Piese albe cu o tentă de albastru palid (aspect de fantomă)
                        SDL_SetTextureColorMod((*board)[i][j].texture, 180, 180, 230);

                        // Adăugăm un efect de glow pentru piesele albe
                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

                        // Glow albastru pentru piesele albe - subtil
                        SDL_SetRenderDrawColor(renderer, 100, 130, 255, 40);

                        // Cream dreptunghiuri pentru efectul de glow
                        SDL_Rect glowRect = {
                            pieceRect.x - 4,
                            pieceRect.y - 4,
                            pieceRect.w + 8,
                            pieceRect.h + 8};
                        SDL_RenderFillRect(renderer, &glowRect);

                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
                    }
                    else
                    {
                        // Piese negre cu un efect similar de glow, dar în rosu
                        SDL_SetTextureColorMod((*board)[i][j].texture, 230, 30, 30);

                        // Adaugam un efect de glow pentru piesele negre similar cu cele albe
                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

                        // Primul strat de glow - mai mare și mai subtil
                        SDL_SetRenderDrawColor(renderer, 255, 30, 30, 25); // Rosu subtil
                        SDL_Rect outerGlowRect = {
                            pieceRect.x - 6,
                            pieceRect.y - 6,
                            pieceRect.w + 12,
                            pieceRect.h + 12};
                        SDL_RenderFillRect(renderer, &outerGlowRect);

                        // Al doilea strat de glow - mai intens
                        SDL_SetRenderDrawColor(renderer, 255, 40, 0, 40); // Rosu-portocaliu mai intens
                        SDL_Rect glowRect = {
                            pieceRect.x - 4,
                            pieceRect.y - 4,
                            pieceRect.w + 8,
                            pieceRect.h + 8};
                        SDL_RenderFillRect(renderer, &glowRect);

                        // Contur pentru piesele negre
                        SDL_SetRenderDrawColor(renderer, 255, 50, 20, 150);
                        SDL_Rect outlineRect = {
                            pieceRect.x - 1,
                            pieceRect.y - 1,
                            pieceRect.w + 2,
                            pieceRect.h + 2};
                        SDL_RenderDrawRect(renderer, &outlineRect);

                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
                    }

                    // Desenează piesele
                    SDL_RenderCopy(renderer, (*board)[i][j].texture, NULL, &pieceRect);

                    SDL_SetTextureColorMod((*board)[i][j].texture, 255, 255, 255);
                }
            }
        }
    }
}
