#include "../src/compact_ed25519.h"
#include "../src/compact_x25519.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)

typedef struct { uint64_t state;  uint64_t inc; } pcg32_random_t;

static uint32_t pcg32_random_r(pcg32_random_t* rng)
{
    uint64_t oldstate = rng->state;
    // Advance internal state
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc|1);
    // Calculate output function (XSH RR), uses old state for max ILP
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static void fill_random(void* target, size_t length, pcg32_random_t* rng) {
    if (length >= sizeof(uint32_t)) {
        uint32_t *p = target;
        uint32_t *last = p + (length / sizeof(uint32_t));
        while (p <= last) {
            *p++ = pcg32_random_r(rng);
        }
    }
    size_t tail_size = length % sizeof(uint32_t);
    if (tail_size != 0) {
        uint32_t tail = pcg32_random_r(rng);
        memcpy((((uint8_t*)target) + length) - tail_size, &tail, tail_size);
    }
}

static void testx25519(pcg32_random_t* rng) {
    printf("Testing x25519: ");
    uint8_t seed1[X25519_KEY_SIZE];
    uint8_t seed2[X25519_KEY_SIZE];
    fill_random(seed1, X25519_KEY_SIZE, rng);
    fill_random(seed2, X25519_KEY_SIZE, rng);

    uint8_t sec1[X25519_KEY_SIZE];
    uint8_t pub1[X25519_KEY_SIZE];
    uint8_t sec2[X25519_KEY_SIZE];
    uint8_t pub2[X25519_KEY_SIZE];
    compact_x25519_keygen(sec1, pub1, seed1);
    compact_x25519_keygen(sec2, pub2, seed2);

    uint8_t shared1[X25519_SHARED_SIZE];
    uint8_t shared2[X25519_SHARED_SIZE];
    compact_x25519_shared(shared1, sec1, pub2);
    compact_x25519_shared(shared2, sec2, pub1);

    if (memcmp(shared1, shared2, X25519_SHARED_SIZE) == 0) {
        printf("Success\n");
    }
    else {
        printf("Failed\n");
    }
}

int main(void) {
    srand(time(NULL)); 
    pcg32_random_t rng;
    rng.state = rand();
    rng.inc = rand() | 1;

    for (int i = 0; i < 5; i++) {
        testx25519(&rng);
    }

}
