#ifndef _COMPACT_X25519_H
#define _COMPACT_X25519_H

#ifndef COMPACT_DISABLE_X25519
#include <stddef.h>
#include <stdint.h>

#define X25519_KEY_SIZE (32)
#define X25519_SHARED_SIZE (32)

void compact_x25519_keygen(
    uint8_t private_key[X25519_KEY_SIZE], 
    uint8_t public_key[X25519_KEY_SIZE], 
    uint8_t random_seed[X25519_KEY_SIZE]
);

void compact_x25519_shared(
    uint8_t shared_key[X25519_SHARED_SIZE], 
    const uint8_t my_private_key[X25519_KEY_SIZE], 
    const uint8_t their_public_key[X25519_KEY_SIZE]
);

#ifndef COMPACT_DISABLE_X25519_DERIVE
void compact_x25519_derive_encryption_key(
    uint8_t *encryption_key, 
    size_t key_size, 
    const uint8_t shared_key[X25519_SHARED_SIZE], 
    const uint8_t public_key1[X25519_KEY_SIZE], 
    const uint8_t public_key2[X25519_KEY_SIZE]
);
#endif
#endif
#endif
