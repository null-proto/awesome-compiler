/// simple parser1: parser key-value pare
///
///  "key:value ..."
///
///  parser    ::=  (key split value) [separator] ...
///  key       ::=  string
///  value     ::=  string
///  split     ::=  ':'
///  separator ::=  ' '


#include <stdio.h>
#include <string.h>

typedef enum {
	STRING,
	SEPARATOR,
	SPLIT
} token_t;

struct Tokens {
	char* data;
	size_t* len;
	token_t type;
};


int main() {

  char *data = "key:value key1:value";

  printf("input: '%s'\n", data);

  for (int i; i < strlen(data); i++) {

  }

  return 0;
}
