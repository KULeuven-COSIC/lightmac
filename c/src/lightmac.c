/**
 * Filename: lightmac.c
 * Author: Atul Luykx
 *
 * Big endian implementation of the LightMAC mode of operation.
 **/

#include "lightmac.h"

void encodeCounter(unsigned long long counter, uint8_t* output) {
  for(int i = COUNTER_LENGTH-1; i>=0; i--) {
    output[i] = counter;
    counter >>= 8;
  }
}

void tag(const uint8_t* message, unsigned long long messageLength, uint8_t* output, const uint8_t* key) {
  // Intermediate values used to store computations
  uint8_t value[BLOCK_LENGTH];
  uint8_t blockInput[BLOCK_LENGTH];
  uint8_t blockOutput[BLOCK_LENGTH];
  
  unsigned long long counter;
  unsigned int i;

  for(i = 0; i < BLOCK_LENGTH; i++) {
    value[i] = 0;
  }

  // Note: the counter starts at 1, not 0.
  counter = 1;
  
  // We stop the moment we are left with a message of length less than
  // BLOCK_LENGTH-COUNTER_LENGTH, after which padding occurs.
  while(messageLength >= BLOCK_LENGTH-COUNTER_LENGTH) {

    encodeCounter(counter, blockInput);

    // Appending BLOCK_LENGTH-COUNTER_LENGTH bytes of the message to
    // the counter to form a byte string of length BLOCK_LENGTH.
    for(i = 0; i < BLOCK_LENGTH-COUNTER_LENGTH; i++) {
      blockInput[i+COUNTER_LENGTH] = message[i];
    }

    BCEncrypt(blockInput, blockOutput, key);

    // XORing the block cipher output to the previously XORed block
    // cipher outputs.
    for(i = 0; i < BLOCK_LENGTH; i++) {
      value[i] ^= blockOutput[i];
    }
    messageLength -= BLOCK_LENGTH-COUNTER_LENGTH;
    message += BLOCK_LENGTH-COUNTER_LENGTH;
    counter++;
  }

  // Copying the remaining part of the message, and then applying
  // padding.
  for(i = 0; i < messageLength; i++) {
    blockInput[i] = message[i];
  }
  // Padding step 1: appending a '1'
  blockInput[messageLength] = 0x80;
  // Padding step 2: append as many zeros as necessary to complete the
  // block.
  for(i = messageLength+1; i < BLOCK_LENGTH; i++) {
    blockInput[i] = 0x00;
  }

  // Xoring the final block with the sum of the previous block cipher
  // outputs
  for(i = 0; i < BLOCK_LENGTH; i++) {
    value[i] ^= blockInput[i];
  }

  // Using the second part of the key for the final block cipher call.
  key += BC_KEY_LENGTH;
  BCEncrypt(value, blockOutput, key);

  // Truncation is performed to the most significant bits. We assume big endian encoding.
  for(i = 0; i < TAG_LENGTH; i++) {
    output[i] = blockOutput[i];
  }
}


int verify(const uint8_t* message, unsigned long long messageLength, const uint8_t* candidateTag, const uint8_t* key) {
  uint8_t tempTag[TAG_LENGTH];
  int i;

  // Compute the tag of the message.
  tag(message, messageLength, tempTag, key);

  // Compare the candidate tag with the computed tag. If they are the
  // same then verification succeeds, otherwise it fails.
  for(i = 0; i < TAG_LENGTH; i++) {
    if(candidateTag[i] != tempTag[i]) {
      return 0;
    }
  }
  return 1;
}
