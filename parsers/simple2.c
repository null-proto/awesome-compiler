/// simple parser2 : parse json like string 
///
/// assumptions:
///  <target>*             -  zero or more possibility of same group
///  [ <target> ... ]      -  a group
///  <target1> | <target>  -  either right or left
///
/// `{ "key" : "value" , "key" : { "key" : "value" } }`
///
/// JSON       ::=  OBJECT
/// OBJECT     ::=  OPEN + KEY + SPLIT + VALUE + [ SEPARATOR + ... ] + CLOSE
/// KEY        ::=  STRING
/// VALUE      ::=  OBJECT | STRING | BOOLEAN | NUMBER | LIST
/// LIST       ::=  LIST_OPEN + [ STRING | BOOLEAN | NUMBER ]* + LIST_CLOSE
/// STRING     ::=  QUOTES + CHARACTERS + QUOTES
///
/// CHARACTERS ::=  `type CStr` ie. `struct { char* data; size_t len; }`
/// NUMBER     ::=  32bit signed integer
///
/// BOOLEAN    ::=  `true` | `false`
/// LIST_OPEN  ::=  `[`
/// LIST_CLOSE ::=  `]`
/// QUOTES     ::=  `"`
/// OPEN       ::=  `{`
/// CLOSE      ::=  `}`
/// SEPARATOR  ::=  `,`


int main(int argc , char** argv) {

}
