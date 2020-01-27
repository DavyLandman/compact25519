#include "compact_ed25519.h"

#ifndef COMPACT_DISABLE_ED25519
#include "c25519/edsign.h"
#include "compact_wipe.h"
#include <string.h>

#define __PUBLIC_KEY_OFFSET (32)

void compact_ed25519_keygen(
    uint8_t private_key[ED25519_PRIVATE_KEY_SIZE], 
    uint8_t public_key[ED25519_PUBLIC_KEY_SIZE], 
    uint8_t random_seed[ED25519_SEED_SIZE]
) {
    // private key is seed + public key, like golang and others
    edsign_sec_to_pub(public_key, random_seed);
    memcpy(private_key, random_seed, ED25519_SEED_SIZE);
    memcpy(private_key + __PUBLIC_KEY_OFFSET, public_key, ED25519_PUBLIC_KEY_SIZE);
    compact_wipe(random_seed, ED25519_SEED_SIZE);
}

void compact_ed25519_calc_public_key(
    uint8_t public_key[ED25519_PUBLIC_KEY_SIZE], 
    const uint8_t private_key[ED25519_PRIVATE_KEY_SIZE]
) {
    memcpy(public_key, private_key + __PUBLIC_KEY_OFFSET, ED25519_PUBLIC_KEY_SIZE);
}

void compact_ed25519_sign(
    uint8_t signature[ED25519_SIGNATURE_SIZE], 
    const uint8_t private_key[ED25519_PRIVATE_KEY_SIZE], 
    const void *message, 
    size_t msg_length
) {
    edsign_sign(signature, private_key + __PUBLIC_KEY_OFFSET, private_key, message, msg_length);
}

bool compact_ed25519_verify(
    const uint8_t signature[ED25519_SIGNATURE_SIZE], 
    const uint8_t public_key[ED25519_PUBLIC_KEY_SIZE], 
    const void *message, 
    size_t msg_length
) {
    return edsign_verify(signature, public_key, message, msg_length) != 0;
}
#endif
