#include "menu.h"

Menu *makeMenu(int columns, int rows, int spacingX, int spacingY) {
	Graph *butts = calloc(columns * rows, sizeof(Graph));
	for (int x = 0; x < columns; x++) {
		for (int y = 0; y < rows; y++) {
			Graph *cur = &butts[(y * columns) + x];
			Button *butt = calloc(1, sizeof(Button));
			butt->textBox = createTextBox(12, 5, "poop");
			butt->pos[0] = x * spacingX;
			butt->pos[1] = y * spacingY;
			cur->data = butt;
			cur->neighbors = calloc(4, sizeof(Graph*));
			cur->maxNeighbors = 4;
			printf("graph %i, %i\n", x, y);
			for (int i = 0; i < 4; i++) {
				int *d = getDir4(i);
				int xp = ((x + d[0]) % columns + columns) % columns;
				int yp = ((y + d[1] % rows + rows) % rows);
				//int pos[2] = {(x + d[0]) % columns, (y + d[1]) % rows};
				if (xp >= 0 && yp >= 0 && xp < columns && yp < rows) {
					printf("	neighbor: %i - %i, %i\n", i, xp, yp);
					cur->neighbors[i] = &butts[(yp * columns) + xp];
				}
			}
		}
	}
	Menu *menu = calloc(1, sizeof(Menu));
	menu->butts = butts;
	menu->selected = butts;
	Button *b = menu->selected->data;
	b->selected = true;
	return menu;
}

void menuMoveCursor(Menu *m, int dir) {
	if (m) {
		Graph *neighbor = m->selected->neighbors[dir];
		if (neighbor) {
			Button *butt = m->selected->data;
			butt->selected = false;
			m->selected = neighbor;
			butt = neighbor->data;
			butt->selected = true;
		}
	}
}

void addMenu(Menu *m, int xp, int yp) {
	drawGraph(m->butts, xp, yp, m->butts->lastVisit + 1);
}

void deleteMenu(Menu *m) {
	freeGraph(m->butts, m->butts->lastVisit + 1);
	free(m->butts);
	free(m);
}

void drawGraph(Graph *g, int xp, int yp, int visit) {
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
		.cmd = 1,
	};
	if (butt->selected) {
		uint8_t color[3] = {255, 255, 0};
		memcpy(reco.data, color, sizeof(uint8_t) * 3);
	} else {
		uint8_t color[3] = {255, 255, 255};
		memcpy(reco.data, color, sizeof(uint8_t) * 3);
	}
	addRenderCommand(reco);
	reco.cmd = 0;
	Pos pos = {
		.x = xp + butt->pos[0],
		.y = screenY - (yp + butt->pos[1]),
	};
	memcpy(reco.data, &pos, sizeof(Pos));
	addRenderCommand(reco);
	for (int i = 0; i < g->maxNeighbors; i++) {
		drawGraph(g->neighbors[i], xp, yp, visit);
	}
}

void freeGraph(Graph *g, int visit) {
	if (!g || g->lastVisit == visit) {
		return;	
	}
	g->lastVisit = visit;
	for (int i = 0; i < g->maxNeighbors; i++) {
		freeGraph(g->neighbors[i], visit);
	}
	//freeGraphPoint(g);
	free(g->data);
	free(g->neighbors);
}
