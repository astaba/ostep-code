// Write a slight modification of the previous program, this time using
// waitpid() instead of wait(). When would waitpid() be useful?

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
    printf("Hi, I am the child process (pid:%d)\n", getpid());
    for (int i = 0; i < 3; i++) {
      printf("Let's sing again...\n");
      usleep(200000);
    }
    printf("Chid process executing.\n");

  } else {
    int status = 0;
    pid_t pid = waitpid(rc, &status, 0);

    if (pid == -1) {
      perror("waitpid() failed");
      exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status)) {
      printf("Parent: Child %d exit status: %d\n", pid, WEXITSTATUS(status));
    } else {
      printf("Parent: Child %d exit status: abnormal\n", pid);
    }

    assert(rc == pid);
    printf("I am the parent of child:%d (pid:%d)\n", pid, getpid());
  }

  return EXIT_SUCCESS;
}
