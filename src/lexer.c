#include <lexer.h>
#include <stdlib.h>
#include <string.h>

TokenArray* lex(const char* s) {
	TokenArray* tka = (TokenArray*) malloc(sizeof(TokenArray));
	TokenArray* tk = tka;
	TOKEN_T type;

	while (*s) {
		while(*s && *s == ' ') s++;
		if (!*s) break;

		char* start = s;
		while(*s !=' ') s++;

		tk->tk = (Token*) malloc(sizeof(Token));
		tk->tk->str = start;
		tk->tk->len = s-start;
	}

	return tka;
}
