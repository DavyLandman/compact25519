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

/*
 Generate a private signing key from a random seed.
 
 input:
     - random_seed = random bytes that need to be filled from a good source of 
         random entropy, PLEASE research the options on your platform!
 
 output:
     - private_key = signing key than can be used for compact_ed25519_sign
     - public_key = key to share with other parties 
 
 (internally the format of the private key is the seed + public key, 
 this makes it compatible with keys coming from libsodium, golang, and 
 implentations that derive from SUPERCOP/ref10)
*/
void compact_ed25519_keygen(
    uint8_t private_key[ED25519_PRIVATE_KEY_SIZE], 
    uint8_t public_key[ED25519_PUBLIC_KEY_SIZE], 
    uint8_t random_seed[ED25519_SEED_SIZE]
);

/*
 Extract public key from private key
 
 input:
     - private_key = signing key generated with compact_ed25519_keygen
 
 output:
     - public_key = public key corresponding to this private key, share with
         interested parties
*/
void compact_ed25519_calc_public_key(
    uint8_t public_key[ED25519_PUBLIC_KEY_SIZE], 
    const uint8_t private_key[ED25519_PRIVATE_KEY_SIZE]
);

/*
 Sign a message with the private key.
 
 input:
     - private_key = signing key generated with compact_ed25519_keygen or from a
         compatible source
     - message = pointer to the data that needs to be signed
     - msg_length = how many bytes to read from the start of the message pointer
 output:
     - signature = the signature that proves you hold the private key and that
         nobody tried to change the message. (the second property only holds
         if the attacker cannot change the size of the message)
*/
void compact_ed25519_sign(
    uint8_t signature[ED25519_SIGNATURE_SIZE], 
    const uint8_t private_key[ED25519_PRIVATE_KEY_SIZE], 
    const void *message, 
    size_t msg_length
);

/*
 Verify a signature against a public key.
 
 input:
     - signature = which signature to verify
     - public_key = the public key that it should be verified against 
         (should not have been part of the message that contained the signature)
     - message = message to verify
     - msg_length = size of the message to verify
 
 returns:
     true if verified
*/
bool compact_ed25519_verify(
    const uint8_t signature[ED25519_SIGNATURE_SIZE], 
    const uint8_t public_key[ED25519_PUBLIC_KEY_SIZE], 
    const void *message, 
    size_t msg_length
);
#endif

#endif
