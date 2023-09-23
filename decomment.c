#include <stdio.h>
#include <stdlib.h>
/* State enum to track DFA states. Consists of an intial start state, 
a begin comment state, an in comment state, an end comment state, an in string literal state, 
an in character literal state, an escape string state, and an escape character state.
*/
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

/* handleStart input when in the START state. Takes the current character from stdin and 
checks for the beginning of a comment, string literal, character literal, or newline character
based on their respective initial characters. This is the starting state of the DFA*/
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
/* handleCommentStart handles input when in the BEGIN_COMMENT state, which is the state that 
occurs after a '/' character is read. Check if input is in a comment, still beginning of comment, 
in a character literal, string literal, or none of above, based on respective characters*/
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
/* handleInComment handles input when in the IN_COMMENT state, which is the state that occurs after a '/*' is read.
In this function, it checks if it is reaching the end of comment, a newline, or still in a comment. */

enum State handleInComment(int currChar) {
  enum State state;
  state = IN_COMMENT;
  if(currChar == '\n')
    putchar('\n');
  else if (currChar == '*')
    state = END_COMMENT;
  return state; 
}
/* handleEndComment handles input when in the END_COMMENT state, which is the state that occurs after 
input is in a comment and the current character is a *. In this function, it verifies for end of comment, 
checks for newline, or if it is still in a comment.*/
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
/* handleInStringLit handles input when in the IN_STRING_LIT state. This occurs when input reads a ".
This function checks for end of string literal or escape character, or if it is still in a string literal. */
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
/* handleInCharLit handles input when in the IN_CHAR_LIT state, which occurs after a '. 
It checks for the end of character literal or escape character, or if it is still in a literal. */
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
/* handleEscString handles input when in the ESC_STRING state, when input reads a \ in the string literal.
It simply puts the current character to the output and goes back to the string literal.*/

enum State handleEscString(int currChar) {
  putchar(currChar);
  return IN_STRING_LIT;
}
/* handleEscChar handles input when in the ESC_CHAR state, when input reads a \ in the character literal. 
It simply puts the current character to the output and goes back to the character literal.*/
enum State handleEscChar(int currChar) {
  putchar(currChar);
  return IN_CHAR_LIT;
}
/* Read a character from stdin until everything is read. While tracking the current line number, 
begin in the START state and switch to the appropriate state based on the current character.
Removes comments from stdin and prints the remaining result to stdout.
Return EXIT_SUCCESS if the program runs successfully, and EXIT_FAILURE if it does not.
Does not run successfully if there are unterminated comments.
*/
int main() {
  // Initialize variables.
  enum State state;
  int currChar;
  int currline = 1;
  int linestart = 0;
  state = START;
  /* Loop through DFA using input. */
  while ((currChar = getchar()) != EOF) {
    // Tracks line number
    if(currChar == '\n'){
      currline++;
    }
    /* Switch statement to handle the DFA states. */
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
  /* Check for unterminated comment. */
  if(state == IN_COMMENT || state == END_COMMENT) {
    fprintf(stderr, "Error: line %d: unterminated comment\n", linestart);
    return EXIT_FAILURE;
  }
  /* Check for unterminated string literal. */
  if(state == BEGIN_COMMENT){
    putchar('/');
  }
  return EXIT_SUCCESS;
}