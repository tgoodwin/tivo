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

int gen_logline(struct logline *line, char* out) {
    return sprintf(out, "{\"id\": %d, \"event_type\": %d, \"value\": \"%s\"}",
            line->id, line->event_type, line->value);
};

int main(int argc, char *argv[]) {
    struct logline *l = malloc(sizeof(struct logline));
    int status = parse_logline(argv[1], l);
    /* if (status == 0) { */
    /*     printf("id: %d\n", l->id); */
    /*     printf("event_type: %d\n", l->event_type); */
    /*     printf("value: %s\n", l->value); */
    /* } else { */
    /*     puts(json_error_string(status)); */
    /* } */
    char out[sizeof(struct logline)];
    gen_logline(l, out);
    puts(out);

    struct logline *l2 = malloc(sizeof(struct logline));
    int status2 = parse_logline(out, l2);
    if (status2 == 0) {
        printf("id: %d\n", l2->id);
        printf("event_type: %d\n", l2->event_type);
        printf("value: %s\n", l2->value);
    } else {
        puts(json_error_string(status2));
    }
    l2->id = 2;
    char out2[sizeof(struct logline)];
    gen_logline(l2, out2);
    puts(out2);
}

