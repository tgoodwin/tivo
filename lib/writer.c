#define _GNU_SOURCE
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<pthread.h>

#include "mjson.h"

static const int MAX_VALUE_LEN = 512;

int __log_idx_counter = 0;
pthread_mutex_t idx_lock;

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
    int status = sprintf(out,
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
    pthread_mutex_lock(&idx_lock);
    FILE* fp;
    fp = fopen("testlog2.txt", "a");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    struct logline *l = malloc(sizeof(struct logline));
    l->id = __log_idx_counter;
    l->event_type = event_t;
    strcpy(l->value, val);

    // render log line into buffer then flush it out to disk
    char out[MAX_VALUE_LEN];
    gen_logline(l, out);
    fprintf(fp, "%s\n", out);

    __log_idx_counter++;
    pthread_mutex_unlock(&idx_lock);
    return 0;
};

void* trythis() {
    char* val = "hey dude";
    for(int i = 0; i < 10; i++) {
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
    for(int i = 0; i < 2; i++) {
        error = pthread_create(&(tid[i]), NULL, &trythis, NULL);
    }
    if (error != 0) {
        printf("failed to creat threads");
    }
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&idx_lock);
}

int main(int argc, char *argv[]) {
    // FILE* fp;
    // size_t linecap = 0;
    // ssize_t linelen;
    // char *line = NULL;

    // fp = fopen("testlog.txt", "r");
    // if (fp == NULL) {
    //     exit(EXIT_FAILURE);
    // }

    // int status;
    // while ((linelen = getline(&line, &linecap, fp)) > 0) {
    //     struct logline *l = malloc(sizeof(struct logline));
    //     status = parse_logline(line, l);
    //     if (status == 0) {
    //         char out[sizeof(struct logline)];
    //         gen_logline(l, out);
    //         puts(out);
    //     } else {
    //         printf("whoops: %s", json_error_string(status));
    //     }
    //     free(l);
    // }

    // fclose(fp);

    // if (line)
    //     free(line);

    // struct logline *ll = replay(2, 0);
    // puts(ll->value);

    // char *val = "hello";
    // record(2, val);
    // record(2, val);
    // record(2, val);
    // record(2, val);
    // record(2, val);
    test_concurrent_record();

    exit(EXIT_SUCCESS);
}

