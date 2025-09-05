// Write a program that creates two children, and connects the standard output
// of one to the standard input of the other, using the pipe() system call.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  // Print a message from the parent process with its PID
  printf("Parent: Hello, world! (pid:%d)\n", getpid());

  int pipe_fds[2]; // Array to hold the two I/O fds ends of the pipe

  if (pipe(pipe_fds) == -1) { // Create a pipe and handle any errors
    perror("pipe() failed");
    exit(EXIT_FAILURE);
  }

  int child_1 = fork(); // Fork the first child process

  if (child_1 < 0) { // Handle fork failure
    perror("fork() failed");
    exit(EXIT_FAILURE);

  } else if (child_1 == 0) { // Inside the first child process
    printf("Child 1: (pid:%d)\n", getpid());

    close(pipe_fds[0]); // Close the read end of the pipe
    dup2(pipe_fds[1], STDOUT_FILENO); // Redirect stdout to pipe's write end
    close(pipe_fds[1]); // Close the write end of the pipe after duplication
    // Print a message that will be sent through the pipe
    printf("Hello, I am Child_1!\n");
    exit(EXIT_SUCCESS);
  }

  // Fork the second child process
  int child_2 = fork();

  if (child_2 < 0) { // Handle fork failure
    perror("fork() failed");
    exit(EXIT_FAILURE);

  } else if (child_2 == 0) { // Inside the second child process
    printf("Child 2: (pid:%d)\n", getpid());

    close(pipe_fds[1]); // Close the write end of the pipe
    dup2(pipe_fds[0], STDIN_FILENO); // Redirect stdin to pipe's read end
    close(pipe_fds[0]); // Close the read end of the pipe after duplication

    char rd_buf[64]; // Buffer to store the message read from the pipe
    if (fgets(rd_buf, sizeof(rd_buf), stdin) == NULL) {
      perror("fgets() failed"); // Handle read failure
      exit(EXIT_FAILURE);
    };
    rd_buf[strcspn(rd_buf, "\n")] = '\0'; // Remove '\n' from the input

    // Print the message received from Child 1
    printf("Child_2 received: '%s'\n", rd_buf);
    exit(EXIT_SUCCESS);
  }

  // Close both ends of the pipe in the parent process
  close(pipe_fds[0]);
  close(pipe_fds[1]);

  // Wait for both child processes to finish
  pid_t out = waitpid(child_1, NULL, 0);
  pid_t in = waitpid(child_2, NULL, 0);
  // Ensure that the correct PIDs are returned after the children finish
  assert(child_1 == out);
  assert(child_2 == in);
  // Print a message indicating both children have finished
  printf("Both children have finished!\n");

  return EXIT_SUCCESS;
}

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char **argv) {

  printf("Parent: Hello, world! (pid:%d)\n", getpid());

  int pipedes[2];

  if (pipe(pipedes) == -1) {
    perror("pipe() failed");
    exit(EXIT_FAILURE);
  }

  int child_1 = fork();

  if (child_1 < 0) {
    perror("fork() failed");
    exit(EXIT_FAILURE);

  } else if (child_1 == 0) {
    printf("Child 1: (pid:%d)\n", getpid());
    close(pipedes[0]);
    char *out_str = "Hello, I am the 1st child!";
    write(pipedes[1], out_str, strlen(out_str));
    close(pipedes[1]);

    exit(EXIT_SUCCESS);
  }

  int child_2 = fork();

  if (child_2 < 0) {
    perror("fork() failed");
    exit(EXIT_FAILURE);

  } else if (child_2 == 0) {
    printf("Child 2: (pid:%d)\n", getpid());
    close(pipedes[1]);
    char buf[64];
    read(pipedes[0], buf, sizeof(buf));
    close(pipedes[0]);
    printf("Child_2 received: %.*s\n", (int)sizeof(buf), buf);

    exit(EXIT_SUCCESS);
  }

  close(pipedes[0]);
  close(pipedes[1]);

  pid_t out = waitpid(child_1, NULL, 0);
  pid_t in = waitpid(child_2, NULL, 0);

  assert(child_1 == out);
  assert(child_2 == in);

  printf("Both children have finished!\n");

  return EXIT_SUCCESS;
} */
