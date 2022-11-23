#ifndef WRITER_H
#define WRITER_H

#define MAX_VALUE_LEN 512

#define EXEC_MODE_RECORD 0
#define EXEC_MODE_REPLAY 1

struct logline {
    int id;                         // used as a serial index
    int writer_id;                  // modeling this as an int for now
    int event_type;                 // modeling this as an int for now
    char value[MAX_VALUE_LEN];      // a character sequence for now. generalize later if needed
};

/* thread-local information for replay */
struct rr_ctx {
    int writer_id;
    int offsets_by_type[256]; // idx: event_t, val: last read idx
};

extern int record(int writer_id, int event_t, char *val);
struct logline* replay(int writer_id, int event_t, int last_read_idx);

extern int deserialize_logline(const char *buf, struct logline *line);
extern int serialize_logline(struct logline *line, char *out);

#endif