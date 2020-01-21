#include "compact_x25519.h"
#include "c25519/c25519.h"
#include "erase_from_memory.h"

void compact_x25519_keygen(uint8_t private_key[X25519_KEY_SIZE], uint8_t public_key[X25519_KEY_SIZE], uint8_t random_seed[X25519_KEY_SIZE]) {
    memcpy(private_key, random_seed, X25519_KEY_SIZE);
    erase_from_memory(random_seed, X25519_KEY_SIZE, X25519_KEY_SIZE);
    c25519_prepare(private_key);
    c25519_smult(public_key, c25519_base_x, private_key);
}

void compact_x25519_shared(uint8_t shared_key[X25519_SHARED_SIZE], const uint8_t my_private_key[X25519_KEY_SIZE], const uint8_t their_public_key[X25519_KEY_SIZE]) {
    c25519_smult(shared_key, their_public_key, my_private_key);
}
