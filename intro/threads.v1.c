// This version fix race conditions in the assembly loop housekeeping.
// Test run: ./threads.v1.out 100000
#include "common_threads.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Declare a mutex variable. Mutexes are used to protect shared data.
pthread_mutex_t m;
volatile int counter = 0;
int loops;

void *worker(void *arg) {
  int i;
  for (i = 0; i < loops; i++) {
    // Acquire the lock for the mutex 'm'.
    // If another thread already holds the lock, this call will block
    // until the lock is released. This ensures that only one thread
    // can enter the critical section at a time.
    Pthread_mutex_lock(&m);
    // This is the CRITICAL SECTION.
    // Only one thread can execute this code block at any given time.
    counter++;
    // Release the lock for the mutex 'm'.
    // This allows another blocked thread to acquire the lock and enter
    // the critical section.
    Pthread_mutex_unlock(&m);
  }
  pthread_exit(NULL);
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
  // With the mutex, the final value will consistently equal 2 * loops,
  // because the race condition has been eliminated.
  return 0;
}
