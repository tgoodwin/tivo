CFLAGS = -O

all: main test

main:
	clang writer.c writer.h mjson.c mjson.h

writer:
	clang writer.c writer.h

mjson:
	clang mjson.c mjson.h

test: writer.o mjson.o
	clang -o test writer_test.c writer.o mjson.o