#include "file_manager.h"
#include <stdio.h>

bool cargarMapa(const char *filename, char mapa[][28], int filas) {
    FILE *archivo = fopen(filename, "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el mapa %s\n", filename);
        return false;
    }

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < 28; j++) {
            int c = fgetc(archivo);
            if (c == '\n' || c == '\r') {
                j--; // Ignorar saltos de línea
                continue;
            }
            if (c == EOF) {
                mapa[i][j] = ' ';
            } else {
                mapa[i][j] = (char)c;
            }
        }
    }
    fclose(archivo);
    return true;
}