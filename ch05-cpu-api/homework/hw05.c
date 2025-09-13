// Now write a program that uses wait() to wait for the child process to finish
// in the parent. What does wait() return? What happens if you use wait() in the
// child?

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {

  printf("Hello, world! (pid:%d)\n", getpid());

  int rc = fork();

  if (rc < 0) {
    perror("fork() failed");
    exit(EXIT_FAILURE);

  } else if (rc == 0) {
    // Try calling wait() in the child process
    int status = wait(NULL);
    if (status == -1)
      perror("Child wait() failed");

    printf("Hi, I am the child process (pid:%d)\n", getpid());
    for (int i = 0; i < 3; i++) {
      printf("Let's sing again...\n");
      usleep(200000);
    }
  } else {
    int sing = wait(NULL);
    assert(sing == rc);
    printf("I am the parent of child:%d (pid:%d)\n", sing, getpid());
  }

  return EXIT_SUCCESS;
}
