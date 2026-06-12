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
    
bool cargarTexturas(SDL_Renderer* renderer, const char* rutaImagen) {
    SDL_Surface* superficieTemporal = IMG_Load(rutaImagen);
    if (superficieTemporal == NULL) {
        printf("Error al cargar %s: %s\n", rutaImagen, IMG_GetError());
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

void renderizarMapa(SDL_Renderer* renderer) {
    if (texturaLaberinto != NULL) {
        SDL_RenderCopy(renderer, texturaLaberinto, NULL, NULL);
    }

    for (int r = 0; r < ROW; r++) {
        for (int c = 0; c < COL; c++) {
            if (map[r][c] == ' ') {
                SDL_Rect espacioVacio = { c * TILE_SIZE, r * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &espacioVacio);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &botonCerrar);
    SDL_RenderDrawLine(renderer, botonCerrar.x + 10, botonCerrar.y + 5, botonCerrar.x + 26, botonCerrar.y + 19);
    SDL_RenderDrawLine(renderer, botonCerrar.x + 26, botonCerrar.y + 5, botonCerrar.x + 10, botonCerrar.y + 19);
}