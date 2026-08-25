#define _GNU_SOURCE
#include "game.h"
#include "OIB.h"

#include "timeWizard.h"
#include "player.h"
#include "inputMap.h"
#include "playerManager.h"
#include "actor.h"
#include "actorList.h"
#include "menu.h"

PollHandler gameTimer = {
	.fd = -1
};	

TimeWizard gameWiz;
int ticksPerSecond = 60;
bool gameRunning = true;
bool paused = false;
bool renderNewShot = true;
bool quickExit = true;
int actorLists = 2;

void (*gameLoop)(float) = 0;
void (*renderFunc)(void) = 0;
void (*resizeScreen)(int, int) = 0;
void (*pauseFunc)(bool) = 0;

bool initGame() {
	initPollSystem(&gamePoll, &receiveEvent);

	initTimerFd(&gameTimer, ticksPerSecond, &gameSimulation);
	addFdToPoll(&gameTimer, gamePoll.pfd);

	initTimeWizard(&gameWiz, ticksPerSecond);

	initActorLists(actorLists);
	makePlayerManager();

	return true;
}

void *runGame(void *data) {
	pthread_setname_np(pthread_self(), "Game");
	while (gameRunning) {
		runPolls(gamePoll.pfd, gamePoll.polls, 16);
	}
	return NULL;
}

void exitGame() {
	gameRunning = false;
	exitCoreLoop();
}

void closeGame() {
	if (gameTimer.fd != -1) {
		close (gameTimer.fd);
	}
	closePoll(gamePoll);
	freePlayerManager();
	deleteActorLists();
}

void setNewRender() {
	renderNewShot = true;
}

void gameSimulation() {
	// used for draining fd
	uint64_t drain;
	if (read(gameTimer.fd, &drain, sizeof(drain)) == -1) {
		perror("readding timer fd for simulations");
		return;
	}
	updateTimeWizard(&gameWiz);

	int steps = consumeTicks(&gameWiz);
	if (!paused) {
		for (int i = 0; i < steps; i++) {
			actorListDo(gameWiz.dt);
			if (gameLoop) {
				gameLoop(gameWiz.dt);
			}
		}
	}
	if (renderFunc && renderNewShot) {
		startRendering();
		renderFunc();
		sendRenderFrame();
		renderNewShot = false;
	}
}

void receiveEvent() {
	// drain event fd
	uint64_t count;
	if (read(gamePoll.handler.fd, &count, sizeof(count)) == -1) {
		perror("read event fd");
		return;
	}

	// read events in queue
	SystemEvent se;
	while (popEvent(&se)) {
		if (se.type == STDIN_FILENO) {
			KeyEvent ke;
			memcpy(&ke, se.data, se.size);
			if (ke.key == 27 && ke.val == 1 && quickExit) {
				exitGame();
			} else {
				// sends input to player manager
				makeKeyInput(ke.key, ke.val);
			}
		} else if (se.type == 1) {
			int data[2];
			memcpy(&data, se.data, se.size);
			updateScreenSize(data[0], data[1]);
			// optinal resize logic can be added
			if (resizeScreen) {
				resizeScreen(data[0], data[1]);
				renderNewShot = true;
			}
		}
	}
}


void toggleGamePause() {
	setGamePause(!paused);
}

void setGamePause(bool state) {
	paused = state;
	if (pauseFunc) {
		pauseFunc(paused);
	}
}

bool getPaused() {
	return paused;
}

