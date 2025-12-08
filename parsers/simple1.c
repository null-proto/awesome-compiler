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

typedef enum { STRING, SEPARATOR, SPLIT } token_t;

struct token {
  char *data;
  size_t len;
  token_t type;
  struct token *next;
};

struct token *parse(char *str, size_t len) {
  struct token *tokens = malloc(sizeof(struct token));

  struct token *list = tokens;

  for (size_t i = 0; i < len; i++) {

    switch (str[i]) {
    case ':': {
      struct token *new = malloc(sizeof(struct token));
      tokens->next = new;
      tokens = new;
      tokens->data = &str[i];
      tokens->type = SPLIT;
      tokens->len = 1;
      break;
    }
    case ' ': {
      if (tokens->type != SEPARATOR) {
        struct token *new = malloc(sizeof(struct token));
        tokens->next = new;
        tokens = new;
        tokens->data = &str[i];
        tokens->type = SEPARATOR;
        tokens->len = 1;
      }
      break;
    }
    default:
      if (tokens->data == NULLPRT) {
        tokens->data = &str[i];
        tokens->type = STRING;
        tokens->len = 1;
      } else {
        (tokens->len)++;
      }
      break;
    }
  }
  return list;
}

void print(struct token *tk) {
  for (struct token *i = tk; i; i = i->next) {
    write(STDOUT_FILENO,(char*)(i->data), i->len);
    printf(" ");
  }
}

void free_tokens(struct token *tk) {
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
