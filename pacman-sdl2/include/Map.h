#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define ROW 26
#define COL 28
#define TILE_SIZE 32

#define WIDTH (COL * TILE_SIZE)
#define HEIGHT (ROW * TILE_SIZE)

typedef struct {
    int x;
    int y;
    int dx;
    int dy;
} Entity;

extern char map[ROW][COL]; 
extern SDL_Texture* texturaLaberinto;
extern SDL_Rect botonCerrar;

void cargarMapa(const char* nombreArchivo);
bool cargarTexturas(SDL_Renderer* renderer);
void renderizarMapa(SDL_Renderer* renderer);

#endif