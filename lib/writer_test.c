#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "writer.h"

pthread_mutex_t test_idx_lock;

int test_deserialize_logline() {
  char *buf = "{\"id\": 1, \"event_type\": 1, \"value\"; \"hello\"}";
  struct logline *l = malloc(sizeof(struct logline));
  int status = deserialize_logline(buf, l);
  int result = EXIT_SUCCESS;
  if (status != 0)
    result = EXIT_FAILURE;
  if (l->id != 1 || l->event_type != 1 || strcmp(l->value, "hello") != 0) {
    result = EXIT_FAILURE;
  }
  free(l);
  return result;
};

void *trythis() {
  char *val = "test test test";
  for (int i = 0; i < 10; i++) {
    record(2, val);
  }
}

int test_concurrent_record() {
  pthread_t tid[2];

  if (pthread_mutex_init(&test_idx_lock, NULL) != 0) {
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
  pthread_mutex_destroy(&test_idx_lock);
}

int main(int argc, char *argv[]) {
  int result = test_deserialize_logline();
  if (result != 0) {
    puts("test deserialize failed");
  }
}