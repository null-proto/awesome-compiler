/// simple parser2 : parse json like string
///
/// Assumptions:
///  <target>*             -  zero or more possibility of same group
///  [ <target> ... ]      -  optional
///  <target1> | <target>  -  either right or left
///
///
/// Sample: `{ "key" : "value" , "key" : { "key" : "value" } }`
///
///
/// Grammar:
///
/// JSON       ::=  OBJECT
/// OBJECT     ::=  O_OPEN + INNER + O_CLOSE
/// INNER      ::=  KEY + SPLIT + VALUE + [ COMMA + INNER ]
///
/// KEY        ::=  STRING
/// VALUE      ::=  OBJECT | STRING | BOOLEAN | NUMBER | LIST
///
/// LIST       ::=  LIST_OPEN + LIST_INNER + LIST_CLOSE
/// LIST_INNER ::=  STRING | BOOLEAN | NUMBER + [ COMMA + LIST_INNER ]
///
/// STRING     ::=  QUOTES + CHARACTERS + QUOTES
///
/// CHARACTERS ::=  `type CStr` ie. `struct { char* data; size_t len; }`
/// NUMBER     ::=  32bit signed integer
///
/// BOOLEAN    ::=  `true` | `false`
/// LIST_OPEN  ::=  `[`
/// LIST_CLOSE ::=  `]`
/// QUOTES     ::=  `"`
/// O_OPEN       ::=  `{`
/// O_CLOSE      ::=  `}`
/// COMMA      ::=  `,`

#include "stdio.h"
#include "unistd.h"
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
  BOOLEAN_TRUE,
  BOOLEAN_FALSE,
  COMMA,
  UNK,
} type_t;

struct token_h {
  type_t type;
  char *d;
  size_t len;
  struct token_h *n;
  struct token_h *p;
};
typedef struct token_h token;

static unsigned a=0;

inline void token_add(token **t, type_t type, char *data, size_t l) {
  if (*t == NULL) {
    token* new = calloc(1,sizeof(token));
    new->type = type;
    new->len = l;
    new->d = data;
		*t = new;
  } else {

    token *new = calloc(1,sizeof(token));
    new->type = type;
    new->len = l;
    new->d = data;
    new->p = *t;
    (*t)->n = new;
    *t = new;
  }

	printf("CALL%d: d:%p n:%p p:%p type:%d len:%ld\n", a , (*t)->d,(*t)->n,(*t)->p , (*t)->type, (*t)->len);
}

token *token_parse(char *stream) {
  token *tk = NULL;

  struct {
    unsigned quotes : 1;
    unsigned group : 1;
  } flags = {0, 0};

  for (; *stream; stream++) {
    if (tk && flags.quotes && tk->type == STRING) {
      tk->len++;
      if (*stream == '"')
        flags.quotes = !flags.quotes;
      continue;
    }

    switch (*stream) {
    case '"': {
      flags.quotes = !flags.quotes;
      token_add(&tk, STRING, stream, 1);
      break;
    }

    case ' ':
    case '\n':
    case '\t': {
      if (flags.group) flags.group = !flags.group;
			break;
    }

    case ',': {
      token_add(&tk, COMMA, stream, 1);
      if (flags.group) flags.group = !flags.group;
      break;
    }

		case ':': {
      token_add(&tk, SPLIT, stream, 1);
      if (flags.group) flags.group = !flags.group;
      break;
    }

    case '{': {
      token_add(&tk, OBJECT_OPEN, stream, 1);
      break;
    }

    case '}': {
      token_add(&tk, OBJECT_CLOSE, stream, 1);
      if (flags.group) flags.group = !flags.group;
      break;
    }

    case '[': {
      token_add(&tk, LIST_OPEN, stream, 1);
      break;
    }

    case ']': {
      token_add(&tk, LIST_CLOSE, stream, 1);
      if (flags.group) flags.group = !flags.group;
      break;
    }

    default: {
      if (!flags.group) {
        if (*stream == 't' && strncmp(stream, "true", 4)) {
          token_add(&tk, BOOLEAN_TRUE, stream, 4);
          stream += 3;
        }

        else if (*stream == 'f' && strncmp(stream, "false", 5)) {
          token_add(&tk, BOOLEAN_FALSE, stream, 5);
          stream += 4;
        }

        else if (*stream <= '9' && *stream >= '0') {
          flags.group = 1;
          token_add(&tk, NUMBER, stream, 1);

        } else if (tk && tk->type == UNK) {
          tk->len++;

        } else {
          token_add(&tk, UNK, stream, 1);
        }
      } else {
        // group = true
        if (tk && *stream <= '9' && *stream >= '0') {
          tk->len++;
        } else {
          token_add(&tk, UNK, stream, 1);
        }
      }
    }
    }
  }
  for (; tk->p; tk = tk->p) ;
  return tk;
};

void token_print(token *tk) {
	printf(" ==== ");
  for (; tk; tk = tk->n) {
    write(STDOUT_FILENO,(char*) tk->d, tk->len);
    printf(" ");
  }
	fflush(stdout);
}

void token_free(token *tk) {
  for (token *piv = tk; piv; piv = tk->n, tk = tk->n)
    free(piv);
}

char *read_file(char *path) {
  FILE *f = fopen(path, "rb");
  fseek(f, 0, SEEK_END);
  size_t len = ftell(f);
  rewind(f);
  char *str = malloc(len * sizeof(char) + sizeof(char));
  fread(str, 1, len, f);
  str[len] = '\0';
  fclose(f);
  return str;
}

int main(int argc, char **argv) {

  if (argc > 2) {
    printf("Usage:\n");
    printf("%s  [json-file]", *argv);
    return 1;
  }

  if (access(*(++argv), F_OK)) {
    fprintf(stdout, "no such files or no directory\n");
    return 1;
  }

  printf("reading file: %s\n", *argv);
  char *json = read_file(*argv);
  token* tk = token_parse(json);
  token_print(tk);
  token_free(tk);
  free(json);
  return 0;
}
