CFLAGS = -O

all: main test

main:
	clang writer.c writer.h mjson.c mjson.h

test:
	clang writer_test.c writer.c writer.h mjson.c mjson.h