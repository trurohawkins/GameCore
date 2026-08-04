#pragma once
#include <stdlib.h>
#include "GameCore.h"
#include "TUI.h"

typedef struct {
	int textBox;
	int pos[2];
} Button;

Graph *makeMenu(int columns, int rows, int spacing);
void addMenu(Graph *g, int xp, int yp);
void deleteMenu(Graph *g);
void searchMenu(Graph *g, int xp, int yp,int visit);
void freeMenu(Graph *g, int visit);

