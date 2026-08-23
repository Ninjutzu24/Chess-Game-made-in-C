#include <stdio.h>
#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h> //Pentru mesaje pe ecran

#include "SDL2/SDL_image.h"
#include "board.h"
#include "game.h"
#include "piece_graphics.h"
#include "mouse_interaction.h"
#include "moves.h"
#include "graphics.h"
#include "ai.h"
#include "special.h"
#include "mod_special.h"
#include "promovare.h"

void afiseazaMesajSah(SDL_Renderer *renderer, const char *mesaj, bool regeleNegru)
{
    // Verificam daca mesajul este gol
    if (!mesaj || strlen(mesaj) == 0)
    {
        return;
    }

    TTF_Font *font = TTF_OpenFont("assets/fonts/OpenSans.ttf", 32);
    if (!font)
    {
        printf("Eroare font: %s\n", TTF_GetError());
        return;
    }

    SDL_Color culoare = regeleNegru ? (SDL_Color){255, 255, 255, 255} : (SDL_Color){0, 0, 0, 255};

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, mesaj, culoare);
    if (!textSurface)
    {
        printf("Eroare surface text: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    if (textSurface->w <= 0 || textSurface->h <= 0)
    {
        printf("Avertisment: Text cu dimensiune zero\n");
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!textTexture)
    {
        printf("Eroare creare textură: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect destinatie = {
        .x = (SCREEN_WIDTH - textSurface->w) / 2,
        .y = 230,
        .w = textSurface->w,
        .h = textSurface->h};

    SDL_RenderCopy(renderer, textTexture, NULL, &destinatie);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

void afiseazaInformatiiJoc(SDL_Renderer *renderer, ModJoc mod, PieceColor jucatorCurent)
{
    SDL_RenderCopy(renderer, buton_back.textura, NULL, &buton_back.rect);

    TTF_Font *font = TTF_OpenFont("assets/fonts/OpenSans.ttf", 16);
    if (!font)
    {
        printf("Eroare font: %s\n", TTF_GetError());
        return;
    }

    char informatii[100] = "";

    // Adaugare informatii despre mod curent
    if (mod == MOD_JUCATOR_VS_JUCATOR)
    {
        sprintf(informatii, "Mod: Jucator vs Jucator | La mutare: %s",
                jucatorCurent == WHITE ? "Alb" : "Negru");
    }
    else if (mod == MOD_JUCATOR_VS_AI)
    {
        // Shorter text to fit in window
        sprintf(informatii, "Mod: Jucator vs AI (Dif: %s) | La mutare: %s",
                nivelDificultateAI == DIFICULTATE_USOARA ? "Usor" : nivelDificultateAI == DIFICULTATE_MEDIE ? "Mediu"
                                                                                                            : "Dificil",
                jucatorCurent == WHITE ? "Jucator" : "AI");
    }
    else if (mod == MOD_TEROARE_NOCTURNA)
    {
        sprintf(informatii, "Mod: Teroare Nocturna | La mutare: %s",
                jucatorCurent == WHITE ? "Alb" : "Negru");
    }

    if (strlen(informatii) == 0)
    {
        TTF_CloseFont(font);
        return;
    }

    SDL_Color culoare = {255, 255, 255, 255}; //Alb
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, informatii, culoare);
    if (!textSurface)
    {
        printf("Eroare surface text: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    if (textSurface->w <= 0 || textSurface->h <= 0)
    {
        printf("Avertisment: Text cu dimensiune zero in informatii joc\n");
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!textTexture)
    {
        printf("Eroare creare textura: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        return;
    }

    // Deseneaza fundal pentru status bar
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (mod == MOD_TEROARE_NOCTURNA)
    {
        //Informatii cu aspect intunecat
        SDL_SetRenderDrawColor(renderer, 50, 0, 0, 230); // Roșu inchis semi-transparent
    }
    else
    {
        // Status bar normal pentru celelalte moduri
        SDL_SetRenderDrawColor(renderer, TROLLEY_GREY_R, TROLLEY_GREY_G, TROLLEY_GREY_B, 230);
    }

    SDL_Rect statusBar = {
        .x = 0,
        .y = SCREEN_HEIGHT - STATUS_BAR_HEIGHT,
        .w = SCREEN_WIDTH,
        .h = STATUS_BAR_HEIGHT};
    SDL_RenderFillRect(renderer, &statusBar);

    // Adaugă highlight pentru status bar
    if (mod == MOD_TEROARE_NOCTURNA)
    {
        SDL_SetRenderDrawColor(renderer, 255, 30, 30, 255); // Roșu aprins
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, LIGHT_CREAM_R, LIGHT_CREAM_G, LIGHT_CREAM_B, 255);
    }

    SDL_Rect statusHighlight = {
        .x = 0,
        .y = SCREEN_HEIGHT - STATUS_BAR_HEIGHT,
        .w = SCREEN_WIDTH,
        .h = 3};
    SDL_RenderFillRect(renderer, &statusHighlight);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    SDL_Rect destinatie = {
        .x = UI_PADDING + 110, // Adaugă spațiu pentru butonul Back
        .y = SCREEN_HEIGHT - STATUS_BAR_HEIGHT / 2 - textSurface->h / 2,
        .w = textSurface->w,
        .h = textSurface->h};

    SDL_RenderCopy(renderer, textTexture, NULL, &destinatie);

    // Desenam butonul de Back deasupra barii de informatie a jocului
    SDL_RenderCopy(renderer, buton_back.textura, NULL, &buton_back.rect);

    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

// Variabila pentru a schimba setarile jocului din modul teroarea nocturna
int sKeyPressed = 0;

void afiseazaNotificareSetari(SDL_Renderer *renderer)
{
    TTF_Font *font = TTF_OpenFont("assets/fonts/OpenSans.ttf", 14);
    if (!font)
    {
        printf("Eroare font: %s\n", TTF_GetError());
        return;
    }

    const char *text = "Apasa tasta 'S' pentru a accesa setarile modului special";

    // Check if text is empty
    if (!text || strlen(text) == 0)
    {
        TTF_CloseFont(font);
        return;
    }

    SDL_Color culoare = {255, 150, 150, 255}; // Light red

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text, culoare);
    if (!textSurface)
    {
        TTF_CloseFont(font);
        return;
    }

    if (textSurface->w <= 0 || textSurface->h <= 0)
    {
        printf("Avertisment: Text cu dimensiune zero in notificare setari\n");
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture)
    {
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 60, 10, 10, 200); // Roșu foarte închis

    SDL_Rect backgroundRect = {
        (SCREEN_WIDTH - textSurface->w) / 2 - 10,
        10,
        textSurface->w + 20,
        textSurface->h + 10};
    SDL_RenderFillRect(renderer, &backgroundRect);

    // Adaugă un border elegant în culoare roșiatică
    SDL_SetRenderDrawColor(renderer, 150, 30, 30, 200);
    SDL_RenderDrawRect(renderer, &backgroundRect);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Desenează textul
    SDL_Rect textRect = {
        (SCREEN_WIDTH - textSurface->w) / 2,
        15,
        textSurface->w,
        textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("Eroare la inițializare: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1)
    {
        printf("TTF_Init error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Șah",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        printf("Eroare la crearea ferestrei: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("Eroare la crearea renderer-ului: %s\n", SDL_GetError());
        return 1;
    }

    // Încarcare texturi piese
    SDL_Texture *textures[12];
    if (!loadPieceTextures(renderer, textures))
    {
        printf("Eroare la încărcarea texturilor\n");
        return 1;
    }

    // Eliminam incarcarea texturii perete care nu mai este folosita
    SDL_Texture *wallTexture = NULL; // Pastram variabila dar nu mai încărcam textura

    // Cream un array corect de texturi pentru ceata cu 3 elemente
    SDL_Texture *fogTextures[3] = {NULL, NULL, NULL};

    // Încercam să incarcam textura de ceata
    SDL_Texture *fogTexture = loadTexture(renderer, "assets/fog/fantoma.png");
    if (fogTexture)
    {
        fogTextures[0] = fogTexture;
        fogTextures[1] = fogTexture; // Reutilizam aceeasi textura
        fogTextures[2] = fogTexture; // Reutilizam aceeasi textura
    }
    else
    {
        printf("Warning: Could not load fog texture. Using fallback.\n");

        // Creăm o textură simplă pentru ceață
        SDL_Surface *fogSurface = SDL_CreateRGBSurface(0, 64, 64, 32, 0, 0, 0, 0);
        if (fogSurface)
        {
            SDL_FillRect(fogSurface, NULL, SDL_MapRGBA(fogSurface->format, 255, 255, 255, 100));
            fogTexture = SDL_CreateTextureFromSurface(renderer, fogSurface);
            SDL_FreeSurface(fogSurface);

            if (fogTexture)
            {
                fogTextures[0] = fogTexture;
                fogTextures[1] = fogTexture;
                fogTextures[2] = fogTexture;
            }
        }
    }

    Fog fogs[NUM_FOGS];
    initFog(renderer, fogs, fogTextures);

    initializeaza_meniu(renderer);
    initializeaza_fundal_meniu(renderer);
    initializeaza_buton_back(renderer);
    initializeaza_meniu_dificultate(renderer);

    ModJoc currentState = ECRAN_MENU;
    Board board;
    initializeazaTabla(board);

    int running = 1;
    int selectedX = -1, selectedY = -1;
    SDL_Event event;

    PieceColor jucatorCurent = WHITE;
    StareJoc stare = IN_DESFASURARE;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }

            // Activam meniul de setari pentru modul special
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s && currentState == MOD_TEROARE_NOCTURNA)
            {
                if (!sKeyPressed)
                {
                    toggleMeniuModSpecial();
                    sKeyPressed = 1;
                }
            }
            if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_s)
            {
                sKeyPressed = 0;
            }

            if (currentState == MOD_TEROARE_NOCTURNA && estePopupActiv())
            {
                gestioneazaEvenimenteMeniuModSpecial(&event, &board, renderer, textures);
                continue; 
            }

            if (currentState == ECRAN_MENU)
            {
                currentState = gestioneaza_meniu(&event);
                if (currentState != ECRAN_MENU && currentState != IESIRE && currentState != MENIU_DIFICULTATE_AI)
                {
                    initializeazaTabla(board);
                    if (currentState == MOD_TEROARE_NOCTURNA)
                    {
                        initializeazaSpecialTabla(&board, renderer, textures);

                        SetariModSpecial setari = getSetariModSpecial();
                        if (setari.efectCeata)
                        {
                            activeazaEfectCeata(renderer);
                        }

                        deschideMeniuModSpecial(renderer, &board, textures);
                    }
                    else
                    {
                        punePiesePeTabla(&board, renderer, textures);
                    }
                    jucatorCurent = WHITE;
                    stare = IN_DESFASURARE;
                }
                else if (currentState == IESIRE)
                {
                    running = 0;
                }
            }
            else if (currentState == MENIU_DIFICULTATE_AI)
            {
                currentState = gestioneaza_meniu_dificultate(&event);
                if (currentState == MOD_JUCATOR_VS_AI)
                {
                    initializeazaTabla(board);
                    punePiesePeTabla(&board, renderer, textures);
                    jucatorCurent = WHITE;
                    stare = IN_DESFASURARE;
                }
            }
            else
            {
                if (gestioneaza_buton_back(&event))
                {
                    currentState = ECRAN_MENU;
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN && stare != SAH_MAT)
                {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    int col = mouseX / TILE_SIZE;
                    int row = mouseY / TILE_SIZE;

                    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
                    {
                        if (selectedX == -1)
                        {
                            if (board[row][col].type != EMPTY && board[row][col].color == jucatorCurent)
                            {
                                selectedX = row;
                                selectedY = col;
                            }
                        }
                        else
                        {
                            Move move = {selectedX, selectedY, row, col,
                                         board[selectedX][selectedY], board[row][col]};

                            int mutareValida = 0;
                            if (currentState == MOD_TEROARE_NOCTURNA)
                            {
                                mutareValida = esteMutareValidaSpecial(&board, &move, 0);
                            }
                            else
                            {
                                mutareValida = esteMutareValida(board, move, 0);
                            }

                            if (mutareValida)
                            {
                                executaMutare(board, move, textures);
                                if (move.piesa.type == PAWN && (move.toX == 0 || move.toX == 7))
                                {
                                    TipPromovare tipPromovare = afiseazaMeniuPromovare(renderer, textures, move.piesa.color);

                                    if (tipPromovare != PROMOVARE_ANULATA)
                                    {
                                        promoveazaPion(board, move.toX, move.toY, move.piesa.color, tipPromovare, textures);
                                    }

                                    else
                                    {
                                        // Dacă s-a anulat promovarea, anulezi mutarea complet
                                        // (poți reface piesele implicate la pozițiile inițiale)
                                        board[move.fromX][move.fromY] = move.piesa;
                                        board[move.toX][move.toY] = move.piesaCapturata;
                                        continue; // ieși din această iterație a buclei pentru a nu schimba jucătorul
                                    }
                                }

                                PieceColor adversar = (jucatorCurent == WHITE) ? BLACK : WHITE;
                                if (esteSahMat(board, adversar))
                                {
                                    stare = SAH_MAT;
                                    printf("Șah mat! %s câștigă\n", (jucatorCurent == WHITE) ? "Alb" : "Negru");
                                }
                                else if (esteSah(board, adversar))
                                {
                                    stare = SAH;
                                    printf("Șah!\n");
                                }
                                else
                                {
                                    stare = IN_DESFASURARE;
                                }

                                jucatorCurent = adversar;
                            }

                            selectedX = selectedY = -1;
                        }
                    }
                }
            }

            if (currentState == MOD_JUCATOR_VS_AI && jucatorCurent == BLACK && stare != SAH_MAT)
            {
                // Afiseaza un mesaj că AI-ul gandeste
                SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
                SDL_RenderClear(renderer);
                deseneazaTabla(renderer, &board, textures);

                // Afisează mesajul "AI gândeste..."
                afiseazaMesajSah(renderer, "AI gandeste...", false);
                SDL_RenderPresent(renderer);

                // Găsește cea mai bună mutare cu adâncimea bazată pe nivelul de dificultate
                Move mutareAI = gasesteCeaMaiBunaMutare(board, BLACK, nivelDificultateAI);
                if (mutareAI.fromX != -1)
                {
                    executaMutare(board, mutareAI, textures);

                    if (esteSahMat(board, WHITE))
                    {
                        stare = SAH_MAT;
                        printf("Șah mat! Computerul câștigă\n");
                    }
                    else if (esteSah(board, WHITE))
                    {
                        stare = SAH;
                        printf("Șah!\n");
                    }
                    else
                    {
                        stare = IN_DESFASURARE;
                    }

                    jucatorCurent = WHITE;
                }
            }
        }

        // Randare
        if (currentState == MOD_TEROARE_NOCTURNA)
        {
            // Culoare de fundal întunecată pentru modul Teroarea Nocturna
            SDL_SetRenderDrawColor(renderer, 25, 10, 15, 255); // Nuanță închisă de rosu-negru
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);
        }
        SDL_RenderClear(renderer);

        if (currentState == ECRAN_MENU)
        {
            randare_meniu(renderer);
        }
        else if (currentState == MENIU_DIFICULTATE_AI)
        {
            randare_meniu_dificultate(renderer);
        }
        else
        {
            if (currentState == MOD_TEROARE_NOCTURNA)
            {
                deseneazaTablaSpeciala(renderer, &board, textures, wallTexture);
                if (ceataActiva)
                {
                    updateFog(fogs);
                    drawFog(renderer, fogs);
                }

                if (!estePopupActiv())
                {
                    afiseazaNotificareSetari(renderer);
                }
            }
            else
            {
                deseneazaTabla(renderer, &board, textures);
            }

            if (selectedX != -1)
            {
                evidentiazaMutariValide(renderer, board, selectedX, selectedY);
            }

            SDL_RenderCopy(renderer, buton_back.textura, NULL, &buton_back.rect);

            // Afisează informatii despre joc
            afiseazaInformatiiJoc(renderer, currentState, jucatorCurent);

            // Randam setarile pentru mod daca e activat
            if (currentState == MOD_TEROARE_NOCTURNA)
            {
                afiseazaMeniuModSpecial(renderer);
            }
        }

        if (stare == SAH)
        {
            const char *mesajSah = (jucatorCurent == WHITE) ? "ALB ESTE IN SAH!" : "NEGRU ESTE IN SAH!";

            afiseazaMesajSah(renderer, mesajSah, (jucatorCurent == WHITE)); // Daca albul a mutat, negrul e în sah
        }
        else if (stare == SAH_MAT)
        {
            const char *mesajMat = (jucatorCurent == WHITE) ? "SAH MAT! NEGRU CASTIGA!" : "SAH MAT! ALB CASTIGA!";
            afiseazaMesajSah(renderer, mesajMat, (jucatorCurent == BLACK)); // Daca albul a mutat, negrul a pierdut
        }

        SDL_RenderPresent(renderer);
    }

    // Curatare resurse
    elibereaza_resurse_meniu();
    for (int i = 0; i < 12; i++)
    {
        SDL_DestroyTexture(textures[i]);
    }
    // Eliminam eliberarea texturii de perete dacă NULL
    if (wallTexture)
    {
        SDL_DestroyTexture(wallTexture);
    }
    for (int i = 0; i < 3; i++)
    {
        if (fogTextures[i])
        {
            SDL_DestroyTexture(fogTextures[i]);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
