#include "promovare.h"
#include "graphics.h" // pentru TILE_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT
#include <SDL_ttf.h>

TipPromovare afiseazaMeniuPromovare(SDL_Renderer *renderer, SDL_Texture *textures[12], PieceColor culoare)
{
    const int latimePiesa = TILE_SIZE;
    const int inaltimePiesa = TILE_SIZE;
    const int distantaIntre = 20;

    SDL_Rect optiuni[4];
    int totalLatime = 4 * latimePiesa + 3 * distantaIntre;
    int startX = (SCREEN_WIDTH - totalLatime) / 2;
    int startY = (SCREEN_HEIGHT - TILE_SIZE) / 2;

    // Ordinea: Regina, Cal, Tura, Nebun
    for (int i = 0; i < 4; i++)
    {
        optiuni[i].x = startX + i * (latimePiesa + distantaIntre);
        optiuni[i].y = startY;
        optiuni[i].w = latimePiesa;
        optiuni[i].h = inaltimePiesa;
    }

    // Inițializare font
    TTF_Font *font = TTF_OpenFont("assets/fonts/OpenSans.ttf", 24);
    if (!font)
    {
        printf("Eroare la încărcarea fontului: %s\n", TTF_GetError());
        return 0;
    }

    // Alege culoarea textului în functie de culoarea piesei
    SDL_Color culoareText;
    if (culoare == WHITE)
    {
        culoareText = (SDL_Color){255, 255, 255, 255}; // alb
    }
    else
    {
        culoareText = (SDL_Color){0, 0, 0, 255}; // negru
    }

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, "Alege o piesa pentru promovare", culoareText);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    textRect.x = (SCREEN_WIDTH - textRect.w) / 2;
    textRect.y = startY - 40;

    SDL_FreeSurface(textSurface);

    SDL_Event event;
    while (1)
    {
        SDL_RenderPresent(renderer);

        // Afiseaza textul
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        // Afiseaza piesele
        int indexuri[4];
        if (culoare == WHITE)
        {
            indexuri[0] = 4; // Regina alba
            indexuri[1] = 2; // Cal alb
            indexuri[2] = 1; // Tura alba
            indexuri[3] = 3; // Nebun alb
        }
        else
        {
            indexuri[0] = 10; // Regina neagra
            indexuri[1] = 8;  // Cal negru
            indexuri[2] = 7;  // Tura neagra
            indexuri[3] = 9;  // Nebun negru
        }

        for (int i = 0; i < 4; i++)
        {
            SDL_RenderCopy(renderer, textures[indexuri[i]], NULL, &optiuni[i]);
        }
        SDL_RenderPresent(renderer);

        while (SDL_WaitEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x;
                int y = event.button.y;
                for (int i = 0; i < 4; i++)
                {
                    if (x >= optiuni[i].x && x <= optiuni[i].x + optiuni[i].w &&
                        y >= optiuni[i].y && y <= optiuni[i].y + optiuni[i].h)
                    {
                        SDL_DestroyTexture(textTexture);
                        TTF_CloseFont(font);

                        switch (i)
                        {
                        case 0:
                            return PROMOVARE_REGINA;
                        case 1:
                            return PROMOVARE_CAL;
                        case 2:
                            return PROMOVARE_TURA;
                        case 3:
                            return PROMOVARE_NEBUN;
                        }
                    }
                }
            }
        }
    }
}

void promoveazaPion(Board board, int rand, int coloana, PieceColor culoare, TipPromovare tip, SDL_Texture *textures[])
{
    switch (tip)
    {
    case PROMOVARE_REGINA:
        board[rand][coloana] = (Piece){QUEEN, culoare, culoare == WHITE ? textures[4] : textures[10]};
        break;
    case PROMOVARE_TURA:
        board[rand][coloana] = (Piece){ROOK, culoare, culoare == WHITE ? textures[1] : textures[7]};
        break;
    case PROMOVARE_NEBUN:
        board[rand][coloana] = (Piece){BISHOP, culoare, culoare == WHITE ? textures[3] : textures[9]};
        break;
    case PROMOVARE_CAL:
        board[rand][coloana] = (Piece){KNIGHT, culoare, culoare == WHITE ? textures[2] : textures[8]};
        break;
    }
}
