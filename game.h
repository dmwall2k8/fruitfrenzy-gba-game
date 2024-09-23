#ifndef GAME_H
#define GAME_H

#include "gba.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

 //!! INCLUDE IMAGE FILENAMES HERE !!//
#include "images/TitleScreen.h"
#include "images/FinalScoreScreen.h"
#include "images/GameOverScreen.h"
#include "images/apple.h"
#include "images/bananas.h"
#include "images/orange.h"
#include "images/grapes.h"
#include "images/pear.h"
#include "images/bomb.h"
#include "images/Lbasket.h"

//START GAME START
void startGame(struct GameState *state) {
    state->score = 0;
    state->basketRow = HEIGHT - 25;
    state->basketCol = WIDTH / 2;
    state->fruit.row = 0;
    state->fruit.col = randint(0, WIDTH - 10);
    state->fruit.isFalling = 1;
    state->fruit.type = randint(0, 5); //0 for bomb, 1-5 for fruits
    state->timeRemaining = 1800; //30 secs * 60 frames per second
    state->gameOver = 0;
    state->endGameType = 0;
}
//Checking Collisions
int checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

//UPDATING GAME STATE
void updateGame(struct GameState *state, u32 currentButtons) {
    //Moving Basket
    if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
        state->basketCol -= 3;
        if (state->basketCol < 0) {
            state->basketCol = 0;
        }
    }
    if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
        state->basketCol += 3;
        if (state->basketCol > WIDTH - 25) {
            state->basketCol = WIDTH - 25;
        }
    }
    
    //Falling fruits/bombs
    if (state-> fruit.isFalling) {
        state->fruit.row += 2;
        if (state->fruit.row > HEIGHT) {
            state->fruit.row = 2;
            state->fruit.col = randint(0, WIDTH - 10);
            state->fruit.type = randint(0, 5); //0 for bomb, 1-5 for fruits
            state->fruit.isFalling = 1;
        }
    }

    //Check Collisions
    if (checkCollision(state->basketCol, state->basketRow, 25, 25,
        state->fruit.col, state->fruit.row, 10, 10)) {
        if (state->fruit.type > 0) {
            state->score++;
        } else if (state->fruit.type == 0) {
            state->gameOver = 1;
            state->endGameType = 1;
            return;
        }
        state->fruit.isFalling = 0;
        state->fruit.row = 240;
        //reset fruit 
        state->fruit.row = 0;
        state->fruit.col = randint(0, WIDTH - 10);
        state->fruit.type = randint(0, 5); //0 for bomb, 1-5 for fruits
        state->fruit.isFalling = 1;
    }

    //Update Timer
    if (state->timeRemaining > 0) {
        state->timeRemaining--;
        if (state->timeRemaining <= 0) {
            state->gameOver = 1;
            state ->endGameType = 2; // Game ends when timer runs out
            return;
        }
    }
}

//Drawing Game
void drawGame(const struct GameState *state) {
    fillScreenDMA(BLACK);
    //Draw basket !! Sprite size should be 25 x 25 !!
    drawImageDMA(state->basketRow, state->basketCol, 25, 25, Lbasket); // !!insert image name here

    //Draw the fruit or bomb !! Sprite should be 10 x 10 !!
    if (state->fruit.type == 0) {
        drawImageDMA(state->fruit.row, state->fruit.col, 10, 10, bomb); // draw bomb !!insert image name here
    } else if (state->fruit.type == 1) {
        drawImageDMA(state->fruit.row, state->fruit.col, 10, 10, apple); //draw apple !!insert image name here
    } else if (state->fruit.type == 2) {
        drawImageDMA(state->fruit.row, state->fruit.col, 10, 10, orange); //draw orange !!insert image name here
    } else if (state->fruit.type == 3) {
        drawImageDMA(state->fruit.row, state->fruit.col, 10, 10, grapes); //draw grapes !!insert image name here
    } else if (state->fruit.type == 4) {
        drawImageDMA(state->fruit.row, state->fruit.col, 10, 10, pear); //draw pear !!insert image name here
    } else if (state->fruit.type == 5) {
        drawImageDMA(state->fruit.row, state->fruit.col, 10, 10, bananas); //draw bananas !!insert image name here
    }

    //Draw score
    char score[30];
    sprintf(score, "Score: %d", state->score);
    drawString(150, 180, score, WHITE);

    //Draw timer
    char timer[30];
    sprintf(timer, "Time: %d", state->timeRemaining / 60);
    drawString(150, 10, timer, WHITE);
}
//Start
void drawStartScreen(void) {
    drawFullScreenImageDMA(TitleScreen); // !!May have tearing here  & insert image name here later!!
    drawCenteredString(HEIGHT / 2 - 15, WIDTH / 2 - 45, 80, 20, "Press [Enter] to start", WHITE);
}

//Final Score
void drawFinalScoreScreen(const struct GameState *state) {
    drawFullScreenImageDMA(FinalScoreScreen); // !!May have tearing here  & insert image name here later!!
    char score[100];
    sprintf(score, "Final Score: %d", state->score);
    drawCenteredString(HEIGHT / 2 - 20 , WIDTH / 2 - 60, 100, 20, score, WHITE);
    drawCenteredString(HEIGHT / 2 + 5, WIDTH / 2 - 40, 80, 20, "Press [Backspace] to restart", WHITE);
}

//Game Over
void drawGameOverScreen(void) {
    drawFullScreenImageDMA(GameOverScreen); // !!May have tearing here  & insert image name here later!!
    drawCenteredString(HEIGHT / 2 + 55, WIDTH / 2 - 40, 80, 20, "Press [Backspace] to restart", WHITE);
}

#endif