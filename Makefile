src = $(wildcard src/*.c) \
	$(wildcard src/c25519/*.c)

CFLAGS+=-std=gnu99 -pedantic -Wall -Wextra -Os -Isrc -Isrc/c25519
LDFLAGS = 

test: bin/run-tests
	./bin/run-tests

bin/run-tests:  $(src) test/run-all.c
	mkdir -p bin/
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

.PHONY: clean test
clean:
	rm -f bin/*

unused: $(src) test/run-all.c
	$(CC) -o bin/unused $^ $(LDFLAGS) $(CFLAGS) -ffunction-sections -fdata-sections -Wl,--gc-sections,--print-gc-sections