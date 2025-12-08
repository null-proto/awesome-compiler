#include "linked_list.h"
#include <stdlib.h>


struct llist* llist_init(void* data) {
	struct llist* list = (struct llist*) malloc(sizeof(struct llist));
	list->data = data;
	list->next = (void*) 0;
	list->pre = (void*) 0;
	return list;
}

void llist_add(struct llist* list, void* data) {
	struct llist* new_list = (struct llist*) malloc(sizeof(struct llist));
	new_list->data = data;
	new_list->pre = list;
	new_list->next = (void*) 0;

	list->next = new_list;
	list = new_list;
}

void llist_free(struct llist* list) {
	for (;list->pre; list=list->pre);

	for (struct llist* next_tmp = list;list; next_tmp=list,list=list->next) {
		free(next_tmp);
	}
}
