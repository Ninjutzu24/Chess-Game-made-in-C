#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "board.h"

// Modern UI constants
#define UI_PADDING 20
#define STATUS_BAR_HEIGHT 80
#define SCREEN_WIDTH (BOARD_SIZE * TILE_SIZE + 2 * UI_PADDING)
#define SCREEN_HEIGHT (BOARD_SIZE * TILE_SIZE + STATUS_BAR_HEIGHT + 2 * UI_PADDING)

// Modern color scheme
#define COLOR_PRIMARY_R 41
#define COLOR_PRIMARY_G 128
#define COLOR_PRIMARY_B 185
#define COLOR_SECONDARY_R 52
#define COLOR_SECONDARY_G 73
#define COLOR_SECONDARY_B 94
#define COLOR_ACCENT_R 231
#define COLOR_ACCENT_G 76
#define COLOR_ACCENT_B 60
#define COLOR_LIGHT_R 236
#define COLOR_LIGHT_G 240
#define COLOR_LIGHT_B 241
#define COLOR_DARK_R 44
#define COLOR_DARK_G 62
#define COLOR_DARK_B 80

// Chess board colors
#define TROLLEY_GREY_R 136 
#define TROLLEY_GREY_G 129
#define TROLLEY_GREY_B 111
#define LIGHT_CREAM_R 234 
#define LIGHT_CREAM_G 230
#define LIGHT_CREAM_B 202

#define PEARL_GRAY_MOUSE_R TROLLEY_GREY_R
#define PEARL_GRAY_MOUSE_G TROLLEY_GREY_G
#define PEARL_GRAY_MOUSE_B TROLLEY_GREY_B

typedef enum
{
    ECRAN_MENU,
    MOD_JUCATOR_VS_JUCATOR,
    MOD_JUCATOR_VS_AI,
    MOD_TEROARE_NOCTURNA,
    IESIRE,
    MENIU_DIFICULTATE_AI
} ModJoc;

typedef enum
{
    DIFICULTATE_USOARA = 2,
    DIFICULTATE_MEDIE = 3,
    DIFICULTATE_DIFICILA = 4
} NivelDificultateAI;

typedef struct
{
    SDL_Texture *textura;
    SDL_Rect rect;
} Buton;

typedef enum
{
    IN_DESFASURARE,
    SAH,
    SAH_MAT
} StareJoc;

int loadPieceTextures(SDL_Renderer *renderer, SDL_Texture *textures[]);

extern Buton buton_back;
extern Buton butoane[4];
extern Buton butoane_dificultate[3];
extern SDL_Texture *fundal_meniu;
extern NivelDificultateAI nivelDificultateAI;

void initializeaza_meniu(SDL_Renderer *renderer);
void initializeaza_meniu_dificultate(SDL_Renderer *renderer);
void randare_meniu(SDL_Renderer *renderer);
void randare_meniu_dificultate(SDL_Renderer *renderer);
ModJoc gestioneaza_meniu(SDL_Event *event);
ModJoc gestioneaza_meniu_dificultate(SDL_Event *event);
void elibereaza_resurse_meniu();
void initializeaza_buton_back(SDL_Renderer *renderer);
int gestioneaza_buton_back(SDL_Event *event);
void initializeaza_fundal_meniu(SDL_Renderer *renderer);

#endif