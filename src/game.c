#include "../include/game.h"
#include "raylib.h"

roundState determineRoundState (
    KeyboardKey key,
    int shootTime,
    int frameCount,
    int maxFrames
) 
{
    roundState roundStatus = RoundOngoing;

    if ((key == KEY_Q) && (frameCount < shootTime)) {
        roundStatus = CowboyFallstart;
    } else if (key == KEY_P && frameCount < shootTime) {
        roundStatus = CowgirlFallstart;
    } else if ((key == KEY_Q && key== KEY_P) || (frameCount == maxFrames)) {
        roundStatus = RoundDraw;
    } else if ((key == KEY_Q)) {
        roundStatus = CowgirlDead;
    } else if ((key == KEY_P)) {
        roundStatus = CowboyDead;
    }

    return roundStatus;
}