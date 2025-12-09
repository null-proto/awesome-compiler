#pragma once

struct llist {
	void* data;
	struct llist* next;
	struct llist* pre;

	void* (*add)(void);
};


struct llist* llist_init(void* data);

void llist_add(struct llist* list, void* data);

void llist_free(struct llist* list);
