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

int main(int argc, char *argv[]) {
    struct logline *l = malloc(sizeof(struct logline));
    int status = parse_logline(argv[1], l);
    if (status == 0) {
        printf("id..: %d\n", l->id);
        printf("event_type: %d\n", l->event_type);
        printf("value: %s\n", l->value);
    } else {
        printf("why");
        puts(json_error_string(status));
    }
    return status;
}

