
#include "common.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
sem_t sem;

void *child(void *arg) {
  printf("child waits 2 seconds\n");
  Spin(2);
  sem_post(&sem);
  return NULL;
}

int main(int argc, char *argv[]){
  // initial value is zero, otherwise if it is 1 then the sem_wait below
  // would decrement it and proceed.
  sem_init(&sem, 0, 0);
  printf("parent started\n");
  pthread_t t1;
  pthread_create(&t1, NULL, child, "A");
  sem_wait(&sem);
  printf("parent end\n");
}
