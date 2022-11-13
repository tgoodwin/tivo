#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "writer.h"

pthread_mutex_t idx_lock;

int test_serialize(const char *buf) {
  struct logline *l = malloc(sizeof(struct logline));
  int status = deserialize_logline(buf, l);
  if (status == 0) {
    printf("id: %d\n", l->id);
    printf("event_type: %d\n", l->event_type);
    printf("value: %s\n", l->value);
  } else {
    puts(json_error_string(status));
  }
  // now serialize it back out
  char out[sizeof(struct logline)];
  serialize_logline(l, out);
  puts(out);
  free(l);
  return 0;
};

void *trythis() {
  char *val = "test test test";
  for (int i = 0; i < 10; i++) {
    record(2, val);
  }
}

int test_concurrent_record() {
  pthread_t tid[2];

  if (pthread_mutex_init(&idx_lock, NULL) != 0) {
    printf("\n mutex init failed");
    return EXIT_FAILURE;
  }
  int error;
  for (int i = 0; i < 2; i++) {
    error = pthread_create(&(tid[i]), NULL, &trythis, NULL);
  }
  if (error != 0) {
    printf("failed to creat threads");
  }
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_mutex_destroy(&idx_lock);
}