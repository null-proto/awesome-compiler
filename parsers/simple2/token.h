#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  T_OBJECT_OPEN = 1,
  T_OBJECT_CLOSE,
  T_LIST_OPEN,
  T_LIST_CLOSE,
  T_SPLIT,
  T_STRING,
  T_NUMBER,
	T_NUMBER_FLOAT,
  T_BOOLEAN_TRUE,
  T_BOOLEAN_FALSE,
  T_COMMA,
  T_UNK,
} token_t;

struct token_ {
  token_t type;
  char *d;
  size_t len;
  struct token_ *n;
  struct token_ *p;
};

typedef struct token_ token;

void token_add(token **t, token_t type, char *data, size_t l);

token *token_parse(char *stream);

void token_print(token *tk);

void token_free(token *tk);

char *read_file(char *path);
