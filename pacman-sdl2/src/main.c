#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "Player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_ttf.h>

int vidas = 3;

//Ghost

#include "Ghost.h"
#include "Player.h"
#include "Map.h"

#define WIDTH 800
#define HEIGHT 600

char map[ROW][COL];


bool running = false;
typedef enum {
    MENU_JUGAR,
    MENU_PUNTAJES,
    MENU_SALIR
} OpcionMenu;

char map[ROW][COL]; 
SDL_Texture* texturaLaberinto = NULL;

SDL_Rect botonCerrar = { WINDOW_WIDTH  - 44, 8, 36, 24 };

OpcionMenu menu(SDL_Renderer* renderer)
{
    OpcionMenu seleccion = MENU_JUGAR;

    SDL_Event event;
    bool ejecutando = true;

    while (ejecutando)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return MENU_SALIR;
            }

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP:
                        if (seleccion > MENU_JUGAR)
                            seleccion--;
                        break;

                    case SDLK_DOWN:
                        if (seleccion < MENU_SALIR)
                            seleccion++;
                        break;

                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        return seleccion;
                }
            }
        }

        // Fondo
        SDL_SetRenderDrawColor(renderer, 0, 0, 15, 255);
        SDL_RenderClear(renderer);

        // Botones 
        SDL_Rect btnJugar   = {(WINDOW_WIDTH/2)-150, 200, 300, 60}; 
        SDL_Rect btnPuntaje = {(WINDOW_WIDTH/2)-150, 300, 300, 60};
        SDL_Rect btnSalir   = {(WINDOW_WIDTH/2)-150, 400, 300, 60};

        // Dibujar borde amarillo al seleccionado
        if (seleccion == MENU_JUGAR)
        {
            SDL_Rect borde = {
                btnJugar.x - 5,
                btnJugar.y - 5,
                btnJugar.w + 10,
                btnJugar.h + 10
            };

            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderDrawRect(renderer, &borde);
        }

        if (seleccion == MENU_PUNTAJES)
        {
            SDL_Rect borde = {
                btnPuntaje.x - 5,
                btnPuntaje.y - 5,
                btnPuntaje.w + 10,
                btnPuntaje.h + 10
            };

            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderDrawRect(renderer, &borde);
        }

        if (seleccion == MENU_SALIR)
        {
            SDL_Rect borde = {
                btnSalir.x - 5,
                btnSalir.y - 5,
                btnSalir.w + 10,
                btnSalir.h + 10
            };

            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderDrawRect(renderer, &borde);
        }

        // Botón Jugar
        SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255);
        SDL_RenderFillRect(renderer, &btnJugar);

        // Botón Puntajes
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        SDL_RenderFillRect(renderer, &btnPuntaje);

        // Botón Salir
        SDL_SetRenderDrawColor(renderer, 180, 0, 0, 255);
        SDL_RenderFillRect(renderer, &btnSalir);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return MENU_SALIR;

}




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



