#ifndef JUG1_H
#define JUG1_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600

typedef enum {
    STATE_IDLE,
    STATE_WALK
} State;

typedef struct {
    int x;
    int y;
    int speed;
    int velX;
    int velY;
    int frame;
    int direction;
    State state;
    State previousState;
    SDL_RendererFlip flip;
    bool moving;
    unsigned int lastFrameTime;
} Player;

// Prototipo de tu función
void jug1(
    Player *player,
    SDL_Renderer *renderer,
    SDL_Texture *texture,
    const Uint8 *keyboard
);

#endif