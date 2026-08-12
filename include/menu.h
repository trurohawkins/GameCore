#pragma once
#include <stdlib.h>
#include "GameCore.h"
#include "TUI.h"

typedef struct {
	int textBox;
	int pos[2];

	bool selected;
} Button;

typedef struct {
	Graph *butts;
	Graph *selected;
} Menu;

Menu *makeMenu(int columns, int rows, int spacingX, int spacingY);

void menuMoveCursor(Menu *m, int dir);

void addMenu(Menu *m, int xp, int yp);
void deleteMenu(Menu *m);
void drawGraph(Graph *g, int xp, int yp,int visit);
void freeGraph(Graph *g, int visit);