void jug1(
    Player *player,
    SDL_Renderer *renderer,
    SDL_Texture *texture,
    const Uint8 *keyboard
)
{
    static bool initialized = false;

    static SDL_Rect walkDown[] = {
        {456,48,14,10},
        {472,48,14,13}
    };

    static SDL_Rect walkHorizontal[] = {
        {460,16,10,14},
        {473,16,13,14}
    };

    static SDL_Rect walkUp[] = {
        {456,36,14,10},
        {472,33,14,13}
    };

    static SDL_Rect idle[] = {
        {488,0,14,14},
        {472,0,13,14}
    };

    /* Inicialización */

    if(!initialized)
    {
        player->x = WIDTH / 2;
        player->y = (HEIGHT / 2)+125;

        player->velX = 0;
        player->velY = 0;

        player->speed = 5;

        player->frame = 0;
        player->direction = 2;

        player->moving = false;

        player->state = STATE_IDLE;
        player->previousState = STATE_IDLE;

        player->flip = SDL_FLIP_NONE;

        player->lastFrameTime = 0;

        initialized = true;
    }

    /* INPUT */

    player->velX = 0;
    player->velY = 0;
    player->moving = false;

    if(keyboard[SDL_SCANCODE_RIGHT])
    {
        player->velX = player->speed;
        player->flip = SDL_FLIP_HORIZONTAL;
        player->direction = 0;
        player->moving = true;
    }

    if(keyboard[SDL_SCANCODE_LEFT])
    {
        player->velX = -player->speed;
        player->flip = SDL_FLIP_NONE;
        player->direction = 0;
        player->moving = true;
    }

    if(keyboard[SDL_SCANCODE_UP])
    {
        player->velY = -player->speed;
        player->direction = 1;
        player->moving = true;
    }

    if(keyboard[SDL_SCANCODE_DOWN])
    {
        player->velY = player->speed;
        player->direction = 2;
        player->moving = true;
    }

    /* MOVIMIENTO */

    player->x += player->velX;
    player->y += player->velY;

    /* COLISIONES */
    /* MOVIMIENTO CON COLISIÓN POR ESQUINAS */

    int nextX = player->x + player->velX;
    int nextY = player->y + player->velY;

    const int width = 15;
    const int height = 25;

    int left   = nextX / TILE_SIZE;
    int right  = (nextX + width - 1) / TILE_SIZE;
    int top    = nextY / TILE_SIZE;
    int bottom = (nextY + height - 1) / TILE_SIZE;

    bool collision = false;

    if(map[top][left] == '#' ||map[top][right] == '#' ||map[bottom][left] == '#' ||map[bottom][right] == '#')
    {
        collision = true;
        player->x = player->x -player->velX;
        player->y = player->y -player->velY;    
    }

    if(!collision)
    {
        player->x = nextX;
        player->y = nextY;
    }

    if(player->x < 0)
        player->x = WINDOW_WIDTH  - width;

    if(player->x + width > WINDOW_WIDTH )
        player->x = 0;

    if(player->y < 0)
        player->y = 0;

    if(player->y + height > WINDOW_HEIGHT)
        player->y = WINDOW_HEIGHT - height;
    



        
    /* ESTADO */

    player->state =
        player->moving ? STATE_WALK : STATE_IDLE;

    if(player->state != player->previousState)
    {
        player->frame = 0;
        player->previousState = player->state;
    }

    /* ANIMACIÓN */

    int animationSpeed =
        (player->state == STATE_IDLE)
        ? 500
        : 150;

    Uint32 currentTime = SDL_GetTicks();

    if(currentTime >
       player->lastFrameTime + animationSpeed)
    {
        player->frame++;
        player->lastFrameTime = currentTime;
    }

    /* SPRITE */

    SDL_Rect src;

    if(player->state == STATE_IDLE)
    {
        src = idle[player->frame % 2];
    }
    else
    {
        switch(player->direction)
        {
            case 0:
                src = walkHorizontal[player->frame % 2];
                break;

            case 1:
                src = walkUp[player->frame % 2];
                break;

            case 2:
                src = walkDown[player->frame % 2];
                break;

            default:
                src = idle[0];
        }
    }

    /* RENDER */

    SDL_Rect dest = {
        player->x,
        player->y,
        15,
        25
    };

    SDL_RenderCopyEx(
        renderer,
        texture,
        &src,
        &dest,
        0,
        NULL,
        player->flip
    );
}

