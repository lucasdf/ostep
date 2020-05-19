#include <errno.h>
#include <fcntl.h> // file descriptor manipulation
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern int errno;

int main() {
  // O_CREAT - creates if it does not exist
  // O_WRONLY - write access mode
  // O_TRUNC - overwrite file by truncating its length to zero
  // S_IRUSR | S_IWUSR - sets read and write access to owner

  // returns 3, since 0 is stardard input, 1 is standard output, and 2 is
  // standard error
  int fd = open("foo", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
  printf("File 'foo' open result is %d\n", fd);
  // creat is the same as open with O_CREAT|O_WRONLY|O_TRUNC
  int fd2 = creat("foo2", S_IRUSR | S_IWUSR);
  printf("File 'foo2' creat result is %d\n", fd2);
  unlink("foo");
  unlink("foo2");
  printf("Unlinked 'foo' and 'foo2'");

  // writing to stout
  char *message = "Writing to standard output using 'write'\n";
  write(1, message, strlen(message));

  int fd3 = open("thisfiledoesnotexist", O_WRONLY);
  printf("File 'thisfiledoesnotexist' open result is %d\n", fd3);
  printf("Error number while reading non-existing file: %d\n", errno);

  int fd4 = open("ch39_01_file_descriptor.c", O_RDONLY);
  char buff[8];
  read(fd4, buff, 8);
  printf("Read content from file: %s\n", buff);
  // change offset to 40 bytes before end of file
  lseek(fd4, -40, SEEK_END);
  // read eight more bytes beginning from current offset
  ssize_t read_result = read(fd4, buff, 8);
  printf("Read content from file: %s\n", buff);
  printf("Read result contains bytes read: %zd\n", read_result);

  lseek(fd4, 0, SEEK_END);
  read_result = read(fd4, buff, 4);
  printf("Read result is zero since offset is at end of file: %zd\n",
         read_result);
}
