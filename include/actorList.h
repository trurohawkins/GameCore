#ifndef ACTORLIST
#define ACTORLIST

bool initActorLists(int num);
void addActor(Actor *a);
void removeActor(Actor *a);
void deleteActorLists();
void actorListDo(float delta);

#endif
