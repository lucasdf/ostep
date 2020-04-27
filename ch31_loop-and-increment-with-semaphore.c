#include <semaphore.h>
#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include "common.h"
#include "common_threads.h"

static volatile int counter = 0;
static sem_t lock;

void *mythread(void *arg) {
  sem_wait(&lock);
  printf("%s: begin\n", (char *) arg);
  int i;
  for (i = 0; i < 1e7; i++) {
    counter = counter + 1;
  }
  printf("%s: done\n", (char *) arg);
  sem_post(&lock);
  return NULL;
}

int main(int argc, char *argv[]) {
  sem_init(&lock, 0, 1);
  pthread_t p1, p2;
  printf("main: begin (counter = %d)\n", counter);
  Pthread_create(&p1, NULL, mythread, "A");
  Pthread_create(&p2, NULL, mythread, "B");
  Pthread_join(p1, NULL);
  Pthread_join(p2, NULL);
  printf("main: done with both (counter = %d)\n", counter);
  return 0;
}
