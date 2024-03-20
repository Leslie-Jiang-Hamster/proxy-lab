#include "csapp.h"
#include "pool.h"
#include <string.h>

#define DEBUG
#undef DEBUG

static int pool[MAX_SIZE + PADDING];
static sem_t mutex, size, rest;
static int size_;

#if defined(DEBUG)
static void debug() {
  printf("The pool is currently of size %d\n", size_);
  printf("It includes:");
  for (int i = 0; i < size_; i++) {
    printf(" %d", pool[i]);
  }
  puts("");
}
#endif

void Pool_init() {
  memset(pool, 0, sizeof(pool));
  size_ = 0;
  Sem_init(&mutex, 0, 1);
  Sem_init(&size, 0, 0);
  Sem_init(&rest, 0, MAX_SIZE);
}

void Pool_push(int fd) {
  P(&rest);
  P(&mutex);
  pool[size_++] = fd;
  V(&mutex);
  V(&size);
  #if defined(DEBUG)
  debug();
  #endif
}

int Pool_pop() {
  P(&size);
  P(&mutex);
  int fd = pool[--size_];
  V(&mutex);
  V(&rest);
  #if defined(DEBUG)
  debug();
  #endif
  return fd;
}