/* p4.c */
// Using the process API to simulate the exact way shell redirection works.

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(void) {

  printf("At start. pid:%d\n", getpid());
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "fork() failed.\n");
    exit(EXIT_FAILURE);

  } else if (rc == 0) {
  printf("Child. pid:%d\n", getpid());
    close(STDOUT_FILENO);
    open("p4_output.txt", O_WRONLY | O_CREAT | O_TRUNC,
         S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    char *myargs[3];
    myargs[0] = strdup("wc");
    myargs[1] = strdup("./p4.c");
    myargs[2] = NULL;
    execvp(myargs[0], myargs);

  } else {
    int wc = wait(NULL);
    assert(wc >= 0);
  }

  printf("At end. pid:%d\n", getpid());
  return EXIT_SUCCESS;
}
