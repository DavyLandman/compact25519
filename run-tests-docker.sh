#!/bin/bash
docker run --rm -v "/$(PWD):/app" silkeh/clang bash -c 'cd /app && CC=clang CFLAGS="-fsanitize=undefined" make clean test'