#define _GNU_SOURCE
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "mjson.h"

static const int MAX_VALUE_LEN = 512;

struct logline {
    int id;
    int event_type;
    char value[MAX_VALUE_LEN];
};

// read_logline reads an individual log line thats been
int parse_logline(const char* buf, struct logline *line) {
    const struct json_attr_t json_attrs[] = {
        {"id", t_integer, .addr.integer = &(line->id)},
        {"event_type", t_integer, .addr.integer = &(line->event_type)},
        {"value", t_string, .addr.string = line->value, .len = sizeof(line->value)},
        {NULL},
    };

    // parse the JSON string from the buffer
    return json_read_object(buf, json_attrs, NULL);
};

// serializes a logline struct into a character buffer
int gen_logline(struct logline *line, char* out) {
    // TODO handle global id thread safe incrementation logic here.
    return sprintf(out,
            "{\"id\": %d, \"event_type\": %d, \"value\": \"%s\"}",
            line->id, line->event_type, line->value);
};

int test_serialize(const char* buf) {
    struct logline *l = malloc(sizeof(struct logline));
    int status = parse_logline(buf, l);
    if (status == 0) {
        printf("id: %d\n", l->id);
        printf("event_type: %d\n", l->event_type);
        printf("value: %s\n", l->value);
    } else {
        puts(json_error_string(status));
    }
    // now serialize it back out
    char out[sizeof(struct logline)];
    gen_logline(l, out);
    puts(out);
    free(l);
    return 0;
};

struct logline* replay(int event_t, int last_read_idx) {
    FILE* fp;
    size_t linecap = 0;
    ssize_t linelen;
    char *line = NULL;

    fp = fopen("testlog.txt", "r");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    int curr_idx = 0;
    int status = 0;
    struct logline* result;

    struct logline *found;

    while ((linelen = getline(&line, &linecap, fp)) != -1) {
        curr_idx++;
        // skip over events we've seen already
        if (curr_idx <= last_read_idx)
            continue;

        struct logline *l = malloc(sizeof(struct logline));
        status = parse_logline(line, l);
        if (status == 0) {
            // skip over other event types
            if (l->event_type != event_t) {
                free(l);
                continue;
            }
            // new record we havent seen yet thats of the event type we care about
            found = l;
        } else {
            puts(json_error_string(status));
        }
    }
    return found;
};

int record(int event_t, char* val) {
    int id = 1;

    FILE* fp;
    fp = fopen("testlog.txt", "a");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    struct logline *l = malloc(sizeof(struct logline));
    l->id = id;
    l->event_type = event_t;
    strcpy(l->value, val);

    char out[MAX_VALUE_LEN];
    gen_logline(l, out);
    fprintf(fp, "%s\n", out);
    return 0;
};

int main(int argc, char *argv[]) {
    FILE* fp;
    size_t linecap = 0;
    ssize_t linelen;
    char *line = NULL;

    fp = fopen("testlog.txt", "r");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    int status;
    while ((linelen = getline(&line, &linecap, fp)) > 0) {
        struct logline *l = malloc(sizeof(struct logline));
        status = parse_logline(line, l);
        if (status == 0) {
            char out[sizeof(struct logline)];
            gen_logline(l, out);
            puts(out);
        } else {
            printf("whoops: %s", json_error_string(status));
        }
        free(l);
    }

    fclose(fp);

    if (line)
        free(line);

    struct logline *ll = replay(2, 0);
    puts(ll->value);

    char *val = "hello";
    record(2, val);

    exit(EXIT_SUCCESS);
}

