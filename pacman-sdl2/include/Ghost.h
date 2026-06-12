#ifndef GHOST_H
#define GHOST_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Map.h"
#include "Player.h"

#define MAXGHOSTS 4
#define INTERVAL 7000

/* Variables globales definidas en otro archivo */
extern char map[ROW][COL];

/* Renderizado */
void renderGhosts(
    SDL_Renderer *renderer,
    SDL_Texture *ghostTexture,
    int ghost_x[],
    int ghost_y[],
    int ghost_direction[],
    bool ghost_active[],
    int *ghost_frame,
    Uint32 *ghost_lastFrameTime
);

/* Inicialización */
void setupGhost(
    int i,
    int ghost_x[],
    int ghost_y[]
);

void spawnGhost(
    int i,
    int ghost_x[],
    int ghost_y[],
    int ghost_speed[],
    int ghost_direction[],
    bool ghost_active[]
);

void releaseGhosts(
    int *active_ghosts,
    int *last_release,
    int ghost_x[],
    int ghost_y[],
    int ghost_speed[],
    int ghost_direction[],
    bool ghost_active[]
);

/* Movimiento */
void updateGhost(
    int *x,
    int *y,
    int *direction,
    int speed,
    char map[ROW][COL]
);

void chasePlayer(
    Player *player,
    int ghost_x[],
    int ghost_y[],
    int ghost_direction[],
    bool ghost_active[]
);

/* Colisiones */
bool playerDeath(
    Player *player,
    int ghost_x[],
    int ghost_y[],
    bool ghost_active[]
);

/* Reinicio */
void resetGhosts(
    int *active_ghosts,
    int *last_release,
    bool ghost_active[],
    int ghost_x[],
    int ghost_y[]
);

/* Administración general */
void manageGhosts(
    Player *player,
    int ghost_x[],
    int ghost_y[],
    int ghost_direction[],
    int ghost_speed[],
    bool ghost_active[],
    int *active_ghosts,
    int *last_release,
    char map[ROW][COL]
);

#endif