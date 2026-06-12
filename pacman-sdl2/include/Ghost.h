#ifndef GHOSTS_H
#define GHOSTS_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

// Incluimos los headers necesarios para reconocer ROW, COL y la estructura Player
#include "Map.h"
#include "Player.h"

#define MAXGHOSTS 4 
#define INTERVAL 7000 

extern char map[ROW][COL]; 

void renderGhosts(SDL_Renderer *renderer, SDL_Texture *ghostTexture, int *ghost_x, int *ghost_y, int *ghost_direction, bool *ghost_active, int *ghost_frame, unsigned int *ghost_lastFrameTime);

void setupGhost(int i, int *ghost_x, int *ghost_y);

void spawnGhost(int i, int *ghost_speed, int *ghost_direction, bool *ghost_active);

void releaseGhosts(int *active_ghosts, int *last_release, int *ghost_speed, int *ghost_direction, bool *ghost_active);

void updateGhost(int *x, int *y, int *direction, int speed, char map[ROW][COL]);

// Actualizamos las firmas para recibir el puntero a Player
void chasePlayer(Player *player, int *ghost_x, int *ghost_y, int *ghost_direction, bool *ghost_active);

bool playerDeath(Player *player, int *ghost_x, int *ghost_y, bool *ghost_active);

void resetGhosts(int *active_ghosts, int *last_release, bool *ghost_active, int *ghost_x, int *ghost_y);

void manageGhosts(Player *player, int *ghost_x, int *ghost_y, int *ghost_direction, int *ghost_speed, bool *ghost_active, int *active_ghosts, int *last_release, char map[ROW][COL]);

#endif