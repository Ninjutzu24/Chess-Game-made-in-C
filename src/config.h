#ifndef CONFIG_H
#define CONFIG_H

#include <SDL2/SDL.h>

#define TILE_SIZE 64
#define SCREEN_WIDTH (8 * TILE_SIZE)
#define SCREEN_HEIGHT (8 * TILE_SIZE + 100)
#define NUM_BUTOANE 3

typedef enum
{
    MENU_PRINCIPAL,
    JOC_2VS2,
    JOC_VS_AI,
    JOC_SPECIAL,
    IN_DESFASURARE,
    SAH,
    SAH_MAT,
    PAT,
    IESIRE
} StareJoc;

typedef struct
{
    SDL_Texture *texture;
    SDL_Rect rect;
} ButonMeniu;

#endif