#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/
//Fruit and Bomb definitions
struct FallingEntity {
    int row;
    int col;
    int isFalling;
    int type; //0 for bomb, 1-5 for fruits
}; 

struct Basket {
    int row;
    int col;
    u32 image;
};

struct GameState {
    int score;
    int basketRow;
    int basketCol;
    struct FallingEntity fruit;
    int timeRemaining;
    int gameOver;
    int endGameType;
};

void startGame(struct GameState *state);
void drawStartScreen(void);
void drawGameOverScreen(void);
void drawFinalScoreScreen(const struct GameState *state);
void updateGame(struct GameState *state, u32 currentButtons);
void drawGame(const struct GameState *state);
int checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

#endif
