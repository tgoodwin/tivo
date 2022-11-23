#include "writer.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

bool __tivo_sync_bool_compare_and_swap_4(volatile int32_t *data,
                                         volatile int32_t expected,
                                         volatile int32_t desired) {
  bool use_replay = getenv(RR_MODE_ENVVAR) == EXEC_MODE_REPLAY;
  if (use_replay) {
    // TODO abstract away writer_id and last_read_idx
    struct logline *l = replay(0, 0, 0);
    char *res = l->value;
    if (strcmp(l->value, "false") == 0) {
      return 0;
    }
    return 1;
  }

  bool result = __sync_bool_compare_and_swap_4(data, expected, desired);
  char *res = "false";
  if (result)
    res = "true";

  // TODO abstract away writer_id and event_t
  record(0, 0, res);
  return result;
}

int main(int argc, char **argv) {
  int32_t data, expected, desired;
  int a = __sync_bool_compare_and_swap_4(&data, expected, desired);
}
