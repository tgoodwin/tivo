#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "writer.h"

#define NUM_THREADS 4

static pthread_mutex_t test_idx_lock;

int test_deserialize_logline() {
  char *buf = "{\"id\": 1, \"event_type\": 1, \"value\": \"hello\"}";
  struct logline *l = malloc(sizeof(struct logline));
  int status = deserialize_logline(buf, l);
  int result = EXIT_SUCCESS;
  if (status != 0) {
    result = EXIT_FAILURE;
  }
  if (l->id != 1 || l->event_type != 1 || strcmp(l->value, "hello") != 0) {
    result = EXIT_FAILURE;
  }
  free(l);
  return result;
};

void *threadtask() {
  char *val = "test test test";
  int writer_id = 0;
  for (int i = 0; i < 10; i++) {
    record(0, 2, val);
  }
}

void *replaytask(void *arg) {
  // sad!
  struct rr_ctx *ctx = (struct rr_ctx *)arg;
  int writer_id = ctx->writer_id;
  int event_t = ctx->writer_id; // just use this for now
  int last_idx = 0;
  struct logline *l;
  do {
    l = replay(writer_id, event_t, last_idx);
    if (l != NULL) {
      fprintf(stdout, "event_t: %d, idx: %d, val: %s\n", event_t, l->id,
              l->value);
      last_idx = l->id;
    }
  } while (l != NULL);
  fprintf(stdout, "thread for event_t: %d is done\n", event_t);
};

int test_concurrent_record() {
  pthread_t tid[NUM_THREADS];

  if (pthread_mutex_init(&test_idx_lock, NULL) != 0) {
    puts("mutex init failed");
    return EXIT_FAILURE;
  }
  int error;
  for (int i = 0; i < NUM_THREADS; i++) {
    error = pthread_create(&(tid[i]), NULL, &threadtask, NULL);
    if (error != 0) {
      puts("failed to create threads");
      return error;
    }
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(tid[i], NULL);
  }
  pthread_mutex_destroy(&test_idx_lock);
  return EXIT_SUCCESS;
};

int test_concurrent_replay() {
  pthread_t tid[NUM_THREADS];
  int err;
  for (int i = 0; i < NUM_THREADS; i++) {
    int *arg = malloc(sizeof(*arg));
    struct rr_ctx *ctx = malloc(sizeof(struct rr_ctx));
    ctx->writer_id = i;
    err = pthread_create(&(tid[i]), NULL, &replaytask, ctx);
    if (err != 0) {
      puts("failed to create task");
      return err;
    }
  }
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(tid[i], NULL);
  }
  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
  int result = test_deserialize_logline();
  if (result != 0) {
    puts("test deserialize failed");
  }
  result = test_concurrent_record();
  if (result != 0) {
    printf("\ntest concurrent record failed: %d\n", result);
  }
  struct logline *out = replay(0, 2, 13);
  if (out != NULL) {
    fprintf(stdout, "%s\n", out->value);
  }
  result = test_concurrent_replay();
  if (result != 0) {
    puts("test concurrent replay failed");
  }
  puts("all done!");
}
