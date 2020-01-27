src = $(wildcard src/*.c) $(wildcard src/c25519/*.c)
.PHONY: clean test dist

CFLAGS+=-std=gnu99 -pedantic -Wall -Wextra -Os -Isrc -Isrc/c25519
LDFLAGS = 

VERSION?=dev-version

test: bin/run-tests 
	./bin/run-tests

test-dist: test/algamize-test.go dist
	cd test && go run algamize-test.go

bin/run-tests:  $(src) test/run-all.c
	mkdir -p bin/
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

dist:
	bash algamize.sh dist/ "${VERSION}"

clean:
	rm -f bin/*

unused: $(src) test/run-all.c
	$(CC) -o bin/unused $^ $(LDFLAGS) $(CFLAGS) -ffunction-sections -fdata-sections -Wl,--gc-sections,--print-gc-sections