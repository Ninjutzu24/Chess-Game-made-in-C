#include "game.h"
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture *fundal_meniu = NULL;
Buton butoane[4];
Buton butoane_dificultate[3];
Buton buton_back;
NivelDificultateAI nivelDificultateAI = DIFICULTATE_MEDIE;

int loadPieceTextures(SDL_Renderer *renderer, SDL_Texture *textures[])
{
    const char *piece_files[12] = {
        "assets/pieces/wp1.png", "assets/pieces/wn1.png", "assets/pieces/wb1.png",
        "assets/pieces/wr1.png", "assets/pieces/wq1.png", "assets/pieces/wk1.png",
        "assets/pieces/bp1.png", "assets/pieces/bn1.png", "assets/pieces/bb1.png",
        "assets/pieces/br1.png", "assets/pieces/bq1.png", "assets/pieces/bk1.png"};

    for (int i = 0; i < 12; i++)
    {
        SDL_Surface *surface = IMG_Load(piece_files[i]);
        if (!surface)
        {
            printf("Failed to load image %s: %s\n", piece_files[i], IMG_GetError());
            return 0;
        }
        textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!textures[i])
        {
            printf("Failed to create texture: %s\n", SDL_GetError());
            return 0;
        }
    }
    return 1;
}

void initializeaza_meniu(SDL_Renderer *renderer)
{
    const char *butoane_img[4] = {
        "assets/ui/1_vs_1.png",
        "assets/ui/joaca_vs_calculator.png",
        "assets/ui/Special.png",
        "assets/ui/exit.png"};

    for (int i = 0; i < 4; i++)
    {
        SDL_Surface *suprafata = IMG_Load(butoane_img[i]);
        if (!suprafata)
        {
            printf("Failed to load button image: %s\n", IMG_GetError());
            continue;
        }
        butoane[i].textura = SDL_CreateTextureFromSurface(renderer, suprafata);
        SDL_FreeSurface(suprafata);

        // Butoane mult mai înalte pentru a ocupa mai mult spatiu pe lungime
        butoane[i].rect = (SDL_Rect){
            (SCREEN_WIDTH - 400) / 2, 
            30 + i * 150,           
            400,                     
            150};                    
    }
}

void initializeaza_fundal_meniu(SDL_Renderer *renderer)
{
    SDL_Surface *suprafata = IMG_Load("assets/ui/fundal.png");
    if (!suprafata)
    {
        printf("Failed to load background image: %s\n", IMG_GetError());
        return;
    }
    fundal_meniu = SDL_CreateTextureFromSurface(renderer, suprafata);
    SDL_FreeSurface(suprafata);
}

void initializeaza_buton_back(SDL_Renderer *renderer)
{
    SDL_Surface *suprafata = IMG_Load("assets/ui/back.png");
    if (!suprafata)
    {
        printf("Failed to load back button: %s\n", IMG_GetError());
        return;
    }
    buton_back.textura = SDL_CreateTextureFromSurface(renderer, suprafata);
    SDL_FreeSurface(suprafata);


    buton_back.rect = (SDL_Rect){10, SCREEN_HEIGHT - STATUS_BAR_HEIGHT / 2 - 15, 90, 30};
}

void initializeaza_meniu_dificultate(SDL_Renderer *renderer)
{
    TTF_Font *font = TTF_OpenFont("assets/fonts/OpenSans.ttf", 36);
    if (!font)
    {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    const char *texte_dificultate[3] = {
        "Ușor",
        "Mediu",
        "Dificil"};

    SDL_Color culoare_text = {230, 214, 144, 255};

    for (int i = 0; i < 3; i++)
    {
        SDL_Surface *suprafata = TTF_RenderUTF8_Blended(font, texte_dificultate[i], culoare_text);
        if (!suprafata)
        {
            printf("Failed to render text: %s\n", TTF_GetError());
            continue;
        }

        butoane_dificultate[i].textura = SDL_CreateTextureFromSurface(renderer, suprafata);
        SDL_FreeSurface(suprafata);

        int width, height;
        SDL_QueryTexture(butoane_dificultate[i].textura, NULL, NULL, &width, &height);

        butoane_dificultate[i].rect = (SDL_Rect){
            (SCREEN_WIDTH - 350) / 2,
            120 + i * 140, 
            350,
            140};
    }

    TTF_CloseFont(font);
}

void randare_meniu(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);
    SDL_RenderClear(renderer);

    if (fundal_meniu)
    {
        SDL_RenderCopy(renderer, fundal_meniu, NULL, NULL); 
    }

    for (int i = 0; i < 4; i++)
    {
        if (butoane[i].textura)
        {
            SDL_RenderCopy(renderer, butoane[i].textura, NULL, &butoane[i].rect);
        }
    }
}

