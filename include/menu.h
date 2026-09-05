#pragma once
#include <stdlib.h>
#include "GameCore.h"
#include "TUI.h"

typedef struct {
	int textBox;
	int pos[2];

	void (*func)(void);

	bool selected;
} Button;

typedef struct {
	Graph *butts;
	Graph *selected;
	float pos[2]; // 0 - 1
	int spacing[2];
} Menu;

Menu *makeMenu(int columns, int rows, int boxX, int boxY);
void setMenuSpacing(Menu *m, int spacingX, int spacingY);

void menuMoveCursor(Menu *m, int dir);
void pressButton(Menu *m);
void selectButton(Menu *m, int xp, int yp);
void deselectButton(Menu *m);
Button *getButton(Menu *m, int xp, int yp);
Graph *getGraph(Menu *m, int xp, int yp);
void nameButton(Button *butt, char *name);

void addMenu(Menu *m);
void deleteMenu(Menu *m);
void drawGraph(Graph *g, Menu *m, int xp, int yp, int visit);
void freeGraph(Graph *g, int visit);

