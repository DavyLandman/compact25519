#!/bin/bash
SRC_DIR="src/"

NESTED_FILES=("c25519/f25519" "c25519/fprime" "c25519/sha512" "erase_from_memory" "c25519/c25519" "c25519/ed25519" "c25519/edsign" )
COMPACT_FILES=(compact_x25519 compact_ed25519)

DST_DIR="$1"
mkdir -p "$DST_DIR"
VERSION="$2"

DST_HEADER="$DST_DIR/compact25519.h"
DST_SOURCE="$DST_DIR/compact25519.c"

function remove_header_guard() {
    # we reverse lines so that it is easier to detect the last endif to drop
    tac | \
        awk '
        BEGIN{LAST_END_FOUND=0;} 
        /#endif/ && !LAST_END_FOUND { LAST_END_FOUND=1; next; } 
        /#.*_H_*$/ { next; }
        42
        ' | \
        tac
}

function remove_local_imports() {
    sed 's/#include ".*h"//'
}

echo "// compact25519 $VERSION
#ifndef __COMPACT_25519_H
#define __COMPACT_25519_H
" > "$DST_HEADER"

for h in "${COMPACT_FILES[@]}"; do 
    cat "$SRC_DIR/$h.h" | remove_header_guard >> "$DST_HEADER"
done

echo "#endif" >> "$DST_HEADER"


echo "// compact25519 $VERSION
#include \"compact25519.h\"
" > "$DST_SOURCE"

for h in "${NESTED_FILES[@]}"; do 
    echo "// ******* BEGIN: $h.h ********" >> "$DST_SOURCE"
    cat "$SRC_DIR/$h.h" | remove_header_guard | remove_local_imports >> "$DST_SOURCE"
    echo "// ******* END:   $h.h ********" >> "$DST_SOURCE"
done

for h in "${NESTED_FILES[@]}"; do 
    echo "// ******* BEGIN: $h.c ********" >> "$DST_SOURCE"
    cat "$SRC_DIR/$h.c" | remove_local_imports >> "$DST_SOURCE"
    echo "// ******* END:   $h.c ********" >> "$DST_SOURCE"
done

for h in "${COMPACT_FILES[@]}"; do 
    cat "$SRC_DIR/$h.c" | remove_local_imports >> "$DST_SOURCE"
done