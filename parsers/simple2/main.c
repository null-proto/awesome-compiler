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
/// CHARACTERS ::=  `type CStr` ie. `struct { char* data; size_t len; }`
/// NUMBER     ::=  INT | FLOAT
/// INT        ::=  '0' to '9' + [INT]
/// FLOAT      ::=  INT + '.' + INT
/// BOOLEAN    ::=  `true` | `false`
/// LIST_OPEN  ::=  `[`
/// LIST_CLOSE ::=  `]`
/// QUOTES     ::=  `"`
/// O_OPEN     ::=  `{`
/// O_CLOSE    ::=  `}`
/// COMMA      ::=  `,`

#include "token.h"
#include "main.h"

#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  if (argc != 2) {
    printf("Usage:\n");
    printf("%s  [json-file]\n", *argv);
    return 1;
  }

  if (access(*(++argv), F_OK)) {
    fprintf(stderr, "no such files or no directory\n");
    return 1;
  }

  printf("reading file: %s\n", *argv);
  char *json = read_file(*argv);
  free(json);
  return 0;
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

void die(const char *reason, char* pos, char *file) {
	// h & v pos
	long unsigned line = 0;
	char* i = 0;
	for (; file < pos; file++) {
		if (*file == '\n') {
			line++;
			i = file;
		}
	}

	long unsigned hpos = pos - i;

	fprintf(stderr, "Error: %s\n", reason);
	fprintf(stderr, "on %ld:%ld", line, hpos);
	fflush(stderr);
	for (; !*file || *file != '\n' || file < pos + 10; i++) {
		write(STDERR_FILENO, file, 1);
	}
	fflush(stderr);
	fprintf(stderr, "\n");
	fflush(stderr);
	exit(1);
}

