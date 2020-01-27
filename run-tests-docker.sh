#!/bin/bash
docker run --rm -v "/$(PWD):/app" silkeh/clang bash -c 'cd /app && CC=clang CFLAGS="-fsanitize=undefined" make clean test'
docker run --rm -v "/$(PWD):/app" golang:1.13 bash -c 'cd /app && make test-dist'