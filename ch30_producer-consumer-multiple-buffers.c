#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include "common_threads.h"

#define MAX 10
int buffer[MAX];
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;

void put(int value){
  buffer[fill_ptr] = value;
  fill_ptr = (fill_ptr + 1) % MAX;
  count++;
}

int get(){
  int tmp = buffer[use_ptr];
  use_ptr = (use_ptr + 1) % MAX;
  count--;
  return tmp;
}

int loops;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *arg) {
  int i;
  for (i = 0; i < loops; i++) {
    Pthread_mutex_lock(&mutex);
    // producer sleeps when all buckets are full
    while (count == MAX)
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
    // consumer sleeps when all buckets are empty
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
