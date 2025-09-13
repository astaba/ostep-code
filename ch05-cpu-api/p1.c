/* ostep-code/cpu-api/p1.c */
// Listing 5.1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  printf("Hello world. (pid: %d)\n", getpid());

  int rc = fork();

  if (rc < 0) { // fork filed; exit
    fprintf(stderr, "fork failed\n");
    exit(EXIT_FAILURE);

  } else if (rc == 0) { // child process
    printf("Hello, I am child. (pid %d)\n", getpid());

  } else { // parent goes down this path (original process)
    printf("Hello, I am parent of %d. (pid %d)\n", rc, getpid());
  }

  printf("At end. pid:%d\n", getpid());
  return EXIT_SUCCESS;
}
