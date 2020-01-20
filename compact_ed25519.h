#ifndef _COMPACT_ED25519_H
#define _COMPACT_ED25519_H

#include <stddef.h>
#include <stdint.h>

#define ED25519_SEED_SIZE (32)
#define ED25519_PUBLIC_KEY_SIZE (32)
#define ED25519_PRIVATE_KEY_SIZE (64)
#define ED25519_SIGNATURE_SIZE (64)

void compact_ed25519_keygen(uint8_t private_key[ED25519_PRIVATE_KEY_SIZE], uint8_t random_seed[ED25519_SEED_SIZE]);
void compact_ed25519_calc_public_key(const uint8_t private_key[ED25519_PRIVATE_KEY_SIZE], uint8_t public_key[ED25519_PUBLIC_KEY_SIZE]);
void compact_ed25519_sign(const uint8_t private_key[ED25519_PRIVATE_KEY_SIZE], const uint8_t *msg, size_t msg_length, uint8_t signature[ED25519_SIGNATURE_SIZE]);
bool compact_ed25519_verify(const uint8_t public_key[ED25519_PUBLIC_KEY_SIZE], const uint8_t *msg, size_t msg_length, const uint8_t signature[ED25519_SIGNATURE_SIZE])

#endif
