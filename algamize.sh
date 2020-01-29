#!/bin/bash
set -e -o nounset

DST_DIR="$1"
VERSION="$2"

mkdir -p "$DST_DIR"

SRC_DIR="src/"

NESTED_FILES=("c25519/f25519" "c25519/fprime" "c25519/sha512" "c25519/c25519" "c25519/ed25519" "c25519/edsign" )
COMPACT_FILES=(compact_wipe compact_x25519 compact_ed25519)


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

function merge_includes() {
    awk '
    /#include .*/ { includes[$0] = 1; next;}
    { other[NR] = $0; next; }
    END {
        for (i in includes) {
            print i;
        }
        for (i in other) {
            print other[i];
        }
    }
    '
}

function remove_double_blank_lines() {
    cat -s
}

function make_everything_static() {
    sed \
        -e 's/^\([^\ \t#{}()\/]\)/static \1/' \
        -e 's/static static/static/' \
        -e 's/static struct/struct/' \
        -e 's/static extern/extern/' \
        -e 's/static const/const/'
}

function add_decl_spec() {
    sed \
        -e 's/^static /static COMPACT_25519_DECL /' \
        -e 's/^\([^\ \t#{}()\/*s]\)/COMPACT_25519_DECL \1/' # non static stuff like global header
}

echo "// compact25519 $VERSION
// Source: https://github.com/DavyLandman/compact25519
// Licensed under CC0-1.0
// Based on Daniel Beer's Public Domain c25519 implementation
// https://www.dlbeer.co.nz/oss/c25519.html version: 2017-10-05

#ifndef __COMPACT_25519_H
#define __COMPACT_25519_H
#if defined(__cplusplus)
extern \"C\" {
#endif

// provide your own decl specificier like "-DCOMPACT_25519_DECL=ICACHE_RAM_ATTR"
#ifndef COMPACT_25519_DECL
#define COMPACT_25519_DECL
#endif
" > "$DST_HEADER"

for h in "${COMPACT_FILES[@]}"; do 
    cat "$SRC_DIR/$h.h" | remove_header_guard 
done | merge_includes | remove_double_blank_lines | add_decl_spec >> "$DST_HEADER" 

echo "#if defined(__cplusplus)
}
#endif
#endif" >> "$DST_HEADER"


echo "// compact25519 $VERSION
// Source: https://github.com/DavyLandman/compact25519
// Licensed under CC0-1.0
// Based on Daniel Beer's Public Domain c25519 implementation
// https://www.dlbeer.co.nz/oss/c25519.html version: 2017-10-05

#include \"compact25519.h\"
" > "$DST_SOURCE"

for h in "${NESTED_FILES[@]}"; do 
    echo "// ******* BEGIN: $h.h ********"
    cat "$SRC_DIR/$h.h" | remove_header_guard | \
        remove_local_imports | remove_double_blank_lines | \
        make_everything_static | add_decl_spec
    echo "// ******* END:   $h.h ********"
done >> "$DST_SOURCE"

for h in "${NESTED_FILES[@]}"; do 
    echo "// ******* BEGIN: $h.c ********"
    cat "$SRC_DIR/$h.c" | remove_local_imports | \
    remove_double_blank_lines | make_everything_static | add_decl_spec
    echo "// ******* END:   $h.c ********"
done >> "$DST_SOURCE"


for h in "${COMPACT_FILES[@]}"; do 
    echo "// ******* BEGIN: $h.c ********"
    cat "$SRC_DIR/$h.c" | remove_local_imports | \
    remove_double_blank_lines | add_decl_spec
    echo "// ******* END:   $h.c ********"
done >> "$DST_SOURCE"