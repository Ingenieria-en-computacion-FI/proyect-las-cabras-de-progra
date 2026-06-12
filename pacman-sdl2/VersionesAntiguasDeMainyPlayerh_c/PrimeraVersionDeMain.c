#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "Player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>





int map[15][15] = {0};



void jug1(
    Player *player,
    SDL_Renderer *renderer,
    SDL_Texture *texture,
    const Uint8 *keyboard
)
{
    static bool initialized = false;

    static SDL_Rect walkDown[] = {
        {456,48,14,10},
        {472,48,14,13}
    };

    static SDL_Rect walkHorizontal[] = {
        {460,16,10,14},
        {473,16,13,14}
    };

    static SDL_Rect walkUp[] = {
        {456,36,14,10},
        {472,33,14,13}
    };

    static SDL_Rect idle[] = {
        {488,0,14,14},
        {472,0,13,14}
    };

    /* Inicialización */

    if(!initialized)
    {
        player->x = WIDTH / 2;
        player->y = HEIGHT / 2;

        player->velX = 0;
        player->velY = 0;

        player->speed = 5;

        player->frame = 0;
        player->direction = 2;

        player->moving = false;

        player->state = STATE_IDLE;
        player->previousState = STATE_IDLE;

        player->flip = SDL_FLIP_NONE;

        player->lastFrameTime = 0;

        initialized = true;
    }

    /* INPUT */

    player->velX = 0;
    player->velY = 0;
    player->moving = false;

    if(keyboard[SDL_SCANCODE_RIGHT])
    {
        player->velX = player->speed;
        player->flip = SDL_FLIP_HORIZONTAL;
        player->direction = 0;
        player->moving = true;
    }

    if(keyboard[SDL_SCANCODE_LEFT])
    {
        player->velX = -player->speed;
        player->flip = SDL_FLIP_NONE;
        player->direction = 0;
        player->moving = true;
    }

    if(keyboard[SDL_SCANCODE_UP])
    {
        player->velY = -player->speed;
        player->direction = 1;
        player->moving = true;
    }

    if(keyboard[SDL_SCANCODE_DOWN])
    {
        player->velY = player->speed;
        player->direction = 2;
        player->moving = true;
    }

    /* MOVIMIENTO */

    player->x += player->velX;
    player->y += player->velY;

    /* COLISIONES */

    const int width = 30;
    const int height = 50;

    if(player->x < 0)
        player->x = WIDTH - width;

    if(player->x + width > WIDTH)
        player->x = 0;

    if(player->y < 0)
        player->y = 0;

    if(player->y + height > HEIGHT)
        player->y = HEIGHT - height;

    /* ESTADO */

    player->state =
        player->moving ? STATE_WALK : STATE_IDLE;

    if(player->state != player->previousState)
    {
        player->frame = 0;
        player->previousState = player->state;
    }

    /* ANIMACIÓN */

    int animationSpeed =
        (player->state == STATE_IDLE)
        ? 500
        : 150;

    Uint32 currentTime = SDL_GetTicks();

    if(currentTime >
       player->lastFrameTime + animationSpeed)
    {
        player->frame++;
        player->lastFrameTime = currentTime;
    }

    /* SPRITE */

    SDL_Rect src;

    if(player->state == STATE_IDLE)
    {
        src = idle[player->frame % 2];
    }
    else
    {
        switch(player->direction)
        {
            case 0:
                src = walkHorizontal[player->frame % 2];
                break;

            case 1:
                src = walkUp[player->frame % 2];
                break;

            case 2:
                src = walkDown[player->frame % 2];
                break;

            default:
                src = idle[0];
        }
    }

    /* RENDER */

    SDL_Rect dest = {
        player->x,
        player->y,
        30,
        50
    };

    SDL_RenderCopyEx(
        renderer,
        texture,
        &src,
        &dest,
        0,
        NULL,
        player->flip
    );
}










int main(int argc, char *argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return 1;

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        return 1;

    SDL_Window *window = SDL_CreateWindow(
        "Juego",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    SDL_Texture *texture =
        IMG_LoadTexture(renderer, "pacm.png");

    if(texture == NULL)
    {
        SDL_Log("Error cargando imagen: %s",
                IMG_GetError());

        return 1;
    }

    Player player;

    bool running = true;

    SDL_Event event;

    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                running = false;
        }

        const Uint8 *keyboard =
            SDL_GetKeyboardState(NULL);

        SDL_SetRenderDrawColor(
            renderer,
            0,
            0,
            0,
            255
        );

        SDL_RenderClear(renderer); 
        jug1(
            &player,
            renderer,
            texture,
            keyboard
        );

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}