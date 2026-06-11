#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define FILAS 22
#define COLUMNAS 28
#define TILE_SIZE 32 

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool is_running;
    char mapa[FILAS][COLUMNAS];
} Game;

#endif