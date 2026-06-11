#include "game.h"
#include "render.h"
#include "file_manager.h"

int main(int argc, char* argv[]) {
    Game game;

    if (!initSDL(&game)) {
        return 1;
    }

    if (!cargarMapa("assets/maps/nivel1.txt", game.mapa, FILAS)) {
        cleanSDL(&game);
        return 1;
    }

    game.is_running = true;

    // Game Loop
    while (game.is_running) {
        handleEvents(&game);
        renderEverything(&game);
        SDL_Delay(16); // ~60 FPS
    }

    cleanSDL(&game);
    return 0;
}