void renderGhosts(
    SDL_Renderer *renderer,
    SDL_Texture *ghostTexture,
    int *ghost_x,
    int *ghost_y,
    int *ghost_direction,
    bool *ghost_active,
    int *ghost_frame,
    Uint32 *ghost_lastFrameTime)
{
    Uint32 currentTime = SDL_GetTicks();
    int animationSpeed = 150;

    if(currentTime > *ghost_lastFrameTime + animationSpeed)
    {
        (*ghost_frame)++;
        *ghost_lastFrameTime = currentTime;
    }

    for(int i = 0; i < MAXGHOSTS; i++)
    {
        if(!ghost_active[i])
            continue;

        SDL_Rect srcRect;
        SDL_Rect destRect;

        int offsetX = 456;
        int offsetY = 64;

        int spriteWidth = 16;
        int spriteHeight = 16;

        int row_sprite = i % 4;

        int baseCol;

        switch(ghost_direction[i])
        {
            case 0: baseCol = 4; break;
            case 1: baseCol = 0; break;
            case 2: baseCol = 6; break;
            case 3: baseCol = 2; break;
            default: baseCol = 0;
        }

        int finalCol = baseCol + ((*ghost_frame) % 2);

        srcRect.x = offsetX + finalCol * spriteWidth;
        srcRect.y = offsetY + row_sprite * spriteHeight;
        srcRect.w = spriteWidth;
        srcRect.h = spriteHeight;

        destRect.x = ghost_x[i];
        destRect.y = ghost_y[i];
        destRect.w = 16;
        destRect.h = 16;

        SDL_RenderCopy(
            renderer,
            ghostTexture,
            &srcRect,
            &destRect
        );
    }
}

void setupGhost(int i, int *ghost_x, int *ghost_y) {
    ghost_x[i] = 0;
    ghost_y[i] = 0;
}

void spawnGhost(
    int i,
    int *ghost_x,
    int *ghost_y,
    int *ghost_speed,
    int *ghost_direction,
    bool *ghost_active)
{
    ghost_x[i] = 14 * TILE_SIZE;
    ghost_y[i] = 11 * TILE_SIZE;

    ghost_speed[i] = 4;
    ghost_direction[i] = rand() % 4;
    ghost_active[i] = true;
}

void releaseGhosts(
    int *active_ghosts,
    int *last_release,
    int *ghost_x,
    int *ghost_y,
    int *ghost_speed,
    int *ghost_direction,
    bool *ghost_active)
{
    if(*active_ghosts >= MAXGHOSTS)
        return;

    int current_time = SDL_GetTicks();

    if(*active_ghosts == 0 ||
       current_time - *last_release >= INTERVAL)
    {
        spawnGhost(
            *active_ghosts,
            ghost_x,
            ghost_y,
            ghost_speed,
            ghost_direction,
            ghost_active
        );

        (*active_ghosts)++;
        *last_release = current_time;
    }
}

void updateGhost(int *x, int *y, int *direction, int speed, char map[ROW][COL]) {
    int nextX = *x;
    int nextY = *y;
    int i, j;

    if (*direction == 0) nextY = nextY - speed;
    else if (*direction == 1) nextX = nextX + speed;
    else if (*direction == 2) nextY = nextY + speed;
    else if (*direction == 3) nextX = nextX - speed;

    i = nextX / TILE_SIZE; 
    j = nextY / TILE_SIZE; 

    if (i >= 0 && i < COL && j >= 0 && j < ROW) {
        if (map[j][i] == '#') {
            *direction = rand() % 4;
        } else {
            *x = nextX;
            *y = nextY;
        }
    }
}

void chasePlayer(Player *player, int *ghost_x, int *ghost_y, int *ghost_direction, bool *ghost_active) {
    for (int i = 0; i < MAXGHOSTS; i++) { 
        if (ghost_active[i] == true) {
            int distX = abs(player->x - ghost_x[i]);
            int distY = abs(player->y - ghost_y[i]);
            int distTotal = distX + distY;

            // Manteniendo la proporción de distancia visual de 5 cuadros
            if (distTotal < (5 * TILE_SIZE)) {
                if (player->y < ghost_y[i]) ghost_direction[i] = 0;
                else if (player->x > ghost_x[i]) ghost_direction[i] = 1;
                else if (player->y > ghost_y[i]) ghost_direction[i] = 2;
                else if (player->x < ghost_x[i]) ghost_direction[i] = 3;
            }
        }
    }
}

