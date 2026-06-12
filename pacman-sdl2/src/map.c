#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>


// Altura y anchura del mapa 1
#define FILAS 26
#define COLUMNAS 28
#define TILE_SIZE 32


#define WINDOW_WIDTH (COLUMNAS * TILE_SIZE)
#define WINDOW_HEIGHT (FILAS * TILE_SIZE)  


typedef struct {
    int x;
    int y;
    int dx;
    int dy;
} Entity;


// Matriz bidimensional global del nivel
char mapa[FILAS][COLUMNAS];


// Puntero global para almacenar la textura del laberinto original
SDL_Texture* texturaLaberinto = NULL;


// Ubicación del botón de escape interactivo
SDL_Rect botonCerrar = { WINDOW_WIDTH - 44, 8, 36, 24 };


//  Lectura del archivo de mapa
void cargarMapa(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", nombreArchivo);
        return;
    }


    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            int c = fgetc(archivo);
           
            while (c == '\n' || c == '\r') {
                c = fgetc(archivo);
            }
            if (c == EOF) break;
           
            mapa[i][j] = (char)c;


            }
        }
    fclose(archivo);
    }
    




// Cargar texturas nativas BMP
bool cargarTexturas(SDL_Renderer* renderer) {
    SDL_Surface* superficieTemporal = SDL_LoadBMP("laberinto.bmp");
    if (superficieTemporal == NULL) {
        printf("Error al cargar laberinto.bmp: %s\n", SDL_GetError());
        return false;
    }
   
    texturaLaberinto = SDL_CreateTextureFromSurface(renderer, superficieTemporal);
    SDL_FreeSurface(superficieTemporal); // Liberar memoria temporal de la superficie
   
    if (texturaLaberinto == NULL) {
        printf("Error al crear la textura del mapa: %s\n", SDL_GetError());
        return false;
    }
    return true;
}


// Renderizado de Sprites
void renderizarMapa(SDL_Renderer* renderer) {
    if (texturaLaberinto != NULL) {
        SDL_RenderCopy(renderer, texturaLaberinto, NULL, NULL);
    }


    for (int r = 0; r < FILAS; r++) {
        for (int c = 0; c < COLUMNAS; c++) {
            // Si en tu matriz hay un camino libre o túnel, nos aseguramos de pintar el fondo negro
            if (mapa[r][c] == ' ') {
                SDL_Rect espacioVacio = { c * TILE_SIZE, r * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Negro absoluto
                // Para imperfecciones de alineación:
                // SDL_RenderFillRect(renderer, &espacioVacio);
            }
        }
    }


    // 3. Renderizar el botón interactivo de la interfaz de usuario
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &botonCerrar);
    SDL_RenderDrawLine(renderer, botonCerrar.x + 10, botonCerrar.y + 5, botonCerrar.x + 26, botonCerrar.y + 19);
    SDL_RenderDrawLine(renderer, botonCerrar.x + 26, botonCerrar.y + 5, botonCerrar.x + 10, botonCerrar.y + 19);
}


