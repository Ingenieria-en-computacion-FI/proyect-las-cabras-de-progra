#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "Ghosts.h"

//cambio de variables para poder tener 4 fantasmas
int ghost_x[MAXGHOSTS]; //coordenada en x de cada fantasma
int ghost_y[MAXGHOSTS]; //coordenada en y de cada fantasma
int ghost_speed[MAXGHOSTS]; //velocidad de cada fantasma
int ghost_direction[MAXGHOSTS]; //dirección de cada fantasma
char map[15][15]; //el mapa en matriz con sus dimensiones (faltan)

//para saber si ya salierón los fantasmas
bool ghost_active[MAXGHOSTS] = {false, false, false, false}; 

//variables para conteo de fantasmas y tiempo de salida
int active_ghosts = 0; //fantasmas liberados
int last_release = 0; //cuándo se liberó el último fantasma

//variables para la animación
int ghost_frame = 0; //cuadro actual de la animación
unsigned int ghost_lastFrameTime = 0; //tiempo en el que cambió de cuadro

//función para dibujar a los fantasmas con sus imagenes
void renderGhosts(SDL_Renderer *renderer, SDL_Texture *ghostTexture) {
    
    //obtiene el tiempo actual para la animación
    unsigned int currentTime = SDL_GetTicks();
    int animationSpeed = 150; //velocidad a la que se mueven
    
    //revisa si ya es tiempo de cambiar de cuadro de animación
    if (currentTime > ghost_lastFrameTime + animationSpeed) {
        ghost_frame++;
        ghost_lastFrameTime = currentTime;
    }

    //revisa y dibuja a todos los fantasmas
    for (int i = 0; i < MAXGHOSTS; i++) {
        //solo dibuja a los fantasmas activos
        if (ghost_active[i] == true) {
            
            SDL_Rect srcRect; //cuadro a recortar de la imagen
            SDL_Rect destRect; //cuadro donde se va a dibujar
            
            int offsetX = 456; //coordenada en x de donde empiezan los fantasmas en el png
            int offsetY = 64; //coordenada en y de donde empiezan en el png
            int spriteWidth = 16; //ancho del fantasma
            int spriteHeight = 16; //alto del fantasma
            
            int row = i; //fila del fantasma (0 es rojo, 1 es rosa...)
            int baseCol = 0; //columna inicial
            
            //calcula la columna inicial dependiendo de la dirección
            if (ghost_direction[i] == 0) {
                baseCol = 4; //arriba
            } 
            else if (ghost_direction[i] == 1) {
                baseCol = 0; //derecha
            } 
            else if (ghost_direction[i] == 2) {
                baseCol = 6; //abajo
            } 
            else if (ghost_direction[i] == 3) {
                baseCol = 2; //izquierda
            }
            
            //suma el cuadro de animación para alternar el dibujo
            int finalCol = baseCol + (ghost_frame % 2);
            
            //llena los datos del cuadro a recortar
            srcRect.x = offsetX + (finalCol * spriteWidth);
            srcRect.y = offsetY + (row * spriteHeight);
            srcRect.w = spriteWidth;
            srcRect.h = spriteHeight;
            
            //llena los datos de la posición en la ventana
            destRect.x = ghost_x[i];
            destRect.y = ghost_y[i];
            destRect.w = 15; //tamaño en x en el mapa
            destRect.h = 15; //tamaño en y en el mapa
            
            //dibuja el fantasma en la pantalla
            SDL_RenderCopy(renderer, ghostTexture, &srcRect, &destRect);
        }
    }
}

//Función para colocar a cada fantasma en su posición inicial
void setupGhost(int i) {
    ghost_x[i] = 0;
    ghost_y[i] = 0;
}

//función para liberar a los fantasmas uno por uno con su velocidad y dirección inicial
void spawnGhost(int i) { //i para saber que fantasma es (max 4)
    ghost_speed[i] = 4; //por definir velocidad a la que se mueve
    ghost_direction[i] = 0; //siempre salen hacia arriba
    ghost_active[i] = true; //para indicar que yaa está activo el fantasma
}

