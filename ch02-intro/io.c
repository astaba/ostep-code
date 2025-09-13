// To fathom all the background tasks to run the program use:
// strace ./io.out
// to show system calls along with arguments and return values
// ltrace ./io.out
// to show library calls

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void dowork(void) {
  int fd = open("./tmp/file", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  assert(fd >= 0);
  char buffer[0x10];
  sprintf(buffer, "hello world\n");
  int rc = write(fd, buffer, strlen(buffer));
  assert(rc == (strlen(buffer)));
  printf("Wrote %d bytes\n", rc);
  fsync(fd);
  close(fd);
}

int main(int argc, char *argv[]) {
  dowork();
  return 0;
}
