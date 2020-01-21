src = $(wildcard src/*.c) \
	$(wildcard src/c25519/*.c)

CFLAGS+=-std=gnu99 -pedantic -Wall -Wextra -O2 -march=native -Isrc -Isrc/c25519
LDFLAGS = 

test: bin/run-tests
	./bin/run-tests

bin/run-tests:  $(src) test/run-all.c
	mkdir -p bin/
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

.PHONY: clean test
clean:
	rm -f bin/*