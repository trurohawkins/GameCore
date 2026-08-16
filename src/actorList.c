linkedList **ActorLists = 0;
int listsLength = 0;;
int curList = 0;

bool initActorLists(int num) {
	if (!ActorLists) {
		ActorLists = calloc(num, sizeof(linkedList*));
		for (int i = 0; i < num; i++) {
			ActorLists[i] = makeList();
		}
		listsLength = num;
		return true;
	} else {
		return false;
	}
}

void addActor(Actor *a) {
	addToList(&ActorLists[curList], a);
}

void removeActor(Actor *a) {
	removeFromList(&ActorLists[curList], a);
}

void deleteActorLists() {
	for (int i = 0; i < listsLength; i++) {
		deleteList(&ActorLists[i], deleteActor);
	}
}

void actorListDo() {
	linkedList *cur = ActorLists[curList];
	linkedList *pre = cur;
	while (cur) {
		if (cur->data) {
			Actor *a = cur->data;
			if (a->deleteMe) {
				linkedList *tmp = cur;
				deleteActor(a);
				cur->data = 0;
				if (pre == cur) {
					ActorLists[curList] = cur->next;
				} else {
					pre->next = cur->next;
				}
				cur = cur->next;
				free(tmp);
			} else if (a->active) {
				doActions(a);
			}
		}
		pre = cur;
		cur = cur->next;
	}
}

