#include "common.h"
#include <pthread.h>
#include <stdio.h>

#define N_FORKS 5
#define N_PHILOSOPHERS 5
enum Philosophers {
  Plato = 0,
  Aristotle = 1,
  Socrates = 2,
  Nietzsche = 3,
  Epicurus = 4
};
char *philosopher_name(enum Philosophers philosopher) {
  switch (philosopher) {
  case Plato:
    return "plato";
  case Aristotle:
    return "aristotle";
  case Socrates:
    return "socrates";
  case Nietzsche:
    return "nietzsche";
  case Epicurus:
    return "epicurus";
  }
}

enum ForkStatus { ForkTaken, ForkAvailable };
typedef struct __fork_ {
  pthread_mutex_t lock;
  enum ForkStatus status;
} fork;

fork forks[N_FORKS];
pthread_mutex_t lock;
/*pthread_cond_t  forks[N_FORKS];*/

void initialize_forks() {
  for (int i = 0; i < N_FORKS; i++) {
    forks[i].status = ForkAvailable;
    pthread_mutex_init(&forks[i].lock, NULL);
  }
}

/*
 *           f1 aristotle f2 socrates f3
 *     plato                             nietzsche
 *           f0 confucius f5 epicurus f4
 * */

void *eat(void *arg) {
  int *philosopher = (int *)arg;
  char *name = philosopher_name((enum Philosophers) * philosopher);
  for (int rounds = 0; rounds < 3; rounds++) {
    int left_fork = *philosopher;
    // fork to the right of Epicurus is number 0, not number 5
    int right_fork = *philosopher == N_PHILOSOPHERS - 1 ? 0 : *philosopher + 1;

    // Solves deadlock by changing how last philosopher takes forks
    if (*philosopher == Epicurus) {
      // Take right fork
      pthread_mutex_lock(&forks[right_fork].lock);
      printf("%s has taken the right fork number %d\n", name, right_fork);
      // Sleeps so it forces all philosophers to start taking their left forks
      if (rounds == 0)
        Spin(4);
      // Take left fork
      pthread_mutex_lock(&forks[left_fork].lock);
      printf("%s has taken the left fork number %d\n", name, left_fork);
    } else {
      // Take left fork
      pthread_mutex_lock(&forks[left_fork].lock);
      printf("%s has taken the left fork number %d\n", name, left_fork);
      // Sleeps so it forces all philosophers to start taking their left forks
      if (rounds == 0)
        Spin(4);
      // Take right fork
      pthread_mutex_lock(&forks[right_fork].lock);
      printf("%s has taken the right fork number %d\n", name, right_fork);
    }

    // Sleeps a second while eating
    Spin(1);

    printf("%s has finished eating %d times\n", name, rounds);
    pthread_mutex_unlock(&forks[left_fork].lock);
    printf("%s released the left fork number %d\n", name, left_fork);
    pthread_mutex_unlock(&forks[right_fork].lock);
    printf("%s has released the right fork number %d\n", name, right_fork);
  }
  return NULL;
}

// wait for threads to finish
void wait_for_threads(pthread_t threads[]) {
  for (int i = 0; i < N_PHILOSOPHERS; i++) {
    pthread_join(threads[i], NULL);
  }
}

int main(int argc, char *argv[]) {
  initialize_forks();
  pthread_t plato, aristotle, socrates, nietzsche, epicurus;
  int plato_number = 0;
  pthread_create(&plato, NULL, eat, &plato_number);
  int aristotle_number = 1;
  pthread_create(&aristotle, NULL, eat, &aristotle_number);
  int socrates_number = 2;
  pthread_create(&socrates, NULL, eat, &socrates_number);
  int nietzsche_number = 3;
  pthread_create(&nietzsche, NULL, eat, &nietzsche_number);
  int epicurus_number = 4;
  pthread_create(&epicurus, NULL, eat, &epicurus_number);

  pthread_t thread_pool[] = {plato, aristotle, socrates, nietzsche, epicurus};
  wait_for_threads(thread_pool);
  printf("All philosopher have eaten the food. Dinner is over!\n");
}
