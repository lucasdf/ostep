#include "common.h"
#include "common_threads.h"
#include <pthread.h>
#include <stdio.h>

int done = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void thread_exit() {
  Pthread_mutex_lock(&m);
  done = 1;
  Pthread_cond_signal(&c);
  Pthread_mutex_unlock(&m);
}

void *child(void *arg) {
  printf("child: begin\n");
  Spin(14);
  thread_exit();
  printf("child: end\n");
  return NULL;
}

void thread_join() {
  Pthread_mutex_lock(&m);
  while (done == 0) {
    printf("parent: waiting\n"); 
    Pthread_cond_wait(&c, &m);
  }
  Pthread_mutex_unlock(&m);
}

int main(int argc, char *argv[]) {
  printf("parent: begin\n");
  pthread_t t;
  Pthread_create(&t, NULL, child, NULL);
  thread_join();
  printf("parent: end\n");
  return 0;
}
