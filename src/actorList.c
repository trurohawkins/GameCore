#include "actorList.h"
ActorLists *actorLists = 0;

bool initActorLists(int num) {
	if (!actorLists) {
		actorLists = calloc(1, sizeof(ActorLists));
		actorLists->lists = calloc(num, sizeof(linkedList*));
		for (int i = 0; i < num; i++) {
			actorLists->lists[i] = makeList();
		}
		actorLists->numLists = num;
		actorLists->curList = 0;
		return true;
	} else {
		return false;
	}
}

void addActor(Actor *a) {
	addToList(&actorLists->lists[actorLists->curList], a);
}

void removeActor(Actor *a) {
	removeFromList(&actorLists->lists[actorLists->curList], a);
}

void deleteActorLists() {
	if (actorLists) {
		if (actorLists->doing) {
			actorLists->deleteMe = true;
			return;
		}
		for (int i = 0; i < actorLists->numLists; i++) {
			deleteList(&actorLists->lists[i], deleteActor);
			actorLists->lists[i] = 0;
		}
		free(actorLists->lists);
		free(actorLists);
		actorLists = 0;
	}
}

void actorListDo(float delta) {
	if (!actorLists) {
		return;
	}
	linkedList *cur = actorLists->lists[actorLists->curList];
	linkedList *pre = cur;
	actorLists->doing = true;
	while (cur) {
		if (cur->data) {
			Actor *a = cur->data;
			if (a->deleteMe) {
				linkedList *tmp = cur;
				deleteActor(a);
				cur->data = 0;
				if (pre == cur) {
					actorLists->lists[actorLists->curList] = cur->next;
					free(tmp);
					break;
				} else {
					pre->next = cur->next;
				}
				cur = cur->next;
				free(tmp);
			} else if (a->active) {
				doActions(a, delta);
			}
		}
		pre = cur;
		if (cur) {
			cur = cur->next;
		}
	}
	actorLists->doing = false;
	if (actorLists->deleteMe) {
		deleteActorLists();
	}

}

