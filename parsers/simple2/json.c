#include "main.h"
#include "token.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

enum json_types {
  JSON_STRING = 1,
  JSON_INT,
  JSON_BOOL,
  JSON_FLOAT,
  JSON_LIST,
  JSON_OBJECT,
};

struct key {
  char *str;
  size_t len;
};

struct value {
  enum json_types type;
  void *inner;
};

struct string {
  char *str;
  size_t len;
};

struct bool { unsigned d : 1; };

struct list {
  struct value **key;
  size_t len;
  size_t cappacity;
};

typedef struct {
  struct key **key;
  struct value **val;
  size_t len;
  size_t cappacity;
} object;

typedef struct json {
  object *main;
} json;

void object_add(object **obj, struct key **key, enum json_types type,
                void **inner) {
  struct value *v = malloc(sizeof(struct value));
  v->inner = *inner;
  v->type = type;

  unsigned i = (*obj)->len;

  if ((*obj)->len >= (*obj)->cappacity) {
    realloc((*obj)->key, (*obj)->cappacity * 2);
    realloc((*obj)->val, (*obj)->cappacity * 2);
    (*obj)->cappacity = (*obj)->cappacity * 2;
  }

  (*obj)->key[i] = *key;
  (*obj)->val[i] = v;
  (*obj)->len++;
}

object *object_new(unsigned cappacity) {
  object *v = malloc(sizeof(object));
  v->key = calloc(cappacity, sizeof(void *));
  v->val = calloc(cappacity, sizeof(void *));
  v->len = 0;
  v->cappacity = cappacity;

  return v;
}

struct string *string_new(char **pos, size_t len) {
  struct string *s = malloc(sizeof(struct string));
  s->str = *pos;
  s->len = len;

  return s;
}

struct list *list_new(unsigned cappacity) {
  struct list *l = malloc(sizeof(object));
  l->key = calloc(cappacity, sizeof(void *));
  l->len = 0;
  l->cappacity = cappacity;

  return l;
}

void list_add(struct list **list, enum json_types type, void **inner) {
  struct value *v = malloc(sizeof(struct value));
  v->inner = *inner;
  v->type = type;

  unsigned i = (*list)->len;

  if (i >= (*list)->cappacity) {
    realloc((*list)->key, (*list)->cappacity * 2);
    (*list)->cappacity = (*list)->cappacity * 2;
  }

  (*list)->key[i] = v;
  (*list)->len++;
}

struct value *value_new(enum json_types t, void **inner) {
  struct value *v = malloc(sizeof(struct value));
  v->type = t;
  v->inner = *inner;
  return v;
}

struct bool *bool_from_type(token_t t) {
  struct bool *b = malloc(sizeof(struct bool));
  b->d = t == T_BOOLEAN_TRUE ? 1 : 0;

  return b;
}

object *json_parse_object(token* stream ,char* const text) {
  object *obj = object_new(1);
	struct {
		unsigned is_key:1;
		unsigned is_list:1;
		unsigned is_object:1;
	} flags = {0};

  for (; stream; stream = stream->n) {

    switch (stream->type) {

    case T_OBJECT_OPEN: {
      break;
    }

    case T_OBJECT_CLOSE: {
      break;
    }

    case T_LIST_OPEN: {
      break;
    }

    case T_LIST_CLOSE: {
      break;
    }

    case T_STRING: {
      break;
    }

    case T_NUMBER_FLOAT: {
      break;
    }

    case T_NUMBER: {
      break;
    }

    case T_BOOLEAN_TRUE:
    case T_BOOLEAN_FALSE: {

      break;
    }

    case T_SPLIT: {
      break;
    }

    case T_COMMA: {
      break;
    }

    case T_UNK: {
      die("unknown token", stream->d, text);
      break;
    }

    default: {
      die("internal error", stream->d, text);
      break;
    }
    }
  }
  token_free(stream);

  return obj;
}

json* json_parse(char* const text) {
	json* json_ = calloc(1, sizeof(json));

  token *stream = token_parse(text);
  token_print(stream);

	json_->main = json_parse_object(stream,text);

	return json_;
}
