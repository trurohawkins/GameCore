Menu *renderMenu = 0;

void pressUp(void *menu, float val) {
	if (val == 1) {
		menuMoveCursor(menu, 0);
	}
}

void pressLeft(void *menu, float val) {
	if (val == 1) {
		menuMoveCursor(menu, 1);
	}
}

void pressDown(void *menu, float val) {
	if (val == 1) {
		menuMoveCursor(menu, 2);
	}
}

void pressRight(void *menu, float val) {
	if (val == 1) {
		menuMoveCursor(menu, 3);
	}
}


void pressAccept(void *menu, float val) {
	if (val == 1) {
		pressButton(menu);
	}
}

void renderButts() {
	if (renderMenu != 0) {
		addMenu(renderMenu);
	}
}
