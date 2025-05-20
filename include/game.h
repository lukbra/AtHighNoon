#ifndef GAME_H
#define GAME_H

#include "raylib.h"

typedef enum {
    TITLE,
    MAIN_MENU,
    ROUND_SPLASH,
    DUEL,
    CHALLENGE,
    RESULTS,
    HELP
}gameScreens;

typedef struct Crds {
    int posX;
    int posY;
} Crds;

typedef enum {
    RoundOngoing,
    CowgirlDead,
    CowboyFallstart,
    CowboyDead,
    CowgirlFallstart,
    RoundDraw
} roundState;

typedef enum {
    GameOngoing,
    CowboyWon,
    CowgirlWon,
    GameDraw
} gameState;

typedef enum {
    mouseNormal,
    mouseHoover,
    mouseClicked
} mouseState;

roundState determineRoundState (
    KeyboardKey key,
    int shootTime,
    int frameCount,
    int maxFrames
);

int CenterY (
    int WinHeight,
    int StrLength
) ;

#endif // GAME_H