#define MAX_VALUE_LEN 512

#define EXEC_MODE_RECORD 0
#define EXEC_MODE_REPLAY 1

struct logline {
    int id;
    int event_type;
    char value[MAX_VALUE_LEN];
};

int record(int event_t, char *val);
struct logline* replay(int event_t, int last_read_idx);

int deserialize_logline(const char *buf, struct logline *line);
int serialize_logline(struct logline *line, char *out);
