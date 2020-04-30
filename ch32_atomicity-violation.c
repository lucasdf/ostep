#include "common.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>

typedef struct __something_ {
  int info;
} something;

something some_something;

void *thread_1(void *arg) {
  if (((something *)arg)->info == 99)
    // bad. 99 may have changed to 0. this may cause crashes.
    // better use a lock before the comparison, to ensure to nasty thread
    // is gonna change this value while we read and use it
    printf("I got 99 problems but a pointer aint one\n");
  return NULL;
}

void *thread_2(void *arg) {
  ((something *)arg)->info = 0;
  return NULL;
}

int main() {
  some_something.info = 99;
  pthread_t t1, t2;
  pthread_create(&t1, NULL, thread_1, &some_something);
  pthread_create(&t2, NULL, thread_2, &some_something);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
}
