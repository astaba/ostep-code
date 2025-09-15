/* ostep-code/ch05-cpu-api/p2.c */
// Listing 5.2
// The process API provides another family of function wait() and waitpid().
// In this case the wait() function forces the parent to block until the
// child process returns even though the parent was the first to get control
// after the fork() call.

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  printf("Hello World! (pid:%d)\n", getpid());

  int rc = fork();

  if (rc < 0) { // fork failed; exit
    fprintf(stderr, "fork failed.\n");
    exit(EXIT_FAILURE);

  } else if (rc == 0) { // child (new process)
    printf("Hello, I am child. (pid:%d)\n", getpid());
    sleep(1);

  } else { // parent goes down this path (original process)
    int wc = wait(NULL);
    printf("Hello, I am parent of %d (wc:%d) (pid:%d)\n", rc, wc, getpid());
  }

  printf("At end. (pid:%d)\n", getpid());
  return EXIT_SUCCESS;
}
