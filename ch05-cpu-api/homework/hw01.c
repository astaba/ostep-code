// Write a program that calls fork(). Before calling fork(), have the main
// process access a variable (e.g., x) and set its value to something (e.g.,
// 100). What value is the variable in the child process? What happens to the
// variable when both the child and parent change the value of x?

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  int x = 100;
  printf("Main process (pid:%d): x = %d\n", getpid(), x);

  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "fork() failed.\n");
    exit(EXIT_FAILURE);

  } else if (rc == 0) {
    printf("Child process (pid:%d): x = %d\n", getpid(), x);
    printf("Child process changing x value.\n");
    x = 99;

  } else {
    printf("Main process parent of (child:%d) (pid:%d): x = %d\n", rc, getpid(),
           x);
    printf("Main process changing x value.\n");
    x = 111;
  }

  printf("End (pid:%d): x = %d\n", getpid(), x);
  return EXIT_SUCCESS;
}
