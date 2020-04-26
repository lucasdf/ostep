#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include "common_threads.h"

int buffer;
int count = 0;

void put(int value){
  assert(count == 0);
  count = 1;
  buffer = value;
}

int get(){
  assert(count == 1);
  count = 0;
  return buffer;
}

int loops;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *arg) {
  int i;
  for (i = 0; i < loops; i++) {
    Pthread_mutex_lock(&mutex);
    while (count == 1)
      Pthread_cond_wait(&empty, &mutex);
    put(i);
    Pthread_cond_signal(&full);
    Pthread_mutex_unlock(&mutex);
  }
  printf("producer ended");
  return NULL;
}

void *consumer(void *arg){
  int i;
  for (i = 0; i < loops; i++){
    Pthread_mutex_lock(&mutex);
    while (count == 0)
      Pthread_cond_wait(&full, &mutex);
    int tmp = get();
    Pthread_cond_signal(&empty);
    Pthread_mutex_unlock(&mutex);
    printf("%d\n", tmp);
  }
  printf("consumer ended");
  return NULL;
}

void wait_for_threads(pthread_t threads[]) {
  for (int i = 0; i < 3; i++) {
    Pthread_join(threads[i], NULL);
  }
}

int main(int argc, char *argv[]){
  loops = 100;
  pthread_t c1, c2, p1;
  Pthread_create(&c1, NULL, consumer, NULL);
  Pthread_create(&p1, NULL, producer, NULL);
  Pthread_create(&c2, NULL, consumer, NULL);
  pthread_t thread_pool[] = {c1, c2, p1};
  wait_for_threads(thread_pool);
}
