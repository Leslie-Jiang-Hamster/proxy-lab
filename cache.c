#include "cache.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_OBJECT_NUMBER 1000
#define PADDING 5
#define DEBUG
#undef DEBUG

#ifdef DEBUG
static const int MAX_CACHE_SIZE = 5;
#else
static const int MAX_CACHE_SIZE = 1000000;
#endif

static const int MODULUS_PRIME = 997;
static const int SMALL_PRIME = 31;
static const int MAX_OBJECT_SIZE = 100000;
static const int INFINITY = 2e9;
static int object_number = 0;
static int cache_size = 0;
static int timestamp = 0;
static object_t objects[MAX_OBJECT_NUMBER + PADDING];

static int hash(const char *key) {
  int len = strlen(key);
  int hash = 0;
  for (int i = 0; i < len; i++) {
    hash = (hash * SMALL_PRIME + key[i]) % MODULUS_PRIME;
  }
  return hash;
}

static bool Cache_has(const char *key) {
  object_t *head = objects + hash(key);
  while (head != NULL && head->key != NULL) {
    if (strcmp(head->key, key) == 0) {
      return true;
    }
    head = head->next;
  }
  return false;
}

static object_t Object_init(char *key, char *val, int timestamp, object_t *next) {
  object_t new_object = {key, val, timestamp, next};
  return new_object;
}

static void Cache_add(const char *key, const char *val) {
  assert(key != NULL && val != NULL);
  assert(!Cache_has(key));

  object_t *head = objects + hash(key);
  object_t *tmp_next = head->next;
  head->next = calloc(1, sizeof(object_t));
  *(head->next) = Object_init(head->key, head->val, head->timestamp, tmp_next);
  *head = Object_init(strdup(key), strdup(val), timestamp++, head->next);

  int object_size = strlen(val);
  object_number++;
  cache_size += object_size;
}


static void Cache_delete(int index) {
  object_t *head = objects + index;

  assert(head->key != NULL);
  assert(head->val != NULL);

  free(head->key);
  int object_size = strlen(head->val);
  free(head->val);
  object_t *tmp_next = head->next;
  *head = *(tmp_next);
  free(tmp_next);
  object_number--;
  cache_size -= object_size;
  
  #ifdef DEBUG
  printf("Cache full, freed %d bytes, current size is %d\n", object_size, cache_size);
  #endif
}

static void Cache_evict() {
  int min_timestamp = INFINITY, min_timestamp_index = -1;
  for (int i = 0; i < MAX_OBJECT_NUMBER; i++) {
    if (objects[i].key == NULL) {
      continue;
    }
    if (objects[i].timestamp < min_timestamp) {
      min_timestamp = objects[i].timestamp;
      min_timestamp_index = i;
    }
  }

  assert(min_timestamp < INFINITY);
  assert(min_timestamp_index >= 0);

  Cache_delete(min_timestamp_index);
}

void Cache_init() {
  memset(objects, 0, sizeof(objects));
}

char *Cache_read(const char *key) {
  assert(Cache_has(key));

  object_t *head = objects + hash(key);
  while (head != NULL && head->key != NULL) {
    if (strcmp(head->key, key) == 0) {
      return strdup(head->val);
    }
    head = head->next;
  }

  // control never reaches here
  assert(false);
  return NULL;
}

void Cache_write(const char *key, const char *val) {
  assert(!Cache_has(key));

  int object_size = strlen(val);
  if (object_size > MAX_OBJECT_SIZE) {
    return;
  }
  while (object_number + 1 > MAX_OBJECT_NUMBER || cache_size + object_size > MAX_CACHE_SIZE) {
    Cache_evict();
  }
  Cache_add(key, val);
}