#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"
#include "lightmac.h"

#define NUMVECS 9

int main(void) {
  uint8_t key[LIGHTMAC_KEY_LENGTH];
  uint8_t message[2056];
  uint8_t output [TAG_LENGTH];
  char temp[4113];
  int messageLengths[NUMVECS] = {0, 257, 514, 771, 1028, 1285, 1542, 1799, 2056};

  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 256; j++) {
      message[2*j + i*512] = (uint8_t) i;
      message[2*j+1 + i*512] = (uint8_t) j;
    }
  }
  for(int j = 0; j < 4; j++) {
    message[2048+2*j] = (uint8_t) 4;
    message[2048+2*j+1] = (uint8_t) j;
  }

  uint8ArrayToString(message, 2056, temp);
  printf("%s\n",temp);

  stringToUint8Array("0123456789abcdeffedcba98765432109cf35e82f26719c4f91cf900cc2cbcc1", LIGHTMAC_KEY_LENGTH*2, key);

  for(int i = 0; i < NUMVECS; i++) {
    tag(message, messageLengths[i], output, key);
    uint8ArrayToString(output, TAG_LENGTH, temp);
    printf("%s\n",temp);
  }
}
