// Test run ./threads.v0.out nmultiple times with the following arguments
// 1000/10000/100000
#include "common_threads.h"
#include <stdio.h>
#include <stdlib.h>

volatile int counter = 0;
int loops;

void *worker(void *arg) {
  int i;
  for (i = 0; i < loops; i++) {
    // INFO: This is the CRITICAL SECTION.
    // The `counter++` operation seems simple but it is not atomic.
    // It typically translates to three assembly instructions:
    // 1. mov EAX, [counter]   // Load the value of counter into a register
    // 2. inc EAX             // Increment the value in the register
    // 3. mov [counter], EAX  // Store the new value back to memory

    // A race condition occurs when one thread is preempted by the scheduler
    // between instructions. For example, Thread A loads `counter` (value is 0),
    // and then the OS switches to Thread B. Thread B also loads `counter`
    // (value is still 0), increments it, and writes the new value (1) back.
    // When Thread A resumes, it continues from where it left off, increments
    // its old value (0) to 1, and writes that back, effectively overwriting
    // Thread B's work. The final value will be less than the expected 2 *
    // loops.
    counter++;
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: threads <loops>\n");
    exit(1);
  }
  loops = atoi(argv[1]);
  pthread_t p1, p2;
  printf("Initial value : %d\n", counter);
  Pthread_create(&p1, NULL, worker, NULL);
  Pthread_create(&p2, NULL, worker, NULL);
  Pthread_join(p1, NULL);
  Pthread_join(p2, NULL);
  printf("Final value   : %d\n", counter);
  // Expected final value is 2 * loops, but due to the race condition,
  // it is often much lower, especially with a high number of loops.
  return 0;
}
