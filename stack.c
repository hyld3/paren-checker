#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

static unsigned long * stack_ptr = NULL;
static unsigned long * stack_start = NULL;

inline static char peek() {
  return *(stack_ptr-1);
}

inline static char pop() {
  char c = peek();
  *stack_ptr-- = '\0';
  return c;
}

inline static void push(char c) {
  *stack_ptr++ = c;
}

int main(int argc, char * argv[]) {

  bool unbalanced = false;
  stack_ptr = mmap(0, (0x1000 * 0x1000), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
  stack_ptr +=(((0x1000 * 0x1000)/2)/0x8);
  stack_start = stack_ptr;

  char c;
  while ((c = getchar()) != EOF) {
    if (unbalanced) 
      break;

    if (c == '(' || c == '{' || c == '[')
      push(c);
    else if (c == ')') 
      if (stack_ptr == stack_start)
	unbalanced = true;
      else if (peek() == '(')
	pop();
      else 
      unbalanced = true;
    else if (c == '}') 
      if (stack_ptr == stack_start) 
	unbalanced = true;
      else if (peek() == '{') 
	pop();
      else  
      unbalanced = true;
    else if (c == ']') {
      if (stack_ptr == stack_start)
	unbalanced = true;
      else if (peek() == '[')
	pop();
    else 
      unbalanced = true;
    }
  }

  ((stack_ptr == stack_start) && !unbalanced) ?
    write(1, "Balanced\n", 9) :
    write(1, "Unbalanced\n", 11);

  return 0;
}
