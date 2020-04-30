#include "common.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct __secret_ {
  char path[1000];
} secret;

void *launch_rockets(void *arg) {
  FILE *file;
  secret *mysecret = ((secret *)arg);
  file = fopen(mysecret->path, "r");
  if (!file) {
    printf("things have gone bad\n");
    exit(EXIT_FAILURE);
  }
  char *line = NULL;
  size_t size;
  getline(&line, &size, file);
  if (line) {
    printf("%s", line);
    free(line);
  }
  return NULL;
}

void *initialize_paranaues(void *arg) {
  char *cwd;
  cwd = getenv("PWD");
  secret *mysecret = ((secret *)arg);
  strcpy(mysecret->path, cwd);
  strcat(mysecret->path, "/");
  strcat(mysecret->path, __FILE__);
  return NULL;
}

int main() {
  secret mysecret;
  pthread_t t1, t2;
  pthread_create(&t2, NULL, launch_rockets, &mysecret);
  pthread_create(&t1, NULL, initialize_paranaues, &mysecret);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
}
