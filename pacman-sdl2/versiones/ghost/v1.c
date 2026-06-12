#include <stdio.h>
#include <stdlib.h>

int ghost_x; //coordenada en x
int ghost_y; //coordenada en y
int ghost_speed; //velocidad en que se mueve
int ghost_direction; //dirección en que se mueve

//función para colocar al fantasma en su posición inicial con velocidad inicial y direccion inicial
void setupGhost(int *x, int *y, int *direction, int *speed) {
    *x = 0;
    *y = 0;
    *speed = 0;
    *direction = 0;
    //Por definir
}

//función para que se empiece a mover el fantasma
void moveGhost(int *x, int *y, int direction, int speed) {
    if (direction == 0) {
        *y = *y - speed; //arriba
    } 
    else if (direction == 1) {
        *x = *x + speed; //derecha
    } 
    else if (direction == 2) {
        *y = *y + speed; //abajo
    } 
    else if (direction == 3) {
        *x = *x - speed; //izquierda
    }
}