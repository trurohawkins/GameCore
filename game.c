#define _GNU_SOURCE
#include "game.h"
#include "OIB.h"

#include "timeWizard.h"
#include "player.h"
#include "inputMap.h"
#include "playerManager.h"
#include "actor.h"
#include "actorList.h"

PollHandler gameTimer = {
	.fd = -1
};	

TimeWizard gameWiz;
int ticksPerSecond = 60;
bool gameRunning = true;
bool paused = false;

void (*gameLoop)(float) = 0;
void (*resizeScreen)(int, int) = 0;
void (*pauseFunc)(bool) = 0;

bool initGame() {
	initPollSystem(&gamePoll, &receiveEvent);

	initTimerFd(&gameTimer, ticksPerSecond, &gameSimulation);
	addFdToPoll(&gameTimer, gamePoll.pfd);

	initTimeWizard(&gameWiz, ticksPerSecond);

	makeActorList();
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
	atomic_store_explicit(&running, false, memory_order_release);
	wakeEvent();
}

void closeGame() {
	if (gameTimer.fd != -1) {
		close (gameTimer.fd);
	}
	closePoll(gamePoll);
	freePlayerManager();
	deleteActorList();
}

void gameSimulation() {
	uint64_t expirations;
	// used for draining
	if (read(gameTimer.fd, &expirations, sizeof(expirations)) == -1) {
		perror("readding timer fd for simulations");
		return;
	}
	updateTimeWizard(&gameWiz);
	//paceFunction(&gameWiz, simulateStep);
	int steps = consumeTicks(&gameWiz);
	char buff[100];
	if (gameLoop && !paused) {
		for (int i = 0; i < steps; i++) {
			actorListDo();
			gameLoop(gameWiz.dt);
		}
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
			if (ke.key == 27 && ke.val == 1) {
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

