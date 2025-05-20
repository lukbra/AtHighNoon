#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct Player {
    int       livesLeft;
    bool      isLifeLost;   
    Texture2D state;
} Player;

#endif //PLAYER_H