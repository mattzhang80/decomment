#include <stdio.h>
#include <stdlib.h>

enum State {
  START,
  BEGIN_COMMENT,
  IN_COMMENT,
  END_COMMENT,
  IN_STRING_LIT,
  IN_CHAR_LIT,
  ESC_STRING,
  ESC_CHAR
};

enum State handleStart(int currChar) {
  enum State state;
  state = START;
  if (currChar == '/')
    state = BEGIN_COMMENT;
  else if (currChar == '\'') {
    putchar(currChar);
    state = IN_CHAR_LIT;
  } else if (currChar == '"') {
    putchar(currChar);
    state = IN_STRING_LIT;
  } 
  else if (currChar == '\n') { 
    putchar(currChar);
    state = START;
  }
  else {
    putchar(currChar);
  }
  return state;
}

enum State handleCommentStart(int currChar) {
  enum State state;
  state = BEGIN_COMMENT;
  if (currChar == '*') {
    putchar(' ');
    state = IN_COMMENT;
  } 
  else if (currChar == '/') {
    putchar('/');
    state = BEGIN_COMMENT;
  }
  else if (currChar == '\''){
    putchar('/');
    putchar(currChar);
    state = IN_CHAR_LIT;
  }
  else if (currChar == '"'){
    putchar('/');
    putchar(currChar);
    state = IN_STRING_LIT;
  }
  else {
    putchar('/');
    putchar(currChar);
    state = START;
  }
  return state;
}

enum State handleInComment(int currChar) {
  enum State state;
  state = IN_COMMENT;
  if(currChar == '\n')
    putchar('\n');
  else if (currChar == '*')
    state = END_COMMENT;
  return state; 
}

enum State handleEndComment(int currChar) {
  enum State state;
  if (currChar == '/'){
    state = START;
    return state;
  }
  else if (currChar == '*')
    state = END_COMMENT;
  else if (currChar == '\n'){
    putchar('\n');
    state = IN_COMMENT;
  }
  else 
    state = IN_COMMENT;
  return state;
}

enum State handleInStringLit(int currChar) {
  enum State state;
  state = IN_STRING_LIT;
  if (currChar == '"')
    state = START;
  else if (currChar == '\\')
    state = ESC_STRING;
  putchar(currChar);
  return state;
}

enum State handleInCharLit(int currChar) {
  enum State state;
  state = IN_CHAR_LIT;
  if (currChar == '\'')
    state = START;
  else if (currChar == '\\')
    state = ESC_CHAR;
  putchar(currChar);
  return state;
}

enum State handleEscString(int currChar) {
  putchar(currChar);
  return IN_STRING_LIT;
}

enum State handleEscChar(int currChar) {
  putchar(currChar);
  return IN_CHAR_LIT;
}

int main(int argc, char *argv[]) {
  enum State state;
  int currChar;
  int currline = 1;
  int linestart = 0;
  state = START;
  while ((currChar = getchar()) != EOF) {
    if(currChar == '\n'){
      currline++;
    }
    switch (state) {
      case START:
        state = handleStart(currChar);
        break;
      case BEGIN_COMMENT:
        linestart = currline;
        state = handleCommentStart(currChar);
        break;
      case IN_COMMENT:
        state = handleInComment(currChar);
        break;
      case END_COMMENT:
        state = handleEndComment(currChar);
        break;
      case IN_STRING_LIT:
        state = handleInStringLit(currChar);
        break;
      case IN_CHAR_LIT:
        state = handleInCharLit(currChar);
        break;
      case ESC_STRING:
        state = handleEscString(currChar);
        break;
      case ESC_CHAR:
        state = handleEscChar(currChar);
        break;
    }
  }
  if(state == IN_COMMENT || state == END_COMMENT) {
    fprintf(stderr, "Error: line %d: unterminated comment\n", linestart);
    exit(1);
  }
  if(state == BEGIN_COMMENT){
    putchar('/');
  }
  return 0;
}