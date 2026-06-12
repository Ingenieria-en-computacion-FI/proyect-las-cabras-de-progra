#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define FILAS 26
#define COLUMNAS 28
#define TILE_SIZE 32
#define WINDOW_WIDTH (COLUMNAS * TILE_SIZE)
#define WINDOW_HEIGHT (FILAS * TILE_SIZE)

typedef struct {
    int x;
    int y;
    int dx;
    int dy;
} Entity;

extern char mapa[FILAS][COLUMNAS];
extern SDL_Texture* texturaLaberinto;
extern SDL_Rect botonCerrar;

void cargarMapa(const char* nombreArchivo);
bool cargarTexturas(SDL_Renderer* renderer);
void renderizarMapa(SDL_Renderer* renderer);

#endif