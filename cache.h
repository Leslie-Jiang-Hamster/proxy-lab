#include <stdbool.h>

struct object {
  char *key;
  char *val;
  int timestamp;
  struct object *next;
};
typedef struct object object_t;

void Cache_write(const char *key, const char *val);
char *Cache_read(const char *key);
void Cache_init();
bool Cache_has(const char *key);
