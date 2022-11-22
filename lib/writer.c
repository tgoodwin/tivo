#include "writer.h"
#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mjson.h"

static int __log_idx_counter;
static pthread_mutex_t idx_lock = PTHREAD_MUTEX_INITIALIZER;

char *LOGFILE_ENV_VAR = "RR_LOGFILE_NAME";
char *LOGFILE_NAME_DEFAULT = "rrlog.out";

char *get_logfile() {
    char *log_file_name = getenv(LOGFILE_ENV_VAR);
    if (log_file_name) {
        return log_file_name;
    }

    return LOGFILE_NAME_DEFAULT;
}

// read_logline reads an individual log line thats been
int deserialize_logline(const char *buf, struct logline *line) {
  const struct json_attr_t json_attrs[] = {
      {"id", t_integer, .addr.integer = &(line->id)},
      {"event_type", t_integer, .addr.integer = &(line->event_type)},
      {"value", t_string, .addr.string = line->value,
       .len = sizeof(line->value)},
      {NULL},
  };

  // parse the JSON string from the buffer
  return json_read_object(buf, json_attrs, NULL);
};

// serializes a logline struct into a character buffer
int serialize_logline(struct logline *line, char *out) {
  return sprintf(out, "{\"id\": %d, \"event_type\": %d, \"value\": \"%s\"}",
                 line->id, line->event_type, line->value);
};

struct logline *replay_from_file(FILE *fp, int event_t, int last_read_idx) {
  size_t linecap = 0;
  ssize_t linelen;
  char *line = NULL;

  int curr_idx = 0;
  int status = 0;
  struct logline *found;

  while ((linelen = getline(&line, &linecap, fp)) != -1) {
    curr_idx++;
    // skip over events we've seen already
    if (curr_idx <= last_read_idx)
      continue;

    struct logline *l = malloc(sizeof(struct logline));
    status = deserialize_logline(line, l);
    if (status == 0) {
      // skip over other event types
      if (l->event_type != event_t) {
        free(l);
        continue;
      }
      found = l;
    } else {
      puts(json_error_string(status));
    }
  }
  return found;
}

struct logline *replay(int event_t, int last_read_idx) {
  FILE *fp;
  size_t linecap = 0;
  ssize_t linelen;
  char *line = NULL;
  const char *fn = get_logfile();
  fp = fopen(fn, "r");
  if (fp == NULL) {
    exit(EXIT_FAILURE);
  }
  struct logline *found = replay_from_file(fp, event_t, last_read_idx);
  fclose(fp);
  return found;
};

int record_to_file(FILE *fp, int record_idx, int event_t, char *val) {
  struct logline *l = malloc(sizeof(struct logline));
  l->id = record_idx;
  l->event_type = event_t;
  strcpy(l->value, val);

  // render log line into buffer then flush it out to disk
  char out[MAX_VALUE_LEN];
  serialize_logline(l, out);
  free(l);
  return fprintf(fp, "%s\n", out);
}

int record(int event_t, char *val) {
  pthread_mutex_lock(&idx_lock);
  FILE *fp;
  const char *fn = get_logfile();
  fp = fopen(fn, "a");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  record_to_file(fp, __log_idx_counter, event_t, val);
  __log_idx_counter++;
  fclose(fp);
  pthread_mutex_unlock(&idx_lock);

  return 0;
};
