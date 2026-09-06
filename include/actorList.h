#ifndef ACTORLIST
#define ACTORLIST

typedef struct {
	linkedList **lists;
	int numLists;
	int curList;
	bool deleteMe;
	bool doing;
} ActorLists;

bool initActorLists(int num);
void addActor(Actor *a);
void removeActor(Actor *a);
void deleteActorLists();
void actorListDo(float delta);

#endif
