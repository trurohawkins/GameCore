#include "GameCore.h"
#include "TUI.h"

bool runGraphics = true;

#include "menuControls.c"

int main() {
	srand(time(NULL));
	initDirections();
	initCore();
	initGame();
	initTermInput();
	if (runGraphics) {
		initScreen();
	}
	gameLoop = &loop;
	pthread_t gameThread = createThread(runGame, NULL, false);
	pthread_t outputThread = 0;
	if (runGraphics > 0) {
		 outputThread = createThread(outputLoop, NULL, false);
	}
	
	Menu *menu = makeMenu(2, 3, 20, 10);
	Player *god = makePlayer(menu, 0, 0);
	renderMenu = menu;
	addPlayer(god);
	addControl(god, "K0W", pressUp);
	addControl(god, "K0A", pressLeft);
	addControl(god, "K0S", pressDown);
	addControl(god, "K0D", pressRight);
	coreLoop();


	pthread_join(gameThread, NULL);
	if (runGraphics > 0) {
		pthread_join(outputThread, NULL);
	}
	exitCore();
	closeGame();
	exitTermInput();
	if (runGraphics > 0) {
		exitScreen();
	}
	deleteMenu(menu);
	return 0;
}
