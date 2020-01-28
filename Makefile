.PHONY: clean test dist test-dist

src = $(wildcard src/*.c) $(wildcard src/c25519/*.c)

CFLAGS+=-std=gnu99 -pedantic -Wall -Wextra -Os -Isrc -Isrc/c25519
LDFLAGS = 

VERSION?=dev-version

bin/run-tests:  $(src) test/run-all.c
	mkdir -p bin/
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

test: bin/run-tests 
	./bin/run-tests

test-dist: test/algamize-test.go dist
	cd test && go run algamize-test.go


dist:
	bash algamize.sh dist/ "${VERSION}"
	cp README.md dist/

PACKAGE_DIST="compact25519-${VERSION}"

package-dist: dist
	rm -rf "${PACKAGE_DIST}"
	cp -r dist "${PACKAGE_DIST}"
	zip -r -9 -X dist.zip "${PACKAGE_DIST}"

clean:
	rm -f bin/*

unused: $(src) test/run-all.c
	$(CC) -o bin/unused $^ $(LDFLAGS) $(CFLAGS) -ffunction-sections -fdata-sections -Wl,--gc-sections,--print-gc-sections

test-windows: test-dist
	docker run --rm -v "/$(PWD):/app" silkeh/clang bash -c 'cd /app && CC=clang CFLAGS="-fsanitize=undefined" make clean test'

test-linux: test test-dist