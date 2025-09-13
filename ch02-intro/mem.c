/* WARN: For this example to work, you need to make sure address-space
 * randomization is disabled; randomization, as it turns out, can be a good
 * defense against certain kinds of security flaws. Read more about it on your
 * own, especially if you want to learn how to break into computer systems via
 * stack-smashing attacks. Not that we would recommend such a thing...
 * Example prompt:
 * The -R option disables address space randomization for the execution of your
 * program
 *
 * $ for arg in 100 200; do setarch $(uname -m) -R ./mem.out $arg & done
 *
 * */

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // The program expects exactly one argument after text invocation.
  if (argc != 2) {
    fprintf(stderr, "usage: mem <value>\n");
    exit(1);
  }

  // An int pointer to store the address of the newly allocated memory on the
  // heap.
  int *p;

  // Allocate memory for a single integer on the heap. The `sizeof(int)` ensures
  // we get the correct amount of memory (4 bytes on most systems).
  p = malloc(sizeof(int)); // a1: memory is allocated on the heap

  // `assert` terminates program if memory allocation was unsuccessful.
  assert(p != NULL);

  // Get the process ID (PID) and print the memory address stored in p.
  // This address is on the heap, and will be the same across multiple processes
  // if ASLR is disabled.
  printf("(%d) addr pointed to by p: %p\n", (int)getpid(), p); // a2

  // Convert to 4-bytes integer the numeric part in the ASCII string pointed to
  // by argv[1] and store it on the heap.
  *p = atoi(argv[1]); // a3: copy value from stack (argv[1]) to heap (*p)

  // Start an infinite loop to continuously increment the value.
  while (1) {
    Spin(1);
    *p = *p + 1;
    // Print the PID and the new value of the integer on the heap.
    printf("(%d) value of p: %d\n", getpid(), *p); // a4
  }

  // This line is unreachable due to the infinite loop.
  return 0;
}
