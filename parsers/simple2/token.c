#include "stdio.h"
#include "unistd.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

void token_add(token **t, token_t type, char *data, size_t l) {
  if (*t == NULL) {
    token *new = calloc(1, sizeof(token));
    new->type = type;
    new->len = l;
    new->d = data;
    *t = new;
  } else {

    token *new = calloc(1, sizeof(token));
    new->type = type;
    new->len = l;
    new->d = data;
    new->p = *t;
    (*t)->n = new;
    *t = new;
  }
}

token *token_parse(char *stream) {
  token *tk = NULL;

  struct {
    unsigned quotes : 1;
    unsigned group : 1;
  } flags = {0, 0};

  for (; *stream; stream++) {
    if (tk && flags.quotes && tk->type == T_STRING) {
      tk->len++;
      if (*stream == '"')
        flags.quotes = !flags.quotes;
      continue;
    }

    switch (*stream) {
    case '"': {
      flags.quotes = !flags.quotes;
      token_add(&tk, T_STRING, stream, 1);
      break;
    }

    case ' ':
    case '\n':
    case '\t': {
      if (flags.group)
        flags.group = !flags.group;
      break;
    }

    case ',': {
      token_add(&tk, T_COMMA, stream, 1);
      if (flags.group)
        flags.group = !flags.group;
      break;
    }

    case ':': {
      token_add(&tk, T_SPLIT, stream, 1);
      if (flags.group)
        flags.group = !flags.group;
      break;
    }

    case '{': {
      token_add(&tk, T_OBJECT_OPEN, stream, 1);
      break;
    }

    case '}': {
      token_add(&tk, T_OBJECT_CLOSE, stream, 1);
      if (flags.group)
        flags.group = !flags.group;
      break;
    }

    case '[': {
      token_add(&tk, T_LIST_OPEN, stream, 1);
      break;
    }

    case ']': {
      token_add(&tk, T_LIST_CLOSE, stream, 1);
      if (flags.group)
        flags.group = !flags.group;
      break;
    }

    default: {
      if (!flags.group) {
        if (*stream == 't' && strncmp(stream, "true", 4)) {
          token_add(&tk, T_BOOLEAN_TRUE, stream, 4);
          stream += 3;
        }

        else if (*stream == 'f' && strncmp(stream, "false", 5)) {
          token_add(&tk, T_BOOLEAN_FALSE, stream, 5);
          stream += 4;
        }

        else if (*stream <= '9' && *stream >= '0') {
          flags.group = 1;
          token_add(&tk, T_NUMBER, stream, 1);

        } else if (tk && tk->type == T_UNK) {
          tk->len++;

        } else {
          token_add(&tk, T_UNK, stream, 1);
        }
      } else {
        // group = true
        if (*stream <= '9' && *stream >= '0') {
          tk->len++;
				} else if (*stream == '.' && stream+1 && *(stream+1) <= '9' && *(stream+1) >= '0') {
					tk->type = T_NUMBER_FLOAT;
          tk->len++;
        } else {
          token_add(&tk, T_UNK, stream, 1);
        }
      }
    }
    }
  }
  for (; tk->p; tk = tk->p)
    ;
  return tk;
};

void token_print(token *tk) {
	const char* e = " ";
  for (; tk; tk = tk->n) {
    write(STDOUT_FILENO, tk->d, tk->len);
    write(STDOUT_FILENO, e, 1);
  }
  fflush(stdout);
}

void token_free(token *tk) {
  for (token *piv = tk; piv ; piv = tk) {
		tk = tk->n;
    free(piv);
	}
}

