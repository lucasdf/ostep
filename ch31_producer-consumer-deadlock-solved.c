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
    printf("producer waiting for empty:\n");
    sem_wait(&empty);
    printf("producer acquiring lock:\n");
    sem_wait(&mutex);
    printf("producer acquired lock!\n");
    printf("producer working:\n");
    put(i);
    sem_post(&mutex);
    sem_post(&full);
  }
  printf("producer ended\n");
  return NULL;
}

void *consumer(void *arg){
  int i;
  for (i = 0; i < loops; i++){
    printf("consumer waiting for full:\n");
    sem_wait(&full);
    printf("consumer acquiring lock:\n");
    sem_wait(&mutex);
    printf("consumer acquired lock:\n");
    printf("consumer working:\n");
    int tmp = get();
    sem_post(&mutex);
    sem_post(&empty);
    printf("consumer value: %d\n", tmp);
  }
  printf("consumer ended\n");
  return NULL;
}

void wait_for_threads(pthread_t threads[]) {
  int i = 0;
  while (threads[i] != '\0') {
    Pthread_join(threads[i], NULL);
    i++;
  }
}

int main(int argc, char *argv[]){
  loops = 100;
  sem_init(&empty, 0, MAX);
  sem_init(&full, 0, 0);
  sem_init(&mutex, 0, 1);
  pthread_t c1, p1, p2;
  Pthread_create(&c1, NULL, consumer, NULL);
  Spin(1);
  Pthread_create(&p1, NULL, producer, NULL);
  Pthread_create(&p2, NULL, producer, NULL);
  pthread_t thread_pool[] = {c1, p1, p2, '\0'};
  wait_for_threads(thread_pool);
}
