#ifndef GAME
#define GAME

#include <stdint.h>
#include <ctype.h>

extern void (*gameLoop)(float);
extern void (*resizeScreen)(int, int);
extern void (*pauseFunc)(bool);

bool initGame();
void *runGame(void *data);
void exitGame();
void closeGame();
void gameSimulation();
void simulateStep(float delta);
void receiveEvent();

void toggleGamePause();
void setGamePause(bool state);
bool getPaused();
#endif
