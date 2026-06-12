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