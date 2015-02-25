AR ?= ar
CC ?= gcc
PREFIX ?= /usr/local

CFLAGS = -O3 -std=c99 -Wall -Wextra -Ideps

SRCS = src/matrix.c

OBJS = $(SRCS:.c=.o)

all: build/libmatrix.a

install: all
	cp -f build/libmatrix.a $(PREFIX)/lib/libmatrix.a
	cp -f src/matrix.h $(PREFIX)/include/matrix.h

uninstall:
	rm -f $(PREFIX)/lib/libmatrix.a
	rm -f $(PREFIX)/include/matrix.h

build/libmatrix.a: $(OBJS)
	@mkdir -p build
	$(AR) rcs $@ $^

bin/test: test.o $(OBJS)
	@mkdir -p bin
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@

clean:
	rm -fr bin build *.o src/*.o

test: bin/test
	@./$<

benchmark: bin/benchmark
	@./$<

.PHONY: test clean install uninstall

