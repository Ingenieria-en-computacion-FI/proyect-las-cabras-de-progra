#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "Ghosts.h"

// El mapa se queda global, aquí se crea su memoria
char map[15][15]; 

void renderGhosts(SDL_Renderer *renderer, SDL_Texture *ghostTexture, int *ghost_x, int *ghost_y, int *ghost_direction, bool *ghost_active, int *ghost_frame, unsigned int *ghost_lastFrameTime) {
    unsigned int currentTime = SDL_GetTicks();
    int animationSpeed = 150; 
    
    if (currentTime > *ghost_lastFrameTime + animationSpeed) {
        (*ghost_frame)++; // actualiza la variable del main
        *ghost_lastFrameTime = currentTime;
    }

    for (int i = 0; i < MAXGHOSTS; i++) {
        if (ghost_active[i] == true) {
            SDL_Rect srcRect; 
            SDL_Rect destRect; 
            
            int offsetX = 456; 
            int offsetY = 64; 
            int spriteWidth = 16; 
            int spriteHeight = 16; 
            
            int row = i; 
            int baseCol = 0; 
            
            if (ghost_direction[i] == 0) baseCol = 4; 
            else if (ghost_direction[i] == 1) baseCol = 0; 
            else if (ghost_direction[i] == 2) baseCol = 6; 
            else if (ghost_direction[i] == 3) baseCol = 2; 
            
            int finalCol = baseCol + ((*ghost_frame) % 2);
            
            srcRect.x = offsetX + (finalCol * spriteWidth);
            srcRect.y = offsetY + (row * spriteHeight);
            srcRect.w = spriteWidth;
            srcRect.h = spriteHeight;
            
            destRect.x = ghost_x[i];
            destRect.y = ghost_y[i];
            destRect.w = 15; 
            destRect.h = 15; 
            
            SDL_RenderCopy(renderer, ghostTexture, &srcRect, &destRect);
        }
    }
}

void setupGhost(int i, int *ghost_x, int *ghost_y) {
    ghost_x[i] = 0;
    ghost_y[i] = 0;
}

void spawnGhost(int i, int *ghost_speed, int *ghost_direction, bool *ghost_active) { 
    ghost_speed[i] = 4; 
    ghost_direction[i] = 0; 
    ghost_active[i] = true; 
}

void releaseGhosts(int *active_ghosts, int *last_release, int *ghost_speed, int *ghost_direction, bool *ghost_active) {
    if (*active_ghosts >= MAXGHOSTS) {
        return;
    }

    int current_time = (int)SDL_GetTicks();

    if (*active_ghosts == 0 || (current_time - *last_release >= INTERVAL)) { 
        spawnGhost(*active_ghosts, ghost_speed, ghost_direction, ghost_active);
        
        (*active_ghosts)++; // incrementa el contador
        *last_release = current_time; 
    }
}

void updateGhost(int *x, int *y, int *direction, int speed, char map[15][15]) {
    int nextX = *x; 
    int nextY = *y; 
    int Col, Row; 

    if (*direction == 0) nextY = nextY - speed;
    else if (*direction == 1) nextX = nextX + speed;
    else if (*direction == 2) nextY = nextY + speed;
    else if (*direction == 3) nextX = nextX - speed;

    Col = nextX / 15; 
    Row = nextY / 15;

    if (Row >= 0 && Row < 15 && Col >= 0 && Col < 15) {
        if (map[Row][Col] == '#') { 
            *direction = rand() % 4; 
        } else {
            *x = nextX; 
            *y = nextY; 
        }
    }
}

void chasePlayer(int player_x, int player_y, int *ghost_x, int *ghost_y, int *ghost_direction, bool *ghost_active) {
    for (int i = 0; i < MAXGHOSTS; i++) {
        if (ghost_active[i] == true) {
            int distX = abs(player_x - ghost_x[i]);
            int distY = abs(player_y - ghost_y[i]);
            int distTotal = distX + distY;

            if (distTotal < (5 * 15)) {
                if (player_y < ghost_y[i]) ghost_direction[i] = 0; 
                else if (player_x > ghost_x[i]) ghost_direction[i] = 1; 
                else if (player_y > ghost_y[i]) ghost_direction[i] = 2; 
                else if (player_x < ghost_x[i]) ghost_direction[i] = 3; 
            }
        }
    }
}

bool playerDeath(int player_x, int player_y, int *ghost_x, int *ghost_y, bool *ghost_active) {
    for (int i = 0; i < MAXGHOSTS; i++) {
        if (ghost_active[i] == true) {
            int distX = abs(player_x - ghost_x[i]);
            int distY = abs(player_y - ghost_y[i]);

            if (distX <= 0 && distY <= 0) { 
                return true; 
            }
        }
    }
    return false; 
}

void resetGhosts(int *active_ghosts, int *last_release, bool *ghost_active, int *ghost_x, int *ghost_y) {
    *active_ghosts = 0; 
    *last_release = (int)SDL_GetTicks(); 
    
    for(int i = 0; i < MAXGHOSTS; i++) {
        ghost_active[i] = false; 
        setupGhost(i, ghost_x, ghost_y); 
    }
}

void manageGhosts(int player_x, int player_y, int *ghost_x, int *ghost_y, int *ghost_direction, int *ghost_speed, bool *ghost_active, int *active_ghosts, int *last_release, char map[15][15]) {
    
    releaseGhosts(active_ghosts, last_release, ghost_speed, ghost_direction, ghost_active);

    chasePlayer(player_x, player_y, ghost_x, ghost_y, ghost_direction, ghost_active);

    for(int i = 0; i < MAXGHOSTS; i++) {
        if(ghost_active[i] == true) {
            updateGhost(&ghost_x[i], &ghost_y[i], &ghost_direction[i], ghost_speed[i], map);
        }
    }
}