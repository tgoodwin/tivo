CFLAGS = -O

.PHONY: all
all: main test

main:
	clang writer.c mjson.c

writer:
	clang writer.c

mjson:
	clang mjson.c

test: writer.o mjson.o
	clang -o test writer_test.c writer.o mjson.o

.PHONY: clean
clean:
	rm -f *.o test rrlog.out
