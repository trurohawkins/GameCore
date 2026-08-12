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

void loop(float delta) {
	if (renderMenu != 0) {
		startRendering();
		addMenu(renderMenu, 30, 15);
		sendRenderFrame();
	}
}
