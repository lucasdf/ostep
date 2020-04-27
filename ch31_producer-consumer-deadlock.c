#include <assert.h>
#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include "common.h"
#include "common_threads.h"

int MAX = 2;
int buffer[2];
int fill = 0;
int use = 0;

void put(int value){
  buffer[fill] = value;
  fill = (fill + 1) % MAX;
}

int get(){
  int tmp = buffer[use];
  use = (use + 1) % MAX;
  return tmp;
}

int loops;
sem_t empty;
sem_t full;
sem_t mutex;

void *producer(void *arg) {
  int i;
  for (i = 0; i < loops; i++) {
    printf("producer acquiring lock:\n");
    sem_wait(&mutex); // if > 0, decreases and proceed, otherwise wait
    printf("producer acquired lock!\n");
    printf("producer waiting for empty:\n");
    sem_wait(&empty); // if > 0, decreases and proceed. otherwise wait
    printf("producer working:\n");
    put(i);
    sem_post(&full); // increases full +1
    sem_post(&mutex); // increases full +1
  }
  printf("producer ended");
  return NULL;
}

void *consumer(void *arg){
  int i;
  for (i = 0; i < loops; i++){
    printf("consumer acquiring lock:\n");
    sem_wait(&mutex);
    printf("consumer acquired lock:\n");
    printf("consumer waiting for full:\n");
    sem_wait(&full);
    printf("consumer working:\n");
    int tmp = get();
    sem_post(&empty);
    sem_post(&mutex);
    printf("consumer value: %d\n", tmp);
  }
  printf("consumer ended");
  return NULL;
}

void wait_for_threads(pthread_t threads[]) {
  for (int i = 0; i < 2; i++) {
    Pthread_join(threads[i], NULL);
  }
}

// in this deadlock scenario, the consumer acquired the mutex by decreasing
// its semaphore values from 1 to 0. Then, it waits for full to be > 0.
// After that, the producer starts. It tries to acquire the lock,
// but the lock is already held by the consumer.
// DEADLOCK!
int main(int argc, char *argv[]){
  loops = 100;
  sem_init(&empty, 0, MAX);
  sem_init(&full, 0, 0);
  sem_init(&mutex, 0, 1);
  pthread_t c1, p1;
  Pthread_create(&c1, NULL, consumer, NULL);
  Spin(2);
  Pthread_create(&p1, NULL, producer, NULL);
  pthread_t thread_pool[] = {c1, p1};
  wait_for_threads(thread_pool);
}
