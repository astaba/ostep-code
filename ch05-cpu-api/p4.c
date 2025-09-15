/* ostep-code/ch05-cpu-api/p4.c */
// Listing 5.4
// Using the process API to simulate the exact way shell redirection works.
// NOTE: The fork() system call is strange; its partner in crime, exec(), is not
// so normal either. What it does: given the name of an executable (e.g., wc),
// and some arguments (e.g., p3.c), it loads code (and static data) from that
// executable and overwrites its current code segment (and current static data)
// with it; the heap and stack and other parts of the memory space of the
// program are re-initialized. Then the OS simply runs that program, passing in
// any arguments as the argv of that process. Thus, it does not create a new
// process; rather, it transforms the currently running program (formerly p3)
// into a different running program (wc). After the exec() in the child, it is
// almost as if p3.c never ran; a successful call to exec() never returns.

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {

  printf("At start. pid:%d\n", getpid());
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "fork() failed.\n");
    exit(EXIT_FAILURE);

  } else if (rc == 0) {
    printf("Child. pid:%d\n", getpid());
    // Set up stream file Redirection --------------------------------------
    close(STDOUT_FILENO);
    open("p4_output.txt", O_WRONLY | O_CREAT | O_TRUNC,
         S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    // Run command which output is to redirect ------------------------------
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
