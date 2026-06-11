#include "render.h"
#include "file_manager.h"
#include <stdio.h>

bool initSDL(Game *game) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL no pudo inicializarse: %s\n", SDL_GetError());
        return false;
    }

    int screen_width = COLUMNAS * TILE_SIZE;
    int screen_height = FILAS * TILE_SIZE;

    game->window = SDL_CreateWindow("Pac-Man - Modulo SDL2", 
                                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                    screen_width, screen_height, SDL_WINDOW_SHOWN);
    if (!game->window) {
        return false;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (!game->renderer) {
        return false;
    }

    return true;
}

void handleEvents(Game *game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game->is_running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                game->is_running = false;
            }
        }
    }
}

void renderEverything(Game *game) {
    // Fondo negro
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);

    // Dibujar matriz
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            
            if (game->mapa[i][j] == '#') {
                SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255); // Azul para paredes
                SDL_RenderFillRect(game->renderer, &tile);
            } else if (game->mapa[i][j] == '.') {
                SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255); // Blanco para puntos
                SDL_Rect pellet = { (j * TILE_SIZE) + 14, (i * TILE_SIZE) + 14, 4, 4 };
                SDL_RenderFillRect(game->renderer, &pellet);
            }
        }
    }
    SDL_RenderPresent(game->renderer);
}

void cleanSDL(Game *game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}