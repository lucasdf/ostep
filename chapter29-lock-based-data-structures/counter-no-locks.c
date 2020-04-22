#include <pthread.h>
#include <stdlib.h>
#include <common_threads.h>
#include <stdio.h>

typedef struct __counter_t {
  int value;
} counter_t;

void init(counter_t *c) {
  c->value = 0;
}

void *increment(void *arg) {
  counter_t *c = arg;
  c->value++;
  return NULL;
}

struct IncrementByN_params {
  counter_t* c;
  int n;
};

void *increment_by_n (void *arg) {
  struct IncrementByN_params *params = arg;
  for (int i = 0; i < params->n; i++)
    params->c->value++;
  return NULL;
}

void decrement(counter_t *c) {
  c->value--;
}

int get(counter_t *c) {
  return c->value;
}

void increment_in_thread(pthread_t *t, counter_t *c) {
  struct IncrementByN_params *params;
  params = malloc(sizeof(*params));
  params->n = 10000;
  params->c = c;
  Pthread_create(t, NULL, increment_by_n, params);
}

void wait_for_threads(pthread_t threads[]) {
  for (int i = 0; i < sizeof(threads) / sizeof(threads[0]); i++) {
    Pthread_join(threads[i], NULL);
  }
}

int main()
{
  counter_t *counter;
  counter = malloc(sizeof(*counter));

  pthread_t t1, t2, t3;
  increment_in_thread(&t1, counter);
  increment_in_thread(&t2, counter);
  increment_in_thread(&t3, counter);
  pthread_t thread_pool[] = {t1, t2, t3};
  wait_for_threads(thread_pool);

  printf("%d", counter->value);
}
