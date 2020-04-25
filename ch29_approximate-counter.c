#include "common_threads.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>

#define NUM_CPUS 4

typedef struct __counter_t {
  int global;
  pthread_mutex_t global_lock;
  int local_locks[NUM_CPUS];
  pthread_mutex_t local_lock[NUM_CPUS];
  int update_frequency;
} counter_t;

void init(counter_t *c, int update_frequency)
{
  c->update_frequency = update_frequency;
  c->global = 0;
  pthread_mutex_init(&c->global_lock, NULL);
  int i;
  for (i = 0; i < NUM_CPUS; i++) {
    c->local_locks[i] = 0;
    pthread_mutex_init(&c->local_lock[i], NULL);
  }
}

void update(counter_t *c, int thread_id, int local_value) {
  int cpu = thread_id % NUM_CPUS;
  pthread_mutex_lock(&c->local_lock[cpu]);
  c->local_locks[cpu] += local_value;
  if (c->local_locks[cpu] >= c->update_frequency) {
    pthread_mutex_lock(&c->global_lock);
    c->global += c->local_locks[cpu];
    pthread_mutex_unlock(&c->global_lock);
    c->local_locks[cpu] = 0;
  }
  pthread_mutex_unlock(&c->local_lock[cpu]);
}

int get(counter_t *c) {
  pthread_mutex_lock(&c->global_lock);
  int val = c->global;
  pthread_mutex_unlock(&c->global_lock);
  return val; // approximate value
}

int main(){}
