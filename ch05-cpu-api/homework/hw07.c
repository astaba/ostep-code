// Write a program that creates a child process, and then in the child closes
// standard output (STDOUT FILENO). What happens if the child calls printf() to
// print some output after closing the descriptor?

// INFO:
// Since the descriptor is closed, there is no valid destination for the output.
// The printf() call silently fails in this case. By default, standard output
// writes fail without causing a crash.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {

  printf("Hello, World! (pid:%d)\n", getpid());

  int rc = fork();

  if (rc < 0) {
    perror("fork() failed");
    exit(EXIT_FAILURE);
  } else if (rc == 0) {
    printf("Hi, I am the child process (pid:%d)\n", getpid());

    close(STDOUT_FILENO);

    if (printf("What happen???\n") < 0)
      perror("printf() failed");

  } else {
    int status = 0;
    pid_t pid = waitpid(rc, &status, 0);
    if (pid == -1) {
      perror("waitpid() failed");
      exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status)) {
      printf("Parent: Child(pid:%d) exit status %d\n", pid,
             WEXITSTATUS(status));
    } else {
      printf("Parent: Child(pid:%d) exit status abnormal\n", pid);
    }

    assert(pid == rc);
    printf("I am the parent of child(pid:%d). (pid:%d)\n", pid, getpid());
  }

  return EXIT_SUCCESS;
}
