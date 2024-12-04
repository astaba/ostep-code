/* WARN: For this example to work, you need to make sure address-space
 * randomization is disabled; randomization, as it turns out, can be a good
 * defense against certain kinds of security flaws. Read more about it on your
 * own, especially if you want to learn how to break into computer systems via
 * stack-smashing attacks. Not that we would recommend such a thing...
 * Example prompt:
 * The -R option disables address space randomization for the execution of your
 * program
 *
 * $ setarch $(uname -m) -R ./bin/mem <value> & \
 * > setarch $(uname -m) -R ./bin/mem <value> &
 * */

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: mem <value>\n");
    exit(1);
  }
  int *p;
  p = malloc(sizeof(int)); // a1
  assert(p != NULL);
  printf("(%d) addr pointed to by p: %p\n", (int)getpid(), p); // a2
  *p = atoi(argv[1]); // a3 assign value to addr stored in p
  while (1) {
    Spin(1);
    *p = *p + 1;
    printf("(%d) value of p: %d\n", getpid(), *p); // a4
  }
  return 0;
}
