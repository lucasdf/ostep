#include "common.h"
#include "common_threads.h"
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void thread_exit() {
  Pthread_mutex_lock(&m);
  Pthread_cond_signal(&c);
  Pthread_mutex_unlock(&m);
}

void *child(void *arg) {
  printf("child: begin\n");
  thread_exit();
  printf("child: end\n");
  return NULL;
}

void thread_join() {
  Pthread_mutex_lock(&m);
  Pthread_cond_wait(&c, &m);
  Pthread_mutex_unlock(&m);
}

int main(int argc, char *argv[]) {
  printf("parent: begin\n");
  pthread_t t;
  Pthread_create(&t, NULL, child, NULL);
  // Spinning here simulates a race condition where the child signals exit,
  // but the parent has not started waiting yet. In this case, it will proceed
  // to wait (thread_join), but it will never wake up again since the child
  // thread has already exit.
  Spin(3);
  thread_join();
  printf("parent: end\n");
  return 0;
}
