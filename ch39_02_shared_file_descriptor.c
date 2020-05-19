#include <assert.h>
#include <errno.h>
#include <fcntl.h> // file descriptor manipulation
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

extern int errno;

int main() {
  int fd = open("ch39_02_shared_file_descriptor.c", O_RDONLY);
  printf("starting offset %d\n", (int)lseek(fd, 0, SEEK_CUR));
  assert(fd >= 0);
  int rc = fork();
  if (rc == 0) {
    rc = lseek(fd, 10, SEEK_SET);
    printf("child: offset %d\n", rc);
  } else {
    // waits for child to exit
    (void)wait(NULL);
    printf("parent: offset %d\n", (int)lseek(fd, 0, SEEK_CUR));
  }
  return 0;
}
