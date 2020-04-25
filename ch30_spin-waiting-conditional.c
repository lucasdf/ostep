#include "common.h"
#include "common_threads.h"
#include <pthread.h>
#include <stdio.h>

volatile int done = 0;

void *child(void *arg) {
  printf("child: started\n");
  Spin(20);
  done = 1;
  printf("child: end\n");
  return NULL;
}

int main(int argc, char *argv[]) {
  printf("parent: begin\n");
  pthread_t t;
  Pthread_create(&t, NULL, child, NULL);
  while (done == 0) {
    printf("parent: waiting for child\n");
    Spin(5);
  }
  printf("parent: end\n");
  return 0;
}
