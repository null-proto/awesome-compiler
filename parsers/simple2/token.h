#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  OBJECT_OPEN = 1,
  OBJECT_CLOSE,
  LIST_OPEN,
  LIST_CLOSE,
  SPLIT,
  STRING,
  NUMBER,
	NUMBER_FLOAT,
  BOOLEAN_TRUE,
  BOOLEAN_FALSE,
  COMMA,
  UNK,
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
