#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include "common.h"
#include "common_threads.h"
#include <stdlib.h>

static volatile int counter = 0;

struct ThreadParams {
  pthread_mutex_t* lock;
  char* threadIdentifier;
};

void *mythread(void *arg) {
  struct ThreadParams *params = arg;
  pthread_mutex_lock(params->lock);
  printf("%s: begin\n", params->threadIdentifier);
  int i;
  for (i = 0; i < 1e7; i++) {
    counter = counter + 1;
  }
  printf("%s: done\n", params->threadIdentifier);
  pthread_mutex_unlock(params->lock);
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_t p1, p2;
  printf("main: begin (counter = %d)\n", counter);

  struct ThreadParams *params1;
  params1 = malloc(sizeof(*params1));
  params1->lock = &lock; 
  params1->threadIdentifier = "A";
  Pthread_create(&p1, NULL, mythread, params1);

  struct ThreadParams *params2;
  params2 = malloc(sizeof(*params2));
  params2->lock = &lock; 
  params2->threadIdentifier = "A";
  Pthread_create(&p2, NULL, mythread, params2);

  Pthread_join(p1, NULL);
  Pthread_join(p2, NULL);
  printf("main: done with both (counter = %d)\n", counter);
  return 0;
}
