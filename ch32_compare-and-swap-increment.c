#include "common.h"
#include "common_threads.h"
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>

static volatile int counter = 0;

void atomic_increment(int amount) {
  int old = counter;
  do {
    old = counter;
    // gcc builtin cas - it is a single atomic instruction
    // if value in counter is old, then change it to old+amount
  } while (__sync_bool_compare_and_swap(&counter, old, old + amount) == 0);
}

void *mythread(void *arg) {
  printf("%s: begin\n", (char *)arg);
  int i;
  for (i = 0; i < 1e7; i++) {
    atomic_increment(1);
  }
  printf("%s: done\n", (char *)arg);
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t p1, p2, p3, p4;
  printf("main: begin (counter = %d)\n", counter);
  Pthread_create(&p1, NULL, mythread, "A");
  Pthread_create(&p2, NULL, mythread, "B");
  Pthread_create(&p3, NULL, mythread, "C");
  Pthread_create(&p4, NULL, mythread, "D");
  Pthread_join(p1, NULL);
  Pthread_join(p2, NULL);
  Pthread_join(p3, NULL);
  Pthread_join(p4, NULL);
  printf("main: done with both (counter = %d)\n", counter);
  return 0;
}
