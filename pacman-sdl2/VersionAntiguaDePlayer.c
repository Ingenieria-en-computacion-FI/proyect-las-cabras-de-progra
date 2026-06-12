
#include "Player.h"
int map[15][15];

SDL_Rect walkDown[] = {
    {456,48,14,10},
    {472,48,14,13},

};

SDL_Rect walkHorizontal[] = {
    {460,16,10,14},
    {473,16,13,14},
};

SDL_Rect walkUp[] = {
    {456,36,14,10},
    {472,33,14,13},
};

SDL_Rect idle[] = {
    {488,0,14,14},
    {472,0,13,14},
};

void Player_Init(Player *player, int posx, int posy){

    player->x = posx;
    player->y = posy;

    player->velX=0;
    player->velY=0;

    player->speed = 5;

    player->frame = 0;
    player->direction = 2;

    player->moving = false;

    player->state = STATE_IDLE;
    player->previousState = STATE_IDLE;

    player->flip = SDL_FLIP_NONE;

    player->lastFrameTime = 0;
}



void Player_Update( Player *player, const Uint8 *keyboard)
{
    Player_HandleInput(player, keyboard);

    Player_Move(player);

    Player_CheckCollisions(player);

    Player_UpdateState(player);

    Player_UpdateAnimation(player);
}

// ======================================================
// INPUT
// ======================================================
static void Player_HandleInput(Player *player,  const unsigned char *keyboard)
{
    player->velX = 0;
    player->velY = 0;

    player->moving = false;

    // DERECHA
    if(keyboard[SDL_SCANCODE_RIGHT]){

        player->velX = player->speed;

        player->flip = SDL_FLIP_HORIZONTAL;

        player->direction = 0;

        player->moving = true;
    }

    // IZQUIERDA
    if(keyboard[SDL_SCANCODE_LEFT]){

        player->velX = -player->speed;

        player->flip = SDL_FLIP_NONE;

        player->direction = 0;

        player->moving = true;
    }

    // ARRIBA
    if(keyboard[SDL_SCANCODE_UP]){

        player->velY = -player->speed;

        player->direction = 1;

        player->moving = true;
    }

    // ABAJO
    if(keyboard[SDL_SCANCODE_DOWN]){

        player->velY = player->speed;

        player->direction = 2;

        player->moving = true;
    }
}


// ======================================================
// MOVIMIENTO
// ======================================================

static void Player_Move(Player *player)
{
    player->x += player->velX;

    player->y += player->velY;
}


// ======================================================
// COLISIONES
// ======================================================

static void Player_CheckCollisions(Player *player)
{
    int width = 30;
    int height = 50;

    // IZQUIERDA
    if(player->x < 0){
        player->x = WIDTH-width;
    }

    // DERECHA
    if(player->x + width > WIDTH){
        player->x = 0;
    }

    // ARRIBA
    if(player->y < 0){
        player->y = 0;
    }

    // ABAJO
    if(player->y + height > HEIGHT){
        player->y = HEIGHT - height;
    }
    //PAREDES
     if (map[player->x][player->y] == '#') {
        player->y = player->y;
        player->x = player->x;
   } else {
   }


    
    //FANTASMAS
}



// ESTADOS

static void Player_UpdateState(Player *player)
{
    if(player->moving){
        player->state = STATE_WALK;
    }else{
        player->state = STATE_IDLE;
    }

    // SI CAMBIÓ EL ESTADO
    if(player->state != player->previousState){

        player->frame = 0;

        player->previousState = player->state;
    }
}


// ======================================================
// ANIMACIONES
// ======================================================

static void Player_UpdateAnimation(Player *player)
{
    int animationSpeed;

    switch(player->state){

        case STATE_IDLE:
            animationSpeed = 500;
            break;

        case STATE_WALK:
            animationSpeed = 150;
            break;

        default:
            animationSpeed = 150;
            break;
    }

    unsigned int currentTime = SDL_GetTicks();

    if(currentTime > player->lastFrameTime + animationSpeed){

        player->frame++;

        player->lastFrameTime = currentTime;
    }
}
void Player_Render(Player *player, SDL_Renderer *renderer, SDL_Texture *texture
){

    SDL_Rect src;

    switch(player->state){
        case STATE_IDLE:
            src = idle[player->frame % 2];
            break;

        case STATE_WALK:
            switch(player->direction){
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
                    break;
            }
        
    }

    SDL_Rect dest = {
        player->x,
        player->y,
        30,
        50
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