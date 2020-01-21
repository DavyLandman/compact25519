#ifndef _COMPACT_X25519_H
#define _COMPACT_X25519_H

#include <stdint.h>

#define X25519_KEY_SIZE (32)
#define X25519_SHARED_SIZE (32)

void compact_x25519_keygen(uint8_t private_key[X25519_KEY_SIZE], uint8_t public_key[X25519_KEY_SIZE], uint8_t random_seed[X25519_KEY_SIZE]);
void compact_x25519_shared(uint8_t shared_key[X25519_SHARED_SIZE], const uint8_t my_private_key[X25519_KEY_SIZE], const uint8_t their_public_key[X25519_KEY_SIZE]);

#endif
