CFLAGS = -std=gnu11 -Iinclude -O2 -Wall -Wextra -Werror

BINS = $(patsubst src/%.c,%,$(wildcard src/*.c))
LIBS = $(wildcard lib/*.c)

all: $(BINS) tests

alfwrapper: src/alfwrapper.c $(LIBS)
	$(CC) $(CFLAGS) --output='$@' $+ -lbcc

clean:
	rm --force --recursive --verbose -- $(BINS)

tests: alfwrapper
	run-parts --regex '.*' --verbose 'tests'

.PHONY: all clean tests
