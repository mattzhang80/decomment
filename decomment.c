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
  } else {
    putchar('/');
    putchar(currChar);
    state = START;
  }
  return state;
}

enum State handleInComment(int currChar) {
  printf("in comment");
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
  if (currChar == '/')
    state = START;
  else if (currChar == '*')
    state = END_COMMENT;
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
  if (currChar == '/') {
    return BEGIN_COMMENT;
  }
  enum State state;
  state = ESC_STRING;
  putchar(currChar);
  return state;
}

enum State handleEscChar(int currChar) {
  if(currChar == '/') {
    return BEGIN_COMMENT;
    enum State state;
    state = ESC_CHAR;
    putchar(currChar);
    return state;
  } 
}

int main(int argc, char *argv[]) {
  enum State state;
  int currChar;
  state = START;
  while ((currChar = getchar()) != EOF) {
    switch (state) {
      case START:
        state = handleStart(currChar);
        break;
      case BEGIN_COMMENT:
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
      default:
        printf("Invalid state %d\n", state);
        exit(1);
    }
  }
  return 0;
}