
#include "common.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
sem_t sem;

void *decrement(void *arg){
  int sem_val;
  sem_getvalue(&sem, &sem_val);
  printf("Thread %s - semaphore value before wait: %d\n", (char *) arg, sem_val);
  sem_wait(&sem);
  sem_getvalue(&sem, &sem_val);
  printf("Thread %s - semaphore value after wait: %d\n", (char *) arg, sem_val);
  return NULL;
}

void *increment(void *arg){
  int sem_val;
  sem_getvalue(&sem, &sem_val);
  printf("Thread %s - semaphore value before post: %d\n", (char *) arg, sem_val);
  sem_post(&sem);
  sem_getvalue(&sem, &sem_val);
  printf("Thread %s - semaphore value after post: %d\n", (char *) arg, sem_val);
  return NULL;
}

int main(int argc, char *argv[]){
  sem_init(&sem, 0, 1);
  int sem_val;
  sem_getvalue(&sem, &sem_val);
  printf("Semaphore initial value: %d\n", sem_val);
  pthread_t t1, t2, t3, t4, t5, t6, t7, t8;
  pthread_create(&t1, NULL, decrement, "A");
  Spin(2);
  pthread_create(&t2, NULL, increment, "B");
  Spin(2);
  pthread_create(&t3, NULL, increment, "C");
  Spin(2);
  pthread_create(&t4, NULL, increment, "D");
  Spin(2);
  pthread_create(&t5, NULL, decrement, "E");
  Spin(2);
  pthread_create(&t6, NULL, decrement, "F");
  Spin(2);
  pthread_create(&t7, NULL, decrement, "G");
  Spin(2);
  pthread_create(&t8, NULL, decrement, "H");
  Spin(2);
}