//función para controlar salida de los fantasmas
void releaseGhosts() {
    //revisa si ya salieron los cuatro fantasmas
    if (active_ghosts >= MAXGHOSTS) {
        return;
    }

    //obtiene el tiempo actual (desde que se abre el juego)
    int current_time = (int)SDL_GetTicks();

    //va sacando uno por uno al los fantasmas
    if (active_ghosts == 0 || (current_time - last_release >= INTERVAL)) { //si es el primer fantasma o ya el tiempo que en el que toiene que salir el que sigue
        
        //liberamos al fantasma que toca
        spawnGhost(active_ghosts);
        
        active_ghosts++; //incrementamos el contador de fantasmas liberados
        last_release = current_time; //reinicia el cronómetro para el siguiente
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

   //primero revisa que no esten fuera del mapa
    if (Row >= 0 && Row < 15 && Col >= 0 && Col < 15) {
        if (map[Row][Col] == '#') { //busca si en la fila y columna hay una pared (#)
            *direction = rand() % 4; //si la hay, aleatoreamente cambia de dirección para que sea más natural y no cambie de dirección siempre al mismo lado
        } else {
            *x = nextX; 
            *y = nextY; 
        }
    }
}

//función para que persigan al jugador
void chasePlayer(int player_x, int player_y) { //entran coordenadas del jugador
    for (int i = 0; i < MAXGHOSTS; i++) {
        //revisa a los fantasmas que ya estan afuera
        if (ghost_active[i] == true) {
            
            //calcula la distancia entre el jugador y este fantasma (valor absoluto)
            int distX = abs(player_x - ghost_x[i]);
            int distY = abs(player_y - ghost_y[i]);
            int distTotal = distX + distY;

            //si la distancia es menor a 5 cuadros
            if (distTotal < (5 * 15)) {
                //compara posiciones para cambiar la dirección hacia el jugador
                if (player_y < ghost_y[i]) {
                    ghost_direction[i] = 0; //arriba
                } 
                else if (player_x > ghost_x[i]) {
                    ghost_direction[i] = 1; //derecha
                } 
                else if (player_y > ghost_y[i]) {
                    ghost_direction[i] = 2; //abajo
                } 
                else if (player_x < ghost_x[i]) {
                    ghost_direction[i] = 3; //izquierda
                }
            }
        }
    }
}

//función para checar si un fantasma toca al jugador y muera
bool playerDeath(int player_x, int player_y) {
    for (int i = 0; i < MAXGHOSTS; i++) {
        //revisa choque solo con fantasmas activos
        if (ghost_active[i] == true) {
            
            //revisa la distancia entre el jugador y el fantasma
            int distX = abs(player_x - ghost_x[i]);
            int distY = abs(player_y - ghost_y[i]);

            //revisa que este en el rango correcto para matar al jugador
            if (distX <= 0 && distY <= 0) { //falta definir rango
                return true; //sí lo toca muere
            }
        }
    }
    return false; //si no, sigue el juego
}

//función para resetear a los fantasmas al su estado inicial
void resetGhosts() {
    active_ghosts = 0; //resetea el contador
    last_release = (int)SDL_GetTicks(); //resetea el tiempo
    
    for(int i = 0; i < MAXGHOSTS; i++) {
        ghost_active[i] = false; //desactiva a los fantasmas
        setupGhost(i); //regresa a los fantasmas a su posición inicial
    }
}

//función principal de fantasmas para eliminar
void manageGhosts(int player_x, int player_y, char map[15][15]) {
    
    //salen los fantasmas
    releaseGhosts();

    // compara posiciones y cambia la dirección hacia el jugador si está cerca
    chasePlayer(player_x, player_y);

    //aplica el movimiento y revisa choques con paredes
    for(int i = 0; i < MAXGHOSTS; i++) {
        if(ghost_active[i] == true) {
            updateGhost(&ghost_x[i], &ghost_y[i], &ghost_direction[i], ghost_speed[i], map);
        }
    }
}