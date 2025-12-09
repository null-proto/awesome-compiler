#pragma once

#include <stddef.h>

typedef struct {
	char** kstore;
	size_t cappacity;
} hash_table;

hash_table* new(int cappacity);

void insert(hash_table* t, char* k , char* v);

void remove(hash_table* t, char* k);

char* get(hash_table* t, char* k);
