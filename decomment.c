#include <stdio.h>
#include <stdlib.h>
// This program removes comments from a C program.
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

// handleStart input when in the START state.
enum State handleStart(int currChar) {
  enum State state;
  state = START;
  // Check for beginning of comment, string literal, character literal, or newline.
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
// handleCommentStart handles input when in the BEGIN_COMMENT state.
enum State handleCommentStart(int currChar) {
  enum State state;
  state = BEGIN_COMMENT;
  // Check for in a comment, beginning of comment, character literal, string literal, or n/a.
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
// handleInComment handles input when in the IN_COMMENT state.
enum State handleInComment(int currChar) {
  enum State state;
  state = IN_COMMENT;
  // Check for end of comment or newline.
  if(currChar == '\n')
    putchar('\n');
  else if (currChar == '*')
    state = END_COMMENT;
  return state; 
}
// handleEndComment handles input when in the END_COMMENT state.
enum State handleEndComment(int currChar) {
  enum State state;
  // Check for end of comment, newline, or still in comment.
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
// handleInStringLit handles input when in the IN_STRING_LIT state.
enum State handleInStringLit(int currChar) {
  enum State state;
  state = IN_STRING_LIT;
  // Check for end of string literal or escape character.
  if (currChar == '"')
    state = START;
  else if (currChar == '\\')
    state = ESC_STRING;
  putchar(currChar);
  return state;
}
// handleInCharLit handles input when in the IN_CHAR_LIT state.
enum State handleInCharLit(int currChar) {
  enum State state;
  state = IN_CHAR_LIT;
  // Check for end of character literal or escape character.
  if (currChar == '\'')
    state = START;
  else if (currChar == '\\')
    state = ESC_CHAR;
  putchar(currChar);
  return state;
}
// handleEscString handles input when in the ESC_STRING state.
enum State handleEscString(int currChar) {
  putchar(currChar);
  return IN_STRING_LIT;
}
// handleEscChar handles input when in the ESC_CHAR state.
enum State handleEscChar(int currChar) {
  putchar(currChar);
  return IN_CHAR_LIT;
}
// main handles int, beginning in the START state.
int main() {
  // Initialize variables.
  enum State state;
  int currChar;
  int currline = 1;
  int linestart = 0;
  state = START;
  // Loop through DFA using input.
  while ((currChar = getchar()) != EOF) {
    // Track line number
    if(currChar == '\n'){
      currline++;
    }
    // Switch statement to handle the DFA.
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
  // Check for unterminated comment.
  if(state == IN_COMMENT || state == END_COMMENT) {
    fprintf(stderr, "Error: line %d: unterminated comment\n", linestart);
    return EXIT_FAILURE;
  }
  // Check for unterminated string literal.
  if(state == BEGIN_COMMENT){
    putchar('/');
  }
  return EXIT_SUCCESS;
}