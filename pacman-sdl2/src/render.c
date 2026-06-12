#include "game.h"
#include <stdio.h>

bool initGame(Game *game) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;

    game->window = SDL_CreateWindow("Pac-Man - Modulo Render", 
                                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                    COLUMNAS * TILE_SIZE, FILAS * TILE_SIZE, SDL_WINDOW_SHOWN);
    if (!game->window) return false;

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (!game->renderer) return false;


    game->spritesheet = IMG_LoadTexture(game->renderer, "assets/pacman_sprites.png");
    if (!game->spritesheet) {
        printf("Error: No se encontro assets/pacman_sprites.png\n");
        return false;
    }

    game->active_ghosts = 0;
    game->last_release = 0;
    return true;
}


void handleEvents(Game *game, Player *player) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) game->is_running = false;
        if (event.type == SDL_KEYDOWN) {
            int dx = 0, dy = 0;
            switch (event.key.keysym.sym) {
                case SDLK_UP:    dy = -1; player->direction = 0; break;
                case SDLK_DOWN:  dy =  1; player->direction = 1; break;
                case SDLK_LEFT:  dx = -1; player->direction = 2; break;
                case SDLK_RIGHT: dx =  1; player->direction = 3; break;
                case SDLK_ESCAPE: game->is_running = false; break;
            }
            if (dx != 0 || dy != 0) {
                movePlayer(player, dx, dy, game->mapa);
            }
        }
    }
}

void renderEverything(Game *game, Player *player, Ghost fantasmas[]) {
    
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            
            if (game->mapa[i][j] == '#') {
                SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255); 
                SDL_RenderFillRect(game->renderer, &tile);
            } else if (game->mapa[i][j] == '.') {
                SDL_SetRenderDrawColor(game->renderer, 255, 184, 174, 255); 
                SDL_Rect pellet = { (j * TILE_SIZE) + 14, (i * TILE_SIZE) + 14, 4, 4 };
                SDL_RenderFillRect(game->renderer, &pellet);
            }
        }
    }


    SDL_Rect player_dst = { player->x * TILE_SIZE, player->y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_RenderCopy(game->renderer, game->spritesheet, &player->sprite_rects[player->direction], &player_dst);

    for (int i = 0; i < MAXGHOSTS; i++) {
        if (fantasmas[i].is_active) {
            SDL_Rect ghost_dst = { fantasmas[i].x * TILE_SIZE, fantasmas[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_RenderCopy(game->renderer, game->spritesheet, &fantasmas[i].sprite_rects[fantasmas[i].direction], &ghost_dst);
        }
    }

    SDL_RenderPresent(game->renderer);
}

void cleanSDL(Game *game) {
    if (game->spritesheet) SDL_DestroyTexture(game->spritesheet);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    IMG_Quit();
    SDL_Quit();
}
