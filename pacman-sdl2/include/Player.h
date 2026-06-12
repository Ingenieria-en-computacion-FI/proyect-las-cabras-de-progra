#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600

typedef enum
{
    STATE_IDLE,
    STATE_WALK
} State;

typedef struct
{
    /* Posición */
    int x;
    int y;

    /* Velocidad */
    int speed;
    int velX;
    int velY;

    /* Animación */
    int frame;
    int direction;

    State state;
    State previousState;

    SDL_RendererFlip flip;

    bool moving;

    Uint32 lastFrameTime;

} Player;

/* Prototipo de la nueva función */

void jug1(
    Player *player,
    SDL_Renderer *renderer,
    SDL_Texture *texture,
    const Uint8 *keyboard
);

#endif