#ifndef COMPACT_DISABLE_ED25519
#include "../src/compact_ed25519.h"
#endif
#ifndef COMPACT_DISABLE_X25519
#include "../src/compact_x25519.h"
#endif
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "pcg_random.h"


static void fill_random(void* target, size_t length, pcg32_random_t* rng) {
    if (length >= sizeof(uint32_t)) {
        uint32_t *p = target;
        uint32_t *last = p + (length / sizeof(uint32_t));
        while (p < last) {
            *p++ = pcg32_random_r(rng);
        }
    }
    size_t tail_size = length % sizeof(uint32_t);
    if (tail_size != 0) {
        uint32_t tail = pcg32_random_r(rng);
        memcpy((((uint8_t*)target) + length) - tail_size, &tail, tail_size);
    }
}

#ifndef COMPACT_DISABLE_X25519
static void testX25519(pcg32_random_t* rng) {
    printf("Testing X25519: ");
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

    printf(memcmp(shared1, shared2, X25519_SHARED_SIZE) == 0 ? "Success\n" : "Fail\n");
}
#endif

#ifndef COMPACT_DISABLE_ED25519
#define MSG_TEST_SIZE (ED25519_PRIVATE_KEY_SIZE * 2)
static void testEd25519(pcg32_random_t* rng) {
    printf("Testing Ed25519: ");
    uint8_t seed[ED25519_SEED_SIZE];
    fill_random(seed, ED25519_SEED_SIZE, rng);

    uint8_t sec[ED25519_PRIVATE_KEY_SIZE];
    compact_ed25519_keygen(sec, seed);

    uint8_t msg[MSG_TEST_SIZE];
    fill_random(msg, MSG_TEST_SIZE, rng);

    uint8_t sig[ED25519_SIGNATURE_SIZE];
    compact_ed25519_sign(sig, sec, msg, MSG_TEST_SIZE);

    uint8_t pub[ED25519_PUBLIC_KEY_SIZE];
    compact_ed25519_calc_public_key(pub, sec);

    if (!compact_ed25519_verify(sig, pub, msg, MSG_TEST_SIZE)) {
        printf("Failed normal signature\n");
        return;
    }

    msg[3] ^= 0x10; // flip a single bit
    if (compact_ed25519_verify(sig, pub, msg, MSG_TEST_SIZE)) {
        printf("Failed on changed data\n");
    }
    else {
        printf("Success\n");
    }
}
#endif

int main(void) {
    srand(time(NULL)); 
    pcg32_random_t rng;
    rng.state = rand();
    rng.inc = rand() | 1;

#ifndef COMPACT_DISABLE_X25519
    for (int i = 0; i < 5; i++) {
        testX25519(&rng);
    }
#endif

#ifndef COMPACT_DISABLE_ED25519
    for (int i = 0; i < 5; i++) {
        testEd25519(&rng);
    }
#endif
}
