#include "common.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct __secret_ {
  char path[1000];
  pthread_mutex_t lock;
  sem_t ready;
} secret;

void *launch_rockets(void *arg) {
  FILE *file;
  secret *mysecret = ((secret *)arg);
  pthread_mutex_lock(&mysecret->lock);
  sem_wait(&mysecret->ready);
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
  pthread_mutex_unlock(&mysecret->lock);
  return NULL;
}

void *initialize_paranaues(void *arg) {
  char *cwd;
  cwd = getenv("PWD");
  secret *mysecret = ((secret *)arg);
  strcpy(mysecret->path, cwd);
  strcat(mysecret->path, "/");
  strcat(mysecret->path, __FILE__);
  sem_post(&mysecret->ready);
  return NULL;
}

secret initialize_secret() {
  secret mysecret;
  pthread_mutex_init(&mysecret.lock, NULL);
  sem_init(&mysecret.ready, 0, 0);
  return mysecret;
}

int main() {
  secret mysecret = initialize_secret();
  pthread_t t1, t2;
  pthread_create(&t2, NULL, launch_rockets, &mysecret);
  Spin(1);
  pthread_create(&t1, NULL, initialize_paranaues, &mysecret);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
}