void randare_meniu_dificultate(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
    SDL_RenderClear(renderer);

    if (fundal_meniu)
    {
        SDL_RenderCopy(renderer, fundal_meniu, NULL, NULL);
    }

   
    TTF_Font *font = TTF_OpenFont("assets/fonts/OpenSans.ttf", 42); 
    if (font)
    {
        SDL_Color culoare_text = {230, 214, 144, 255};
        SDL_Surface *suprafata = TTF_RenderUTF8_Blended(font, "Alege dificultatea AI", culoare_text);
        if (suprafata)
        {
            SDL_Texture *textura = SDL_CreateTextureFromSurface(renderer, suprafata);

            SDL_Rect rect = {
                (SCREEN_WIDTH - suprafata->w) / 2,
                30,
                suprafata->w,
                suprafata->h};

            SDL_RenderCopy(renderer, textura, NULL, &rect);
            SDL_DestroyTexture(textura);
            SDL_FreeSurface(suprafata);
        }
        TTF_CloseFont(font);
    }

    for (int i = 0; i < 3; i++)
    {
        if (butoane_dificultate[i].textura)
        {
            SDL_Rect buton = butoane_dificultate[i].rect;

            SDL_Rect umbra = {buton.x + 3, buton.y + 3, buton.w, buton.h};
            SDL_SetRenderDrawColor(renderer, 20, 20, 20, 150);
            SDL_RenderFillRect(renderer, &umbra);

            SDL_SetRenderDrawColor(renderer, 37, 34, 27, 255);

            if ((i == 0 && nivelDificultateAI == DIFICULTATE_USOARA) ||
                (i == 1 && nivelDificultateAI == DIFICULTATE_MEDIE) ||
                (i == 2 && nivelDificultateAI == DIFICULTATE_DIFICILA))
            {
                SDL_SetRenderDrawColor(renderer, 56, 44, 30, 255); 
            }

            SDL_RenderFillRect(renderer, &buton);

            //Border pentru butoane
            SDL_SetRenderDrawColor(renderer, 230, 214, 144, 255); // Culoarea textului pentru border
            SDL_RenderDrawRect(renderer, &buton);

            // Adaugă highlight la partea de sus a butonului pentru efect 3D
            SDL_Rect highlight = {buton.x + 1, buton.y + 1, buton.w - 2, 2};
            SDL_SetRenderDrawColor(renderer, 230, 214, 144, 100);
            SDL_RenderFillRect(renderer, &highlight);

            int textW, textH;
            SDL_QueryTexture(butoane_dificultate[i].textura, NULL, NULL, &textW, &textH);

            SDL_Rect textRect = {
                buton.x + (buton.w - textW) / 2,
                buton.y + (buton.h - textH) / 2,
                textW,
                textH};

            //Afisare butoane
            SDL_RenderCopy(renderer, butoane_dificultate[i].textura, NULL, &textRect);
        }
    }
    SDL_RenderCopy(renderer, buton_back.textura, NULL, &buton_back.rect);
}

ModJoc gestioneaza_meniu(SDL_Event *event)
{
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        for (int i = 0; i < 4; i++)
        {
            if (mouseX >= butoane[i].rect.x &&
                mouseX <= butoane[i].rect.x + butoane[i].rect.w &&
                mouseY >= butoane[i].rect.y &&
                mouseY <= butoane[i].rect.y + butoane[i].rect.h)
            {
                if (i + 1 == MOD_JUCATOR_VS_AI)
                {
                    // Redirectionează catre meniul de dificultate
                    return MENIU_DIFICULTATE_AI;
                }
                return (ModJoc)(i + 1); 
            }
        }
    }
    return ECRAN_MENU;
}

ModJoc gestioneaza_meniu_dificultate(SDL_Event *event)
{
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Verifica dacă s-a apăsat butonul înapoi
        if (mouseX >= buton_back.rect.x &&
            mouseX <= buton_back.rect.x + buton_back.rect.w &&
            mouseY >= buton_back.rect.y &&
            mouseY <= buton_back.rect.y + buton_back.rect.h)
        {
            return ECRAN_MENU;
        }

        // Verifica daca s-a apasat un buton de dificultate
        for (int i = 0; i < 3; i++)
        {
            if (mouseX >= butoane_dificultate[i].rect.x &&
                mouseX <= butoane_dificultate[i].rect.x + butoane_dificultate[i].rect.w &&
                mouseY >= butoane_dificultate[i].rect.y &&
                mouseY <= butoane_dificultate[i].rect.y + butoane_dificultate[i].rect.h)
            {
                // Seteaza dificultatea
                switch (i)
                {
                case 0:
                    nivelDificultateAI = DIFICULTATE_USOARA;
                    break;
                case 1:
                    nivelDificultateAI = DIFICULTATE_MEDIE;
                    break;
                case 2:
                    nivelDificultateAI = DIFICULTATE_DIFICILA;
                    break;
                }
                return MOD_JUCATOR_VS_AI;
            }
        }
    }
    return MENIU_DIFICULTATE_AI;
}

int gestioneaza_buton_back(SDL_Event *event)
{
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseX >= buton_back.rect.x &&
            mouseX <= buton_back.rect.x + buton_back.rect.w &&
            mouseY >= buton_back.rect.y &&
            mouseY <= buton_back.rect.y + buton_back.rect.h)
        {
            return 1;
        }
    }
    return 0;
}

void elibereaza_resurse_meniu()
{
    for (int i = 0; i < 4; i++)
    {
        if (butoane[i].textura)
        {
            SDL_DestroyTexture(butoane[i].textura);
            butoane[i].textura = NULL;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        if (butoane_dificultate[i].textura)
        {
            SDL_DestroyTexture(butoane_dificultate[i].textura);
            butoane_dificultate[i].textura = NULL;
        }
    }

    if (buton_back.textura)
    {
        SDL_DestroyTexture(buton_back.textura);
        buton_back.textura = NULL;
    }

    if (fundal_meniu)
    {
        SDL_DestroyTexture(fundal_meniu);
        fundal_meniu = NULL;
    }
}