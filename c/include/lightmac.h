/**
 * Filename: lightmac.h
 * Author: Atul Luykx
 *
 * The files "lightmac.h" and "lightmac.c" contain an implementation
 * of the Message Authentication Code (MAC) mode of operation LightMAC
 * as described in https://eprint.iacr.org/2016/190 and presented at
 * Fast Software Encryption 2016 in Bochum, Germany. This file
 * includes the headers of the two functions which expose the
 * functionality of the MAC, namely tag and verify. However in order
 * to fully specify a MAC, the following parameters need to be fixed:
 * - the block cipher,
 * - the tag length, and
 * - the counter length.

 * Fixing the parameters involves implementing BCEncrypt, and defining
 * the following macros in a "parameters.h" file:
 * COUNTER_LENGTH -- the length of the counters in bytes
 * TAG_LENGTH -- the tag length in bytes
 * BLOCK_LENGTH -- the block cipher block length in bytes
 * BC_KEY_LENGTH -- the block cipher key length in bytes

 * The following equations translate between the symbols of the design
 * document (https://eprint.iacr.org/2016/190) and the parameters
 * described above:
 * s = 8*COUNTER_LENGTH
 * t = 8*TAG_LENGTH
 * n = 8*BLOCK_LENGTH
 * k = 8*BC_KEY_LENGTH
 * E = BCEncrypt
 **/

#ifndef LIGHTMAC_H
#define LIGHTMAC_H

#include <stdint.h>
#include <math.h>
#include <limits.h>
#include "parameters.h"

// LightMAC's key is twice that of the underlying block cipher
#define LIGHTMAC_KEY_LENGTH (2*BC_KEY_LENGTH)

/**
 * The maximum message length that LightMAC can process is a function
 * of the COUNTER_LENGTH. The following function gives the length in
 * bytes.
 **/
static inline unsigned long long maxMessageLength() {
  return (log2(BLOCK_LENGTH-COUNTER_LENGTH)+COUNTER_LENGTH*8) >= 64 ? ULLONG_MAX : (BLOCK_LENGTH-COUNTER_LENGTH)*(1ULL << (COUNTER_LENGTH*8));
}

/**
 * LightMAC tagging algorithm, which can be used as a PRF as well.
 *
 * The message has length messageLength bytes, the output has length
 * TAG_LENGTH bytes, and the key has length LIGHTMAC_KEY_LENGTH
 * bytes. It is assumed that messageLength < maxMessageLength().
 **/
void tag(const uint8_t* message, unsigned long long messageLength, uint8_t* output, const uint8_t* key);


/**
 * LightMAC verification algorithm. Returns one if the message-tag
 * pair is authentic, and zero otherwise.
 *
 * The message has length messageLength bytes, the tag has length
 * TAG_LENGTH bytes, and the key has length LIGHTMAC_KEY_LENGTH
 * bytes. It is assumed that messageLength < maxMessageLength().
 **/
int verify(const uint8_t* message, unsigned long long messageLength, const uint8_t* tag, const uint8_t* key);


/**
 * Function which computes the output of the block cipher with the
 * given key and input. This function needs to be implemented
 * elsewhere.
 **/
void BCEncrypt(const uint8_t input[BLOCK_LENGTH], uint8_t output[BLOCK_LENGTH], const uint8_t key[BC_KEY_LENGTH]);

#endif
