/* ostep-code/ch05-cpu-api/p3.c */
// Listing 5.3
// NOTE: A final and important piece of the process creation API is the exec()
// system call3. This system call is useful when you want to run a program that
// is different from the calling program. For example, calling fork() in p2.c is
// only useful if you want to keep running copies of the same program. However,
// often you want to run a different program; exec() does just that

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  printf("Hello, World! (pid:%d)\n", getpid());

  int rc = fork();

  if (rc < 0) { // fork failed; exit;
    fprintf(stderr, "fork failed.\n");
    exit(EXIT_FAILURE);

  } else if (rc == 0) { // child (new process)
    printf("Hello, I am child. (pid:%d)\n", getpid());
    char *myargs[3];
    myargs[0] = strdup("wc");   // program to run on the prompt
    myargs[1] = strdup("p3.c"); // program arguments
    myargs[2] = NULL;           // NULL pointer required by execvp
    execvp(myargs[0], myargs);  // run the program
    // execvp does not return to the calling program. Instead the new program
    // takes over the process. As a result, next line is never executed.
    printf("This line should never run.\n");

  } else { // parent goes down this path (original process)
    int wc = wait(NULL);
    printf("Hello, I am the parent of %d. (wc:%d) (pid:%d)\n", rc, wc,
           getpid());
  }

  printf("At end. pid:%d\n", getpid());
  return EXIT_SUCCESS;
}
