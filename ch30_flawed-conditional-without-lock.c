#include "common.h"
#include "common_threads.h"
#include <pthread.h>
#include <stdio.h>

int done = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void thread_exit() {
  done = 1;
  Pthread_cond_signal(&c);
}

void *child(void *arg) {
  printf("child: begin\n");
  thread_exit();
  printf("child: end\n");
  return NULL;
}

void thread_join() {
  while (done == 0) {
    // This simulates a race condition where the child thread is already running.
    // Since no lock is being used, the child runs, it signals and it exit.
    // Then, the parent wakes up and goes to sleep, waiting for a signal
    // that will never come :sadpanda:
    // By using a lock (which automatically released on sleep and retained on wake by cond_wait),
    // this race condition does not happen, since the code in the function thread_exit
    // won't get executed before we release the lock here, by going to sleep.
    Spin(6);
    printf("parent: waiting\n"); 
    Pthread_cond_wait(&c, &m);
  }
}

int main(int argc, char *argv[]) {
  printf("parent: begin\n");
  pthread_t t;
  Pthread_create(&t, NULL, child, NULL);
  thread_join();
  printf("parent: end\n");
  return 0;
}
