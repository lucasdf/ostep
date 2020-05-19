#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  char *dir_path = argv[argc - 1];
  printf("Dir path is %s\n", dir_path);
  DIR *dp = opendir(dir_path);
  assert(dp != NULL);
  struct dirent *d;
  while ((d = readdir(dp)) != NULL) {
    printf("%lu %s\n", (unsigned long)d->d_ino, d->d_name);
  }
  closedir(dp);
  return 0;
}
