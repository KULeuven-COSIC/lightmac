#include <stdint.h>
#include "parameters.h"

void BCEncrypt(const uint8_t input[BLOCK_LENGTH], uint8_t output[BLOCK_LENGTH], const unsigned char key[BC_KEY_LENGTH]) {
  for(int i = 0; i < BLOCK_LENGTH; i++) {
    output[i] = input[i];
  }
}
