#include "game.h"
#include "render.h"
#include "file_manager.h"
#include "player.h"
#include "ghost.h"

int main(int argc, char* argv[]) {
    Game game;
    Player jugador;
    Ghost fantasmas[4]; // Supongamos 4 fantasmas para empezar
    int num_fantasmas = 4;

    // 1. Inicializar SDL2 (Tu función)
    if (!initSDL(&game)) {
        return 1;
    }

    // 2. Cargar el mapa desde el archivo de texto
    if (!cargarMapa("assets/maps/nivel1.txt", game->mapa, FILAS)) {
        cleanSDL(&game);
        return 1;
    }

    // 3. Inicializar entidades (Funciones de tus compañeros)
    initPlayer(&jugador);
    initGhost(&fantasmas[0], 10, 10); // Posiciones de prueba en la matriz
    initGhost(&fantasmas[1], 11, 10);
    initGhost(&fantasmas[2], 12, 10);
    initGhost(&fantasmas[3], 13, 10);

    game.is_running = true;

    // --- BUCLE PRINCIPAL DEL JUEGO ---
    while (game.is_running) {
        // A. Capturar entradas de teclado (Tu función)
        handleEvents(&game, &jugador);

        // B. Actualizar IA de fantasmas (Código de tus compañeros)
        for (int i = 0; i < num_fantasmas; i++) {
            moveGhost(&fantasmas[i], jugador.x, jugador.y, game->mapa);
        }

        // C. Dibujar todo en pantalla (Tu función)
        renderEverything(&game, &jugador, fantasmas, num_fantasmas);

        // Controlar la velocidad del juego (aprox. 60 FPS)
        SDL_Delay(16);
    }

    // 4. Liberar memoria al cerrar (Tu función)
    cleanSDL(&game);

    return 0;
}