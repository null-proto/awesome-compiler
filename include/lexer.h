

#include <stddef.h>
typedef enum {
	SET,
	VAR,
	VAL,
} TOKEN_T;


typedef struct {
	TOKEN_T type;
	const char* str;
	size_t len;
} Token;

struct TokenArray {
	Token* tk;
	struct TokenArray* next;
};

typedef struct TokenArray TokenArray;

TokenArray* lex(const char* s);

#pragma once
