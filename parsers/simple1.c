/// simple parser1: parser key-value pare
///
///  "key:value ..."
///
///  parser    ::=  (key split value) [separator] ...
///  key       ::=  string
///  value     ::=  string
///  split     ::=  ':'
///  separator ::=  ' '

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#define NULLPRT (void *)0

typedef enum { STRING = 1, SEPARATOR, SPLIT } token_t;

struct token {
  char *data;
  size_t len;
  token_t type;
  struct token *next;
  struct token *pre;
};

struct token *parse(char *str, size_t len) {
  struct token *tokens = NULL;

  for (size_t i = 0; i < len; i++) {

    switch (str[i]) {
    case ':': {
      tokens->next = calloc(1, sizeof(struct token));
				tokens->next->pre = tokens;
      tokens = tokens->next;

      tokens->data = &str[i];
      tokens->type = SPLIT;
      tokens->len = 1;
      break;
    }
    case ' ': {
      if (tokens->type != SEPARATOR) {
        tokens->next = calloc(1, sizeof(struct token));
				tokens->next->pre = tokens;
        tokens = tokens->next;

        tokens->data = &str[i];
        tokens->type = SEPARATOR;
        tokens->len = 1;
      }
      break;
    }
    default:
      if (!tokens) {
        tokens = calloc(1, sizeof(struct token));
				tokens->type = STRING;
        goto fill;
      }
      if (tokens->type != STRING) {
        tokens->next = calloc(1, sizeof(struct token));
				tokens->next->pre = tokens;
        tokens = tokens->next;
      fill:
        tokens->data = &str[i];
        tokens->type = STRING;
        tokens->len = 1;
      } else {
        (tokens->len)++;
      }
      break;
    }
  }
	for (;tokens->pre;tokens = tokens->pre){};
  return tokens;
}

void print(struct token *tk) {
	for (;tk->pre;tk = tk->pre){};
  for (struct token *i = tk; i; i = i->next) {
    // printf("TOK { \n type: %d\n len: %ld\n data: %p\n next: %p\n}\n", i->type,
    //        i->len, i->data, i->next);
    switch (i->type) {
    case STRING: {
      write(STDOUT_FILENO, (char *)(i->data), i->len);
			fflush(stdout);
      break;
    }
    case SPLIT: {
      printf(" -> ");
			fflush(stdout);
      break;
    }
    case SEPARATOR: {
      printf("\n");
			fflush(stdout);
      break;
    }
    default: {
      break;
    }
    }
  }
}

void free_tokens(struct token *tk) {
	for (;tk->pre;tk = tk->pre){};
  for (struct token *i = tk; i; i = tk) {
    tk = tk->next;
    free(i);
  }
}

int main() {
  char *data = "key:value key1:value key3:value2";
  printf("input: '%s'\n", data);

  struct token *tokens = parse(data, strlen(data));
  print(tokens);

  free(tokens);
  return 0;
}
