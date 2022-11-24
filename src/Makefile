CC = clang-14
CFLAGS = -flegacy-pass-manager -Xclang -load -Xclang ../build/tivo-pass/libTivoPass.so -O

.PHONY: all
all: main test

main:
	clang writer.c mjson.c

writer:
	clang -pthread writer.c

mjson:
	clang mjson.c

test: writer.o mjson.o
	clang -pthread -o test writer_test.c writer.o mjson.o

cas: writer.o mjson.o
	clang -pthread -o cas_test cas.c writer.o mjson.o

.PHONY: clean
clean:
	rm -f *.o test cas_test rrlog.out
