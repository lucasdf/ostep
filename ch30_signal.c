#include <pthread.h>
#include "common_threads.h"
#include "common.h"
#include <stdio.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *thread_wait(void *arg){
  Pthread_mutex_lock(&lock);
  printf("thread %s went to sleep\n", (char *) arg);
  Pthread_cond_wait(&cond, &lock);
  printf("thread %s awake\n", (char *) arg);
  Pthread_mutex_unlock(&lock);
  return NULL;
}

void *thread_wakeup_other_threads(void *arg){
  Pthread_mutex_lock(&lock);
  printf("waking other threads using signal\n");
  Pthread_cond_signal(&cond);
  Pthread_mutex_unlock(&lock);
}

void using_signal(){
  pthread_t t1, t2, t3;
  Pthread_create(&t1, NULL, thread_wait, "1");
  Pthread_create(&t2, NULL, thread_wait, "2");
  Spin(4);
  Pthread_create(&t3, NULL, thread_wakeup_other_threads, NULL);
  Pthread_join(t1, NULL);
  Pthread_join(t2, NULL);
  Pthread_join(t3, NULL);
}

int main(int argc, char *argv[]){
  printf("Using signal - only one thread is awake\n");
  using_signal();
}