bool playerDeath(
    Player *player,
    int *ghost_x,
    int *ghost_y,
    bool *ghost_active)
{
    for(int i = 0; i < MAXGHOSTS; i++)
    {
        if(!ghost_active[i])
            continue;

        int distX = abs(player->x - ghost_x[i]);
        int distY = abs(player->y - ghost_y[i]);

        if(distX < 16 && distY < 16)
            return true;
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

void manageGhosts(
    Player *player,
    int *ghost_x,
    int *ghost_y,
    int *ghost_direction,
    int *ghost_speed,
    bool *ghost_active,
    int *active_ghosts,
    int *last_release,
    char map[ROW][COL])
{
    releaseGhosts(
        active_ghosts,
        last_release,
        ghost_x,
        ghost_y,
        ghost_speed,
        ghost_direction,
        ghost_active
    );

    chasePlayer(
        player,
        ghost_x,
        ghost_y,
        ghost_direction,
        ghost_active
    );

    for(int i = 0; i < MAXGHOSTS; i++)
    {
        if(ghost_active[i])
        {
            updateGhost(
                &ghost_x[i],
                &ghost_y[i],
                &ghost_direction[i],
                ghost_speed[i],
                map
            );
        }
    }
}

int main(int argc, char *argv[]){

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return 1;

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        return 1;

    SDL_Window *window = SDL_CreateWindow(
        "Juego",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if(!cargarTexturas(renderer, "laberinto1.png"))
    {
        printf("No se pudo cargar el laberinto\n");
        return 1;
    }

    SDL_Texture *texture =
        IMG_LoadTexture(renderer, "pacm.png");
    if(texture == NULL)
    {
        SDL_Log("Error cargando imagen: %s",
                IMG_GetError());

        return 1;
    }

    SDL_Texture *ghostTexture =
    IMG_LoadTexture(renderer, "sprites.png");

    if(ghostTexture == NULL)
    {
        SDL_Log(
        "Error cargando imagen:  %s",
        IMG_GetError());
        return 1;
    }



    OpcionMenu opcion = menu(renderer);

    switch(opcion)
    {
    case MENU_JUGAR:

        printf("Iniciar juego\n");
        Player player;
        cargarMapa("nivel1.txt");

        bool running = true;

        SDL_Event event;
        int ghost_x[MAXGHOSTS];
        int ghost_y[MAXGHOSTS];

        int ghost_speed[MAXGHOSTS];
        int ghost_direction[MAXGHOSTS];

        bool ghost_active[MAXGHOSTS];

        int active_ghosts = 0;

        int last_release = SDL_GetTicks();

        int ghost_frame = 0;

        Uint32 ghost_lastFrameTime = 0;

        resetGhosts(
        &active_ghosts,
        &last_release,
        ghost_active,
        ghost_x,
        ghost_y
        );

        while(running)
        {
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                    running = false;
            }

            const Uint8 *keyboard =
                SDL_GetKeyboardState(NULL);

            SDL_SetRenderDrawColor(
                renderer,
                0,
                0,
                0,
                255
            );

            SDL_RenderClear(renderer); 

            renderizarMapa(renderer);
            
            jug1(
                &player,
                renderer,
                texture,
                keyboard
            );
            manageGhosts(
            &player,
            ghost_x,
            ghost_y,
            ghost_direction,
            ghost_speed,
            ghost_active,
            &active_ghosts,
            &last_release,
            map
            );
            renderGhosts(
            renderer,
            ghostTexture,
            ghost_x,
            ghost_y,
            ghost_direction,
            ghost_active,
            &ghost_frame,
            &ghost_lastFrameTime
            );
            if(playerDeath(
            &player,
            ghost_x,
            ghost_y,
            ghost_active))
            {
                printf("GAME OVER\n");


                resetGhosts(
                &active_ghosts,
                &last_release,
                ghost_active,
                ghost_x,
                ghost_y
                );
                vidas= vidas-1;
                if (vidas <= -1)
                break;
}
            SDL_RenderPresent(renderer);

            SDL_Delay(16);
        }
        break;

    case MENU_PUNTAJES:
        printf("Mostrar puntajes\n");
        break;

    case MENU_SALIR:
        printf("Salir\n");
        running = false;
        break;
}
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);

IMG_Quit();
if(texturaLaberinto != NULL)
{
    SDL_DestroyTexture(texturaLaberinto);
}
SDL_Quit();

return 0;
}



