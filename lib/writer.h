#ifndef WRITER_H
#define WRITER_H

#define MAX_VALUE_LEN 512

#define EXEC_MODE_RECORD 0
#define EXEC_MODE_REPLAY 1

struct logline {
    int id;
    int event_type;
    char value[MAX_VALUE_LEN];
};

extern int record(int event_t, char *val);
struct logline* replay(int event_t, int last_read_idx);

extern int deserialize_logline(const char *buf, struct logline *line);
extern int serialize_logline(struct logline *line, char *out);

#endif