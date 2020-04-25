#include <common_threads.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct __counter_t {
  int value;
  pthread_mutex_t lock;
} counter_t;

void init(counter_t *c) {
  c->value = 0;
  Mutex_init(&c->lock);
}

void increment(counter_t *c) {
  Pthread_mutex_lock(&c->lock);
  c->value++;
  Pthread_mutex_unlock(&c->lock);
}

struct IncrementByN_params {
  counter_t *c;
  int n;
};

void *increment_by_n(void *arg) {
  struct IncrementByN_params *params = arg;
  for (int i = 0; i < params->n; i++)
    increment(params->c);
  return NULL;
}

void decrement(counter_t *c) {
  Pthread_mutex_lock(&c->lock);
  c->value--;
  Pthread_mutex_unlock(&c->lock);
}

int get(counter_t *c) {
  Pthread_mutex_lock(&c->lock);
  int value = c->value;
  Pthread_mutex_unlock(&c->lock);
  return value;
}

void increment_in_thread(pthread_t *t, counter_t *c) {
  struct IncrementByN_params *params;
  params = malloc(sizeof(*params));
  params->n = 10000;
  params->c = c;
  Pthread_create(t, NULL, increment_by_n, params);
}

void wait_for_threads(pthread_t threads[]) {
  for (int i = 0; i < 3; i++) {
    Pthread_join(threads[i], NULL);
  }
}

int main() {
  counter_t *counter;
  counter = malloc(sizeof(*counter));
  init(counter);

  pthread_t t1, t2, t3;
  increment_in_thread(&t1, counter);
  increment_in_thread(&t2, counter);
  increment_in_thread(&t3, counter);
  pthread_t thread_pool[] = {t1, t2, t3};
  wait_for_threads(thread_pool);

  printf("%d", get(counter));
}
