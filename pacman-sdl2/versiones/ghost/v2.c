#include <stdio.h>
#include <stdlib.h>

int ghost_x; //coordenada en x
int ghost_y; //coordenada en y
int ghost_speed; //velocidad en que se mueve
int ghost_direction; //dirección en que se mueve
char map[15][15]; //el mapa en matriz con sus dimensiones (faltan)

//función para colocar al fantasma en su posición inicial con dirección y velocidad inicial
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

//función para checar que el fantasma no se salga del mapa y cambie de dirección al chocar con una pared
void updateGhost(int *x, int *y, int *direction, int speed, char map[15][15]) {
    int nextX = *x; //clon de la posición actual en x
    int nextY = *y; //clon de la posición actual en y
    int Col, Row; //i, j de la matriz donde esta el mapa (posiciones)

    //checa si hay pared adelante de donde esta el fantasma (predice/se adelanta)
    if (*direction == 0) {
        nextY = nextY - speed;
    } else if (*direction == 1) {
        nextX = nextX + speed;
    } else if (*direction == 2) {
        nextY = nextY + speed;
    } else if (*direction == 3) {
        nextX = nextX - speed;
    }

    //convierte los pixeles de la ventana en las coordenadas de la matriz
    Col = nextX / 15; //el número se define con la cantidad de filas y columnas que tiene la matriz del mapa
    Row = nextY / 15;

    //busca si en la fila y columna hay una pared (#) y si la hay aleatoreamente cambia de dirección para que sea más natural y no cambie de dirección siempre al mismo lado
    if (map[Row][Col] == '#') {
        *direction = rand() % 4;
    } else {
        *x = nextX; //avanza en X
        *y = nextY; //avanza en Y
    }
}