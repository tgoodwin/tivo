#include "writer.h"
#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mjson.h"

static int __log_idx_counter = 1;
static pthread_mutex_t idx_lock = PTHREAD_MUTEX_INITIALIZER;

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
      {"writer_id", t_integer, .addr.integer = &(line->writer_id)},
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

struct logline *replay_from_file(FILE *fp, int writer_id, int event_t,
                                 int last_read_idx) {
  size_t linecap = 0;
  ssize_t linelen;
  char *line = NULL;

  int curr_idx = 0;
  int status = 0;

  while ((linelen = getline(&line, &linecap, fp)) > 0) {
    curr_idx++;
    // skip over events we've seen already
    if (curr_idx <= last_read_idx) {
      continue;
    }

    struct logline *l = malloc(sizeof(struct logline));
    status = deserialize_logline(line, l);
    if (status == 0) {
      // skip over other event types or records written by someone else
      if (l->event_type != event_t || l->writer_id != writer_id) {
        free(l);
        continue;
      }
      // otherwise, we found what we want, return.
      return l;
    } else {
      puts(json_error_string(status));
    }
  }
  return NULL;
};

struct logline *replay(int writer_id, int event_t, int last_read_idx) {
  FILE *fp;
  size_t linecap = 0;
  ssize_t linelen;
  char *line = NULL;
  const char *fn = get_logfile();
  fp = fopen(fn, "r");
  if (fp == NULL) {
    exit(EXIT_FAILURE);
  }
  struct logline *found =
      replay_from_file(fp, writer_id, event_t, last_read_idx);
  fclose(fp);
  return found;
};

int record_to_file(FILE *fp, int writer_id, int event_t,
                   char *val) {
  struct logline *l = malloc(sizeof(struct logline));
  char out[MAX_VALUE_LEN];

  pthread_mutex_lock(&idx_lock);
  l->id = __log_idx_counter;
  l->writer_id = writer_id;
  l->event_type = event_t;
  strcpy(l->value, val);

  // render log line into buffer then flush it out to disk
  serialize_logline(l, out);
  fprintf(fp, "%s\n", out);
  __log_idx_counter++;
  pthread_mutex_unlock(&idx_lock);
  free(l);
  return 0;
};

int record(int writer_id, int event_t, char *val) {
  const char *fn = get_logfile();
  FILE *fp;
  fp = fopen(fn, "a");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  record_to_file(fp, writer_id, event_t, val);
  fclose(fp);
};
