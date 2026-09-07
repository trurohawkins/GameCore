#ifndef PLAYMAN
#define PLAYMAN
typedef struct PlayerManager {
	linkedList *playerList;
	int curPlayers;
} PlayerManager;

void makePlayerManager();
Player *checkPlayer(int num);
Player *addPlayer(void *character);
void removePlayer(Player *p);
//void processKeys(bool paused);
void makeKeyInput(char c, float val);
void processInput(inpReceived ir);
void freePlayerManager();
#endif
