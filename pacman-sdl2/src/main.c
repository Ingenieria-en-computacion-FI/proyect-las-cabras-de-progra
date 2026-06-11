#include "game.h"

int main(int argc, char* argv[]) {
    Game game;
    Player jugador;
    Ghost fantasmas[4]; 
    int num_fantasmas = 4;

    // 1. Inicializar tus ventanas de SDL2
    if (!initSDL(&game)) {
        return 1;
    }

    // 2. CARGAR LOS SPRITES OFICIALES (Tu nueva función)
    if (!loadSprites(&game)) {
        cleanSDL(&game);
        return 1;
    }

    // 3. Cargar el mapa de texto
    if (!cargarMapa("mapas/nivel1.txt", game.mapa, FILAS)) {
        cleanSDL(&game);
        return 1;
    }

    // 4. Inicializar los personajes (Funciones de tus compañeros)
    // Tus compañeros necesitan definir los rectángulos exactos en initPlayer/initGhost
    initPlayer(&jugador);
    initGhost(&fantasmas[0], 10, 10);
    initGhost(&fantasmas[1], 11, 10);
    initGhost(&fantasmas[2], 12, 10);
    initGhost(&fantasmas[3], 13, 10);

    game.is_running = true;

    // --- GAME LOOP ---
    while (game.is_running) {
        handleEvents(&game, &jugador);

        // Lógica de movimiento de tus compañeros (No cambia)
        for (int i = 0; i < num_fantasmas; i++) {
            moveGhost(&fantasmas[i], jugador.x, jugador.y, game.mapa);
        }

        // Tu renderizado de SDL2 que dibuja TODO con los SPRITES
        renderEverything(&game, &jugador, fantasmas, num_fantasmas);

        SDL_Delay(16); 
    }

    cleanSDL(&game);
    return 0;
}
