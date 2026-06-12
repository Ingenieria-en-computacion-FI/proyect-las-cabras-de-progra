#include <stdio.h>
#include <stdlib.h>
#include "Map.h"

char map[ROW][COL]; 
SDL_Texture* texturaLaberinto = NULL;
SDL_Rect botonCerrar = { WINDOW_WIDTH - 44, 8, 36, 24 };

void cargarMapa(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", nombreArchivo);
        return;
    }

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            int c = fgetc(archivo);
            while (c == '\n' || c == '\r') {
                c = fgetc(archivo);
            }
            if (c == EOF) break;
            map[i][j] = (char)c; 
        }
    }
    fclose(archivo);
}

bool cargarTexturas(SDL_Renderer* renderer) {
    SDL_Surface* superficieTemporal = SDL_LoadBMP("laberinto.bmp");
    if (superficieTemporal == NULL) {
        printf("Error al cargar laberinto.bmp: %s\n", SDL_GetError());
        return false;
    }

    texturaLaberinto = SDL_CreateTextureFromSurface(renderer, superficieTemporal);
    SDL_FreeSurface(superficieTemporal);

    if (texturaLaberinto == NULL) {
        printf("Error al crear la textura del mapa: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void renderizarMapa(SDL_Renderer* renderer) {
    if (texturaLaberinto != NULL) {
        SDL_RenderCopy(renderer, texturaLaberinto, NULL, NULL);
    }

    for (int r = 0; r < ROW; r++) {
        for (int c_index = 0; c_index < COL; c_index++) {
            if (map[r][c_index] == ' ') {
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &botonCerrar);
    SDL_RenderDrawLine(renderer, botonCerrar.x + 10, botonCerrar.y + 5, botonCerrar.x + 26, botonCerrar.y + 19);
    SDL_RenderDrawLine(renderer, botonCerrar.x + 26, botonCerrar.y + 5, botonCerrar.x + 10, botonCerrar.y + 19);
}