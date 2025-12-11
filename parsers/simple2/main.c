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

#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
  token *tk = token_parse(json);
  token_print(tk);
  token_free(tk);
  free(json);
  return 0;
}
