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


static void fill_crappy_random(void* target, size_t length, pcg32_random_t* rng) {
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
static int testX25519(pcg32_random_t* rng) {
    printf("Testing X25519: ");
    uint8_t seed1[X25519_KEY_SIZE];
    uint8_t seed2[X25519_KEY_SIZE];
    fill_crappy_random(seed1, X25519_KEY_SIZE, rng);
    fill_crappy_random(seed2, X25519_KEY_SIZE, rng);

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

#ifndef COMPACT_DISABLE_X25519_DERIVE
    uint8_t derived[64];
    compact_x25519_derive_encryption_key(derived, sizeof(derived), shared1, pub1, pub2);
#endif

    if (memcmp(shared1, shared2, X25519_SHARED_SIZE) == 0) {
        printf("Success\n");
        return 0;
    }
    else {
        printf("Fail\n");
        return -1;
    }
}

int testX25519WithRfc7748Vectors(void) {
    printf("Testing X25519 using rfc7748 test vectors: ");
    int r = 0;
    uint8_t alice_sec[X25519_KEY_SIZE] = {
        0x77, 0x07, 0x6d, 0x0a, 0x73, 0x18, 0xa5, 0x7d,
        0x3c, 0x16, 0xc1, 0x72, 0x51, 0xb2, 0x66, 0x45,
        0xdf, 0x4c, 0x2f, 0x87, 0xeb, 0xc0, 0x99, 0x2a,
        0xb1, 0x77, 0xfb, 0xa5, 0x1d, 0xb9, 0x2c, 0x2a
    };

    uint8_t alice_pub[X25519_KEY_SIZE] = {
        0x85, 0x20, 0xf0, 0x09, 0x89, 0x30, 0xa7, 0x54,
        0x74, 0x8b, 0x7d, 0xdc, 0xb4, 0x3e, 0xf7, 0x5a,
        0x0d, 0xbf, 0x3a, 0x0d, 0x26, 0x38, 0x1a, 0xf4,
        0xeb, 0xa4, 0xa9, 0x8e, 0xaa, 0x9b, 0x4e, 0x6a
    };

    uint8_t bob_sec[X25519_KEY_SIZE] = {
        0x5d, 0xab, 0x08, 0x7e, 0x62, 0x4a, 0x8a, 0x4b,
        0x79, 0xe1, 0x7f, 0x8b, 0x83, 0x80, 0x0e, 0xe6,
        0x6f, 0x3b, 0xb1, 0x29, 0x26, 0x18, 0xb6, 0xfd,
        0x1c, 0x2f, 0x8b, 0x27, 0xff, 0x88, 0xe0, 0xeb
    };

    uint8_t bob_pub[X25519_KEY_SIZE] = {
        0xde, 0x9e, 0xdb, 0x7d, 0x7b, 0x7d, 0xc1, 0xb4,
        0xd3, 0x5b, 0x61, 0xc2, 0xec, 0xe4, 0x35, 0x37,
        0x3f, 0x83, 0x43, 0xc8, 0x5b, 0x78, 0x67, 0x4d,
        0xad, 0xfc, 0x7e, 0x14, 0x6f, 0x88, 0x2b, 0x4f
    };

    uint8_t expected_shared[X25519_KEY_SIZE] = {
        0x4a, 0x5d, 0x9d, 0x5b, 0xa4, 0xce, 0x2d, 0xe1,
        0x72, 0x8e, 0x3b, 0xf4, 0x80, 0x35, 0x0f, 0x25,
        0xe0, 0x7e, 0x21, 0xc9, 0x47, 0xd1, 0x9e, 0x33,
        0x76, 0xf0, 0x9b, 0x3c, 0x1e, 0x16, 0x17, 0x42
    };


    uint8_t shared1[X25519_SHARED_SIZE];
    uint8_t shared2[X25519_SHARED_SIZE];

    compact_x25519_shared(shared1, alice_sec, bob_pub);
    compact_x25519_shared(shared2, bob_sec, alice_pub);

    if (memcmp(shared1, shared2, X25519_SHARED_SIZE) != 0) {
        printf("Fail\n");
        r--;
    }

    if (memcmp(shared1, expected_shared, X25519_KEY_SIZE) != 0) {
        printf("Fail\n");
        r--;
    }
    if (r == 0) printf("Success\n");
    return r;
}
#endif

#ifndef COMPACT_DISABLE_ED25519
#define MSG_TEST_SIZE (ED25519_PRIVATE_KEY_SIZE * 2)
static int testEd25519(pcg32_random_t* rng) {
    printf("Testing Ed25519: ");
    uint8_t seed[ED25519_SEED_SIZE];
    fill_crappy_random(seed, ED25519_SEED_SIZE, rng);

    uint8_t sec[ED25519_PRIVATE_KEY_SIZE];
    uint8_t pub[ED25519_PUBLIC_KEY_SIZE];
    compact_ed25519_keygen(sec, pub, seed);

    uint8_t msg[MSG_TEST_SIZE];
    fill_crappy_random(msg, MSG_TEST_SIZE, rng);

    uint8_t sig[ED25519_SIGNATURE_SIZE];
    compact_ed25519_sign(sig, sec, msg, MSG_TEST_SIZE);

    if (!compact_ed25519_verify(sig, pub, msg, MSG_TEST_SIZE)) {
        printf("Failed normal signature\n");
        return -1;
    }

    msg[3] ^= 0x10; // flip a single bit
    if (compact_ed25519_verify(sig, pub, msg, MSG_TEST_SIZE)) {
        printf("Failed on changed data\n");
        return -1;
    }
    else {
        printf("Success\n");
    }
    return 0;
}
#endif

int main(void) {
    srand(time(NULL)); 
    pcg32_random_t rng;
    rng.state = rand();
    rng.inc = rand() | 1;

    int result = 0;
#ifndef COMPACT_DISABLE_X25519
    testX25519WithRfc7748Vectors();
    for (int i = 0; i < 5; i++) {
        result += testX25519(&rng);
    }
#endif

#ifndef COMPACT_DISABLE_ED25519
    for (int i = 0; i < 5; i++) {
        result += testEd25519(&rng);
    }
#endif
    return result;
}
