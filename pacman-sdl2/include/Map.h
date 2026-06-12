#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <stdbool.h>

/* Dimensiones del mapa */

#define ROW 26
#define COL 28
#define TILE_SIZE 30

/* Dimensiones de la ventana */

#define WINDOW_WIDTH  (COL * TILE_SIZE)
#define WINDOW_HEIGHT (ROW * TILE_SIZE)

/* Entidad genérica */

typedef struct
{
    int x;
    int y;
    int dx;
    int dy;
} Entity;

/* Variables globales */

extern char map[ROW][COL];

extern SDL_Texture *texturaLaberinto;

extern SDL_Rect botonCerrar;

/* Funciones */

void cargarMapa(const char *nombreArchivo);

bool cargarTexturas(SDL_Renderer *renderer,const char *rutaImagen);

void renderizarMapa(SDL_Renderer *renderer);

#endif