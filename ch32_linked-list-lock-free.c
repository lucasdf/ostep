#include <common_threads.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct __node_t {
  int key;
  struct __node_t *next;
} node_t;

// global lock for whole list
typedef struct __list_t {
  node_t *head;
  pthread_mutex_t lock;
} list_t;

void List_Init(list_t *L) {
  L->head = NULL;
  pthread_mutex_init(&L->lock, NULL);
}

int List_Insert(list_t *L, int key) {
  node_t *new = malloc(sizeof(node_t));
  if (new == NULL) {
    perror("malloc failed");
    return -1;
  }
  new->key = key;

  do {
    new->next = L->head;
    // changes '&L->head' from old 'new->next' to new 'new'
    // if it fails (ie. head has been updated and it is not new->next),
    // then updates new->next with the new head and try again
  } while (__sync_bool_compare_and_swap(&L->head, new->next, new) == 0);
  return 0;
}

int List_Lookup(list_t *L, int key) {
  int rv = -1;
  pthread_mutex_lock(&L->lock);
  node_t *curr = L->head;
  while (curr) {
    if (curr->key == key) {
      rv = 0;
      break;
    }
    curr = curr->next;
  }
  pthread_mutex_unlock(&L->lock);
  return rv;
}

struct t_params {
  list_t *list;
  int identifier;
};

void *thread_insert(void *arg) {
  struct t_params *params = (struct t_params *)arg;
  for (int i = 0; i < 10; i++) {
    List_Insert(params->list, params->identifier + i);
  }
  return NULL;
}

int main() {
  pthread_t t1, t2, t3;
  list_t list;
  List_Init(&list);
  struct t_params t1_p;
  t1_p.list = &list;
  t1_p.identifier = 1000;
  pthread_create(&t1, NULL, thread_insert, &t1_p);
  struct t_params t2_p;
  t2_p.list = &list;
  t2_p.identifier = 2000;
  pthread_create(&t2, NULL, thread_insert, &t2_p);
  struct t_params t3_p;
  t3_p.list = &list;
  t3_p.identifier = 3000;
  pthread_create(&t3, NULL, thread_insert, &t3_p);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  for (int i = 0; i < 10; i++) {
    for (int j = 1000; j < 4000; j += 1000) {
      // all zero means all elements were added. if concurrency did not work,
      // there would be some -1
      printf("Element %d\n", List_Lookup(&list, j + i));
    }
  }
}
