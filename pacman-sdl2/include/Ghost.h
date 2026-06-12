#ifndef GHOSTS_H
#define GHOSTS_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXGHOSTS 4 //la cantidad de fantasmas
#define INTERVAL 7000 //cada cuanto salen los fantasmas (por definir)

//cambio de variables para poder tener 4 fantasmas
extern int ghost_x[MAXGHOSTS]; //coordenada en x de cada fantasma
extern int ghost_y[MAXGHOSTS]; //coordenada en y de cada fantasma
extern int ghost_speed[MAXGHOSTS]; //velocidad de cada fantasma
extern int ghost_direction[MAXGHOSTS]; //dirección de cada fantasma
extern char map[15][15]; //el mapa en matriz con sus dimensiones (faltan)

//para saber si ya salierón los fantasmas
extern bool ghost_active[MAXGHOSTS]; 

//variables para conteo de fantasmas y tiempo de salida
extern int active_ghosts; //fantasmas liberados
extern int last_release; //cuándo se liberó el último fantasma

//variables para la animación
extern int ghost_frame; //cuadro actual de la animación
extern unsigned int ghost_lastFrameTime; //tiempo en el que cambió de cuadro

//función para dibujar a los fantasmas
void renderGhosts(SDL_Renderer *renderer, SDL_Texture *ghostTexture);

//función para colocar a cada fantasma en su posición inicial
void setupGhost(int i);

//función para liberar a los fantasmas uno por uno con su velocidad y dirección inicial
void spawnGhost(int i);

//función para controlar salida de los fantasmas
void releaseGhosts(void);

//función para checar que el fantasma no se salga del mapa y cambie de dirección al chocar con una pared
void updateGhost(int *x, int *y, int *direction, int speed, char map[15][15]);

//función para que persigan al jugador
void chasePlayer(int player_x, int player_y);

//función para checar si un fantasma toca al jugador y muera
bool playerDeath(int player_x, int player_y);

//función para resetear a los fantasmas al su estado inicial
void resetGhosts(void);

//función principal de fantasmas para eliminar
void manageGhosts(int player_x, int player_y, char map[15][15]);

#endif