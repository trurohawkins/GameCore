#include "GameCore.h"
#include "TUI.h"

bool runGraphics = true;

int main() {
	srand(time(NULL));
	initDirections();
	initCore();
	initGame();
	initTermInput();
	if (runGraphics) {
		initScreen();
	}

	pthread_t gameThread = createThread(runGame, NULL, false);
	pthread_t outputThread = 0;
	if (runGraphics > 0) {
		 outputThread = createThread(outputLoop, NULL, false);
	}
	
	Graph *menu = makeMenu(2, 2, 10);
	startRendering();
	addMenu(menu, 30, 30);
	sendRenderFrame();
	
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
