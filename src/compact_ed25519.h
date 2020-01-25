#ifndef _COMPACT_ED25519_H
#define _COMPACT_ED25519_H

#ifndef COMPACT_DISABLE_ED25519
/*
Ed25519 signs messages using Curve25519.

After pre-sharing a public key, it allows two parties to verify the other side
is who they say they are and that the communication is not altered.

A signature is a ED25519_SIGNATURE_SIZE size byte array that can be only be 
generated when you have the private key, but can be verified if you have 
the public key. 

As an example: two parties want to have rotating keys for their encryption.
The only thing they need to know about eachother is their long term Ed25519
public key. Per session they perform a X25519 key exchange, but sign their 
"Ephemeral" public key with their long-term Ed25519 private key. Now they can 
share this public key (and the signature) over a cleartext channel, nobody can
alter of impersonate this exchange.

(this is roughly how TLS works)
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ED25519_SEED_SIZE (32)
#define ED25519_PUBLIC_KEY_SIZE (32)
#define ED25519_PRIVATE_KEY_SIZE (64)
#define ED25519_SIGNATURE_SIZE (64)

void compact_ed25519_keygen(
    uint8_t private_key[ED25519_PRIVATE_KEY_SIZE], 
    uint8_t random_seed[ED25519_SEED_SIZE]
);

void compact_ed25519_calc_public_key(
    uint8_t public_key[ED25519_PUBLIC_KEY_SIZE], 
    const uint8_t private_key[ED25519_PRIVATE_KEY_SIZE]
);

void compact_ed25519_sign(
    uint8_t signature[ED25519_SIGNATURE_SIZE], 
    const uint8_t private_key[ED25519_PRIVATE_KEY_SIZE], 
    const void *message, 
    size_t msg_length
);

bool compact_ed25519_verify(
    const uint8_t signature[ED25519_SIGNATURE_SIZE], 
    const uint8_t public_key[ED25519_PUBLIC_KEY_SIZE], 
    const void *message, 
    size_t msg_length
);
#endif

#endif
