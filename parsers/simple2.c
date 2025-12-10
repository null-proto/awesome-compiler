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

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  OBJECT_OPEN = 1,
  OBJECT_CLOSE,
  LIST_OPEN,
  LIST_CLOSE,
  STRING,
  NUMBER,
  BOOLEAN_TRUE,
  BOOLEAN_FALSE,
  COMMA,
  UNK,
} type_t;

typedef struct token {
  type_t type;
  char *d;
  size_t len;
  struct token *n;
  struct token *p;
} token;

void add_token(token *t, type_t type, char *data, size_t l) {
  if (t == NULL) {
    t = malloc(sizeof(token));
    t->type = type;
    t->len = l;
    t->n = NULL;
		t->p = NULL;
    t->d = data;
  } else {
    token *new = malloc(sizeof(token));
    new->type = type;
    new->len = l;
    new->n = NULL;
		new->p = t;
    new->d = data;
    t->n = new;
    t = new;
  }
}

token *parse(char *stream) {
  token *tk = NULL;
  token *f = tk;

  struct {
    unsigned quotes : 1;
    unsigned group : 1;
  } flags;

  for (; stream; stream++) {

    if (flags.quotes && tk->type == STRING) {
      tk->len++;
      if (*stream == '"')
        flags.quotes = !flags.quotes;
      continue;
    }

    switch (*stream) {
    case ' ' | '\n': {
      flags.group = !flags.group;
    }

    case '"': {
      flags.quotes = !flags.quotes;
      add_token(tk, STRING, stream, 1);
      break;
    }

    case ',': {
      add_token(tk, COMMA, stream, 1);
      break;
    }

    case '{': {
      add_token(tk, OBJECT_OPEN, stream, 1);
      break;
    }

    case '}': {
      add_token(tk, OBJECT_CLOSE, stream, 1);
      break;
    }

    default: {
      if (!flags.group) {
        if (*stream == 't' && strncmp(stream, "true", 4)) {
          add_token(tk, BOOLEAN_TRUE, stream, 4);
          stream += 3;
        }

        else if (*stream == 'f' && strncmp(stream, "false", 5)) {
          add_token(tk, BOOLEAN_FALSE, stream, 5);
          stream += 4;
        }

        else if (*stream <= '9' && *stream >= '0') {
          flags.group = 1;
          add_token(tk, NUMBER, stream, 1);

        } else if (tk->type == UNK) {
          tk->len++;

        } else {
          add_token(tk, UNK, stream, 1);
        }
      } else {
        // group = true
        if (*stream <= '9' && *stream >= '0') {
          tk->len++;
        } else {
          add_token(tk, UNK, stream, 1);
        }
      }
    }
    }
  }
  return f;
};

int main(int argc, char **argv) {}
