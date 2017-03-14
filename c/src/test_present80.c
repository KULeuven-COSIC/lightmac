#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"

void BCEncrypt(const uint8_t input[8], uint8_t output[8], const uint8_t key[10]);

int test(char inputStr[16], char expectedOutputStr[16], char keyStr[20]) {
  uint8_t output[8];
  uint8_t input[8];
  uint8_t expectedOutput[8];
  uint8_t key[10];

  stringToUint8Array(inputStr, 16, input);
  stringToUint8Array(expectedOutputStr, 16, expectedOutput);
  stringToUint8Array(keyStr, 20, key);

  BCEncrypt(input, output, key);

  for(int i = 0; i < 8; i++) {
    if(output[i] != expectedOutput[i]) {
      printf("%i, %c, %c\n", i, output[i], expectedOutput[i]);
      return 0;
    }
  }
  
  return 1;
}


int main(void) {
  printf("Test e80-k00_t00: %i\n", test("0000000000000000", "5579c1387b228445", "00000000000000000000"));
  printf("Test e80-k00_tff: %i\n", test("ffffffffffffffff", "a112ffc72f68417b", "00000000000000000000"));
  printf("Test e80-kff_t00: %i\n", test("0000000000000000", "e72c46c0f5945049", "ffffffffffffffffffff"));
  printf("Test e80-kff_tff: %i\n", test("ffffffffffffffff", "3333dcd3213210d2", "ffffffffffffffffffff"));
}
