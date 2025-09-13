#ifndef __common_h__
#define __common_h__

#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h> // Required for the gettimeofday() function

// GetTime()
// Returns the current system time as a high-precision double.
// This is used for timing events with microsecond accuracy.
static double GetTime() {
  // Declare a structure to hold the time value.
  // 'tv_sec' holds seconds, 'tv_usec' holds microseconds.
  struct timeval t;
  // Call the gettimeofday() system call to get the current time.
  // The first argument is the address of the timeval struct to fill.
  // The second argument (timezone) is NULL as it's not needed.
  int rc = gettimeofday(&t, NULL);
  // Assert that the system call was successful. If rc is not 0,
  // the program will terminate with an error message.
  assert(rc == 0);
  // Convert the seconds and microseconds into a single floating-point number.
  // We divide tv_usec by 1,000,000 to convert it from microseconds to seconds.
  return (double)t.tv_sec + (double)t.tv_usec / 1e6;
}

// Spin()
// This function creates a busy-wait loop that consumes CPU cycles
// for a specified number of seconds. It's a simple but inefficient
// way to create a delay.
// @param howlong: The number of seconds to wait.
static void Spin(int howlong) {
  // Get the starting time with high precision.
  double t = GetTime();
  // Start a busy-wait loop. The loop's condition is constantly checked.
  // It compares the elapsed time (current time - start time) with 'howlong'.
  // The empty statement ';' means the loop does nothing
  // except check the condition and consume CPU time.
  // This is known as "busy waiting" or "polling".
  while ((GetTime() - t) < (double)howlong)
    ;
}

#endif // __common_h__
