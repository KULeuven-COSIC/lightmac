#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"
#include "lightmac.h"

#define NUMVECS 64

int main(void) {
  uint8_t key[LIGHTMAC_KEY_LENGTH];
  uint8_t message[NUMVECS];
  uint8_t output [TAG_LENGTH];
  char temp[4113];

  for(uint8_t i = 0; i < NUMVECS; i++) {
    message[i] = i;
  }

  uint8ArrayToString(message, NUMVECS, temp);
  printf("%s\n",temp);

  stringToUint8Array("00112233445566778899aabbccddeeff833d3433009f389f2398e64f417acf39", LIGHTMAC_KEY_LENGTH*2, key);

  for(int i = 0; i < NUMVECS; i++) {
    tag(message, i, output, key);
    uint8ArrayToString(output, TAG_LENGTH, temp);
    printf("%s\n",temp);
  }
}
