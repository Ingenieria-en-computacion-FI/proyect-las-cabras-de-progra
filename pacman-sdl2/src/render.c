#include "game.h"
#include <stdio.h>

bool initSDL(Game *game) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL no pudo inicializarse: %s\n", SDL_GetError());
        return false;
    }

    // Inicializar SDL_image para cargar PNGs
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image no pudo inicializarse: %s\n", IMG_GetError());
        SDL_Quit();
        return false;
    }

    int screen_width = COLUMNAS * TILE_SIZE;
    int screen_height = FILAS * TILE_SIZE;

    game->window = SDL_CreateWindow("Pac-Man con Sprites Oficiales - FI UNAM", 
                                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                    screen_width, screen_height, SDL_WINDOW_SHOWN);
    if (!game->window) return false;

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (!game->renderer) return false;

    return true;
}

// Nueva función para cargar tu imagen de sprites en la textura
bool loadSprites(Game *game) {
    game->spritesheet = IMG_LoadTexture(game->renderer, "assets/pacman_sprites.png");
    if (!game->spritesheet) {
        printf("No se pudo cargar la imagen de sprites: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

void handleEvents(Game *game, Player *player) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game->is_running = false;
        }
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

// ESTA FUNCIÓN CAMBIA POR COMPLETO PARA USAR LOS SPRITES
void renderEverything(Game *game, Player *player, Ghost fantasmas[], int num_fantasmas) {
    // 1. Limpiar pantalla en negro
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);

    // 2. Dibujar Laberinto (Paredes y Puntos) - Sin cambios
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            SDL_Rect tile_dst = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            if (game->mapa[i][j] == '#') {
                SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255); // Azul
                SDL_RenderFillRect(game->renderer, &tile_dst);
            } else if (game->mapa[i][j] == '.') {
                SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255); // Blanco
                SDL_Rect pellet = { (j * TILE_SIZE) + 14, (i * TILE_SIZE) + 14, 4, 4 };
                SDL_RenderFillRect(game->renderer, &pellet);
            }
        }
    }

    // 3. Dibujar Jugador usando SPRITES (SDL_RenderCopy)
    SDL_Rect player_dst = { player->x * TILE_SIZE, player->y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    
    // Obtenemos el rectángulo de recorte correcto según la dirección
    SDL_Rect *player_src = &player->sprite_rects[player->direction];
    
    // Copiamos el sprite de la imagen a la pantalla
    SDL_RenderCopy(game->renderer, game->spritesheet, player_src, &player_dst);

    // 4. Dibujar Fantasmas usando SPRITES (SDL_RenderCopy)
    for (int f = 0; f < num_fantasmas; f++) {
        SDL_Rect ghost_dst = { fantasmas[f].x * TILE_SIZE, fantasmas[f].y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        SDL_Rect *ghost_src = &fantasmas[f].sprite_rects[fantasmas[f].direction];
        
        SDL_RenderCopy(game->renderer, game->spritesheet, ghost_src, &ghost_dst);
    }

    SDL_RenderPresent(game->renderer);
}

void cleanSDL(Game *game) {
    if (game->spritesheet) SDL_DestroyTexture(game->spritesheet); // Limpiar la textura
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    IMG_Quit(); // Cerrar SDL_image
    SDL_Quit();
}
