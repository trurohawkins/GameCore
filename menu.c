#include "menu.h"

Graph *makeMenu(int columns, int rows, int spacing) {
	Graph *menu = calloc(columns * rows, sizeof(Graph));
	for (int x = 0; x < columns; x++) {
		for (int y = 0; y < rows; y++) {
			Graph *cur = &menu[(y * columns) + x];
			Button *butt = calloc(1, sizeof(Button));
			butt->textBox = createTextBox(8, 5, "poop");
			butt->pos[0] = x * spacing;
			butt->pos[1] = y * spacing;
			cur->data = butt;
			cur->neighbors = calloc(4, sizeof(Graph*));
			cur->maxNeighbors = 4;
			for (int i = 0; i < 4; i++) {
				int *d = getDir4(i);
				int pos[2] = {x + d[0], y + d[1]};
				if (pos[0] >= 0 && pos[1] >= 0 && pos[0] < columns && pos[1] < rows) {
					cur->neighbors[i] = &menu[(pos[1] * columns) + pos[0]];
				}
			}
		}
	}
	return menu;
}

void addMenu(Graph *g, int xp, int yp) {
	searchMenu(g, xp, yp, g->lastVisit + 1);
}

void deleteMenu(Graph *g) {
	freeMenu(g, g->lastVisit + 1);
	free(g);
}

void searchMenu(Graph *g, int xp, int yp, int visit) {
	if (!g || g->lastVisit == visit) {
		return;
	}
	g->lastVisit = visit;
	Button *butt = g->data;
	//addUI(butt->textBox, xp + butt->pos[0], yp + butt->pos[1]);
	RenderCommand reco = {
		.type = 1,
		.index = butt->textBox,
		.layer = 10,
	};
	Pos pos = {
		.x = xp + butt->pos[0],
		.y = yp + butt->pos[1],
	};
	memcpy(reco.data, &pos, sizeof(Pos));
	addRenderCommand(reco);
	for (int i = 0; i < g->maxNeighbors; i++) {
		searchMenu(g->neighbors[i], xp, yp, visit);
	}
}

void freeMenu(Graph *g, int visit) {
	if (!g || g->lastVisit == visit) {
		return;	
	}
	g->lastVisit = visit;
	for (int i = 0; i < g->maxNeighbors; i++) {
		freeMenu(g->neighbors[i], visit);
	}
	//freeGraphPoint(g);
	free(g->data);
	free(g->neighbors);
}
