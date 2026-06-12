#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#include "Map.h"
#include "Player.h"
#include "Ghosts.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error al inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Error al inicializar SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Pacman en C", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("Error al crear la ventana: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Error al crear el renderizador: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    cargarMapa("mapa.txt");
    
    SDL_Surface* tempSurf = IMG_Load("sprites.png");
    if (!tempSurf) {
        printf("Error cargando sprites.png: %s\n", IMG_GetError());
    }
    SDL_Texture* spriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurf);
    SDL_FreeSurface(tempSurf);

    bool running = true;
    SDL_Event event;

    Player player;
    bool isDead = false;
    int vidas = 3; // <-- Inicializamos el contador de vidas

    int ghost_x[MAXGHOSTS];
    int ghost_y[MAXGHOSTS];
    int ghost_direction[MAXGHOSTS];
    int ghost_speed[MAXGHOSTS];
    bool ghost_active[MAXGHOSTS];
    int active_ghosts = 0;
    int last_release = SDL_GetTicks();
    int ghost_frame = 0;
    unsigned int ghost_lastFrameTime = SDL_GetTicks();

    resetGhosts(&active_ghosts, &last_release, ghost_active, ghost_x, ghost_y);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;
                if (mx >= botonCerrar.x && mx <= botonCerrar.x + botonCerrar.w &&
                    my >= botonCerrar.y && my <= botonCerrar.y + botonCerrar.h) {
                    running = false;
                }
            }
        }

        const Uint8* keyboard = SDL_GetKeyboardState(NULL);

        if (!isDead) {
            manageGhosts(&player, ghost_x, ghost_y, ghost_direction, ghost_speed, ghost_active, &active_ghosts, &last_release, map);

            if (playerDeath(&player, ghost_x, ghost_y, ghost_active)) {
                vidas--; // Restamos una vida
                printf("¡Te atraparon! Vidas restantes: %d\n", vidas);

                if (vidas > 0) {
                    // Reiniciamos la posición del jugador manualmente
                    player.x = WIDTH / 2;
                    player.y = HEIGHT / 2;
                    
                    // Reiniciamos a los fantasmas
                    resetGhosts(&active_ghosts, &last_release, ghost_active, ghost_x, ghost_y);
                    
                    // Damos una pausa de 1 segundo (1000 ms) para que el jugador se prepare antes de reanudar
                    SDL_Delay(1000); 
                    
                    // Actualizamos el tiempo de liberación para que no salgan todos de golpe
                    last_release = SDL_GetTicks();
                } else {
                    // Si las vidas llegan a 0, termina el juego
                    isDead = true;
                    printf("¡Game Over! Te quedaste sin vidas.\n");
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderizarMapa(renderer);

        if (!isDead) {
            jug1(&player, renderer, spriteSheet, keyboard);
        }

        renderGhosts(renderer, spriteSheet, ghost_x, ghost_y, ghost_direction, ghost_active, &ghost_frame, &ghost_lastFrameTime);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyTexture(spriteSheet);
    if (texturaLaberinto) {
        SDL_DestroyTexture(texturaLaberinto);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}