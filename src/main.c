/*******************************************************************************************
*
*   raylib [core] example - Basic 3d example
*
*   Welcome to raylib!
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "stdio.h"
#include "stdbool.h"

#include "../include/player.h"
#include "../include/game.h"

#define   MAX_CHALL_CHARS   (50)
#define   MIN_DUEL_TIME_S   (2)
#define   MAX_DUEL_TIME_S   (5)
#define   WIN_WIDTH         (1024)
#define   WIN_HEIGHT        (512)
#define   TARGET_FPS        (120)

int main() {  
    // game state and logic
    Player cowboy, cowgirl;
    int    playerLives      = 3;
    bool   isRoundEnd       = false;
    int    duelState        = mouseNormal;
    int    challengeState   = mouseNormal;
    int    challConfState   = mouseNormal;
    bool   mouseAction      = false;
    bool   mouseOnText      = false;
    int    gameFrameCount   = 0;
    int    shootTime        = 0;
    int    roundStartFrame  = 0;
    int    roundsCount      = 0;
    float  ReactionFrames   = 0;

    // Font stuff
    Crds   duel, challenge, challConf, challText, help;
    Color  fontColor        = MAROON;
    Color  fontActive       = RAYWHITE;
    Color  fontInactive     = LIGHTGRAY;
    int smallFont           = 50;
    int regularFont         = smallFont * 2;
    int bigFont             = smallFont * 4;
    int challengeFont       = 30;
    duel.posX               = 110;
    duel.posY               = 75;
    challenge.posX          = 420;
    challenge.posY          = 75;
    challConf.posX          = 500;
    challConf.posY          = 300;
    challText.posX          = 10;
    challText.posY          = 200;
    help.posX               = duel.posX;
    help.posY               = 320;
    int letterCount         = 0;
    char challengeStr[MAX_CHALL_CHARS] = "\0";
    
    InitWindow (WIN_WIDTH, WIN_HEIGHT, "basic window");
    SetTargetFPS (TARGET_FPS);

    // graphics
    roundState roundStatus = RoundOngoing; 
    gameScreens currentScreen = TITLE;
    gameScreens gameMode;
    gameScreens nextScreen;
    Vector2 mousePoint        = { 0.0f, 0.0f };
    KeyboardKey keyPressed;
    Texture2D background       = LoadTexture("resources/background.png");
    Texture2D cowboy_idle      = LoadTexture("resources/cowboy_red_idle.png");
    Texture2D cowboy_shooting  = LoadTexture("resources/cowboy_red_shooting.png");
    Texture2D cowboy_dead      = LoadTexture("resources/cowboy_red_dead.png");
    Texture2D cowgirl_idle     = LoadTexture("resources/cowgirl_blue_idle.png");
    Texture2D cowgirl_shooting = LoadTexture("resources/cowgirl_blue_shooting.png");
    Texture2D cowgirl_dead     = LoadTexture("resources/cowgirl_blue_dead.png");
    Rectangle duelBounds       = {duel.posX, duel.posY, MeasureText("Duel", regularFont) , regularFont};
    Rectangle ChallBounds      = {challenge.posX, challenge.posY, MeasureText("Challenge", regularFont), regularFont};
    Rectangle textBounds       = {challText.posX, challText.posY, 1000, smallFont};
    Rectangle challConfBounds  = {challConf.posX, challConf.posY, MeasureText("Confirm", smallFont), smallFont};

    while (!WindowShouldClose ()) {
        if (gameFrameCount < (__INT_MAX__ - 1)) {
        gameFrameCount++;
        } else {
            gameFrameCount = 0;
        }

        switch (currentScreen)
        {
        case TITLE:
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            if (gameFrameCount > TARGET_FPS * 1) {
                currentScreen = MAIN_MENU;
            }
            break;

        case MAIN_MENU:
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            mousePoint     = GetMousePosition();
            mouseAction    = false;
            duelState      = mouseNormal;
            challengeState = mouseNormal;
            duelState      = mouseNormal;

            if (CheckCollisionPointRec(mousePoint, duelBounds))
            {
                SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    duelState = mouseClicked;
                } else {
                    duelState = mouseHoover;
                }

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    cowboy.livesLeft = playerLives;
                    cowgirl.livesLeft = playerLives;
                    mouseAction = true;
                    gameMode    = DUEL;
                    nextScreen  = ROUND_SPLASH;
                }
            }

            if (CheckCollisionPointRec(mousePoint, ChallBounds))
            {
                SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    challengeState = mouseClicked;
                } else {
                    challengeState = mouseHoover;
                }

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    cowboy.livesLeft = playerLives;
                    cowgirl.livesLeft = playerLives;
                    mouseAction = true;
                    gameMode    = CHALLENGE;
                    nextScreen  = CHALLENGE;
                }
            }

            if (mouseAction) {
                roundsCount = 1;
                currentScreen = nextScreen;
            }

            break;

        case ROUND_SPLASH:
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            WaitTime(3);

            roundStartFrame = gameFrameCount;
            isRoundEnd      = false;

            cowboy.isLifeLost  = false;
            cowboy.state       = cowboy_idle;
            cowgirl.isLifeLost = false;
            cowgirl.state      = cowgirl_idle;

            shootTime     = roundStartFrame +  GetRandomValue(MIN_DUEL_TIME_S * TARGET_FPS, MAX_DUEL_TIME_S* TARGET_FPS);
            ReactionFrames  = -1;
            currentScreen = DUEL;

            roundsCount++;

            break;
        
        case CHALLENGE:
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            if (CheckCollisionPointRec(GetMousePosition(), textBounds)) mouseOnText = true;
            else mouseOnText = false;
            mouseAction = false;

            if (mouseOnText)
            {
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                // Get char pressed (unicode character) on the queue
                int key = GetCharPressed();

                // Check if more characters have been pressed on the same frame
                while (key > 0)
                {
                    if ((key >= 32) && (key <= 125) && (letterCount < MAX_CHALL_CHARS))
                    {
                        challengeStr[letterCount] = (char)key;
                        challengeStr[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                        letterCount++;
                    }

                    key = GetCharPressed();  // Check next character in the queue
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    letterCount--;
                    if (letterCount < 0) {
                        letterCount = 0;
                    }
                    challengeStr[letterCount] = '\0';
                }
            }
            else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            
            // challenge confirmation
            mousePoint = GetMousePosition();
            if (CheckCollisionPointRec(mousePoint, challConfBounds))
            {
                SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
                
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    challConfState = mouseClicked;
                } else {
                    challConfState = mouseHoover;
                }

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    mouseAction = true;
                }
            }

            if (mouseAction) {
                currentScreen = ROUND_SPLASH;
            }
            
            break;
            
        case DUEL:
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            keyPressed = (GetKeyPressed());
            roundStatus = determineRoundState (keyPressed, shootTime, gameFrameCount, shootTime + TARGET_FPS * 3);
            if (isRoundEnd == false) {
                switch (roundStatus)
                {
                    case CowboyFallstart:
                        cowboy.state      = cowboy_shooting;
                        cowboy.livesLeft -= 1;
                        cowboy.isLifeLost = true;
                        cowgirl.state     = cowgirl_idle;
                        isRoundEnd        = true;
                        break;

                    case CowgirlFallstart:
                        cowboy.state       = cowboy_idle;
                        cowgirl.state      = cowgirl_shooting;
                        cowgirl.livesLeft -= 1;
                        cowgirl.isLifeLost = true;
                        isRoundEnd         = true;
                        break;

                    case CowboyDead:
                        cowboy.state      = cowboy_dead;
                        cowboy.livesLeft -= 1;
                        cowboy.isLifeLost = true;
                        cowgirl.state     = cowgirl_shooting;
                        isRoundEnd        = true;
                        break;

                    case CowgirlDead:
                        cowboy.state       = cowboy_shooting;
                        cowgirl.state      = cowgirl_dead;
                        cowgirl.livesLeft -= 1;
                        cowgirl.isLifeLost = true;
                        isRoundEnd         = true;
                        break;

                    case RoundDraw:
                        cowboy.state       = cowboy_idle;
                        cowboy.livesLeft  -= 1;
                        cowboy.isLifeLost  = true;
                        cowgirl.state      = cowgirl_idle;
                        cowgirl.livesLeft -= 1;
                        cowgirl.isLifeLost = true;
                        isRoundEnd         = true;
                        break;

                    default:
                        break;
                    } // case
            }

            if (isRoundEnd == false && gameFrameCount > shootTime) {
                ReactionFrames++;
            }

            if ((isRoundEnd == true) && (gameFrameCount > (shootTime + TARGET_FPS * 5))) {
                if (cowboy.livesLeft == 0 || cowgirl.livesLeft == 0) {
                    currentScreen = RESULTS;
                } else {
                    currentScreen = ROUND_SPLASH;
                }
            }  
            break;
      
        case RESULTS:
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            WaitTime (3);
            currentScreen = MAIN_MENU;
            break;

        default:
        break;
    }

    BeginDrawing ();
    switch (currentScreen)
    {
    case TITLE:
        DrawTexture (background, 0, 0, BLACK);
        DrawText ("At High Noon", 200, 100, 100, fontColor);
        break;

    case MAIN_MENU:
        DrawTexture (background, 0, 0, BLACK);
        
        DrawText ("Battle to\nDEATH!", duel.posX, duel.posY + 100, smallFont, fontColor);
        DrawText ("Set what, are you \nfighting for!", challenge.posX, challenge.posY + 100, smallFont, fontColor);
        DrawText ("Controls: \nPress [Q] to fire the cowboy\nPress [P] to fire the cowgirl", help.posX, help.posY, smallFont, fontColor);

        if (duelState == mouseHoover) {
            DrawText ("Duel", duel.posX, duel.posY, regularFont, fontActive);
        } else {
            DrawText ("Duel", duel.posX, duel.posY, regularFont, fontInactive);
        }
        
        if (challengeState == mouseHoover) {
            DrawText ("Challenge", challenge.posX, challenge.posY, regularFont, fontActive);
        } else {
            DrawText ("Challenge", challenge.posX, challenge.posY, regularFont, fontInactive);
        }
        break; 

    case ROUND_SPLASH:
        DrawTexture (background, 0, 0, BLACK);
        if (gameMode == DUEL) {
            DrawText ("Duel", 400, 100, regularFont, fontColor);
        } else {
            DrawText (challengeStr, 0.5 * WIN_WIDTH - 0.5 * MeasureText(challengeStr, smallFont), 70, smallFont, fontColor);
        }
        DrawText(TextFormat("Round %d", roundsCount), 400, 200, smallFont, fontColor);
        DrawText (TextFormat("Cowboy lives: %d", cowboy.livesLeft), 100, 300, smallFont, fontColor);
        DrawText (TextFormat("Cowgirl lives: %d", cowgirl.livesLeft), 600, 300, smallFont, fontColor);
        break; 

    case DUEL:        
        DrawTexture (background, 0, 0, WHITE);
        DrawText (TextFormat("Lives: %d", cowboy.livesLeft), 50, 10, smallFont, fontColor);
        DrawText (TextFormat("Lives: %d", cowgirl.livesLeft), 750, 10, smallFont, fontColor);
        DrawTextureEx (cowboy.state, (Vector2){50, 200}, 0.0, 0.5, WHITE);
        DrawTextureEx (cowgirl.state, (Vector2){750, 200}, 0.0, 0.55, WHITE);

        if ((gameFrameCount > shootTime) && (isRoundEnd != true)) {
            DrawText ("Fire!", 0.5 * WIN_WIDTH - 0.5 * MeasureText("Fire!", bigFont + 5), 50, bigFont + 5, BLACK);
            DrawText ("Fire!", 0.5 * WIN_WIDTH - 0.5 * MeasureText("Fire!", bigFont), 50, bigFont, fontColor);
        }

        if (isRoundEnd && ReactionFrames >= 0) {
            DrawText (TextFormat("Reaction time: %.3f s", ReactionFrames * (1.0 / TARGET_FPS)), 270, 300, smallFont, fontColor);
        }

        break;

    case CHALLENGE:
        DrawTexture (background, 0, 0, BLACK);

        DrawRectangleRec(textBounds, fontActive);
        if (mouseOnText) DrawRectangleLines((int)textBounds.x, (int)textBounds.y, (int)textBounds.width, (int)textBounds.height, fontActive);
        else DrawRectangleLines((int)textBounds.x, (int)textBounds.y, (int)textBounds.width, (int)textBounds.height, fontColor);

        DrawText(challengeStr, (int)textBounds.x + 5, (int)textBounds.y + 8, challengeFont, fontColor);

        DrawText(TextFormat("Your challenge: %i/%i", letterCount, MAX_CHALL_CHARS), 315, 250, 20, fontColor);

        if (mouseOnText)
        {
            if (letterCount < MAX_CHALL_CHARS)
            {
                // Draw blinking underscore char
                if (((gameFrameCount/20)%2) == 0) DrawText("_", MeasureText(challengeStr, challengeFont) + 20, (int)textBounds.y + 12, challengeFont, fontColor);
            }
            else DrawText("Character limit reached", 230, 300, 20, fontColor);
        }

        if (challConfState == mouseHoover) {
            DrawText ("Confirm", challConf.posX, challConf.posY, smallFont, fontActive);
        } else {
            DrawText ("Confirm", challConf.posX, challConf.posY, smallFont, fontInactive);
        }

        break;
    
    case HELP:
        break;

    case RESULTS:
        DrawTexture (background, 0, 0, BLACK);
        DrawText ("Duel", 100, 100, smallFont, fontColor);
        if (cowboy.livesLeft == 0) {
            DrawText (TextFormat("Cowgirl\nwon!"), 300, 100, bigFont, fontColor);
        } else {
            DrawText (TextFormat("Cowboy\nwon!"), 290, 100, bigFont, fontColor);
        }        
        
        break;

        default:
        break;
        }

    EndDrawing ();        
    }

    UnloadTexture (background);
    UnloadTexture (cowboy_idle);
    UnloadTexture (cowboy_shooting);
    UnloadTexture (cowboy_dead);
    UnloadTexture (cowgirl_idle);
    UnloadTexture (cowgirl_shooting);
    UnloadTexture (cowgirl_dead);

    CloseWindow ();
    
    return 0;
    
}