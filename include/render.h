#ifndef RENDER_H
#define RENDER_H

#include "game.h"

// Firmas de las funciones de tu parte (SDL2)
bool initSDL(Game *game);
void handleEvents(Game *game);
void renderEverything(Game *game);
void cleanSDL(Game *game);

#endif