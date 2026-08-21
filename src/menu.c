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
			for (int i = 0; i < 4; i++) {
				int *d = getDir4(i);
				int xp = ((x + d[0]) % columns + columns) % columns;
				int yp = ((y + d[1] % rows + rows) % rows);
				//int pos[2] = {(x + d[0]) % columns, (y + d[1]) % rows};
				if (xp >= 0 && yp >= 0 && xp < columns && yp < rows) {
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
			setNewRender();
		}
	}
}

void pressButton(Menu *m) {
	Button *butt = m->selected->data;
	if (butt->func) {
		butt->func();
	}
}

Button *getButton(Menu *m, int xp, int yp) {
	Graph *cur = getGraph(m, xp, yp);
	return cur->data;
}

void selectButton(Menu *m, int xp, int yp) {
	Graph *new = getGraph(m, xp, yp);
	if (new) {
		Button *old = m->selected->data;
		old->selected = false;
		m->selected = new;
		((Button*)new->data)->selected = true;
	}

}

Graph *getGraph(Menu *m, int xp, int yp) {
	Graph *cur = m->butts;
	if (xp != 0) {
		int xd = 3;
		if (xp < 0) {
			xd = 1;
		}
		for (int i = 0; i < abs(xp); i++) {
			if (cur->neighbors[xd]) {
				cur = cur->neighbors[xd];
			}
		}
	}
	if (yp != 0) {
		int yd = 0;
		if (yp < 0) {
			yd = 2;
		}
		for (int i = 0; i < abs(yp); i++) {
			if (cur->neighbors[yd]) {
				cur = cur->neighbors[yd];
			}
		}
	}
	return cur;
}

void nameButton(Button *butt, char *name) {
	TextBox *tBox = getTextBox(butt->textBox);
	memcpy(tBox->string, name, strlen(name));
}

void addMenu(Menu *m) {
	drawGraph(m->butts, m->pos[0] * screenX, m->pos[1] * screenY, m->butts->lastVisit + 1);
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
