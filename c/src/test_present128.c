#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"

void BCEncrypt(const uint8_t input[8], uint8_t output[8], const uint8_t key[10]);

int test(char inputStr[16], char expectedOutputStr[16], char keyStr[32]) {
  uint8_t output[8];
  uint8_t input[8];
  uint8_t expectedOutput[8];
  uint8_t key[10];

  stringToUint8Array(inputStr, 16, input);
  stringToUint8Array(expectedOutputStr, 16, expectedOutput);
  stringToUint8Array(keyStr, 32, key);

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
  printf("Test e128-k00_t00.txt: %i\n", test("0000000000000000", "96db702a2e6900af", "00000000000000000000000000000000"));
  printf("Test e128-k00_tff.txt: %i\n", test("ffffffffffffffff", "3c6019e5e5edd563", "00000000000000000000000000000000"));
  printf("Test e128-kff_t00.txt: %i\n", test("0000000000000000", "13238c710272a5d8", "ffffffffffffffffffffffffffffffff"));
  printf("Test e128-kff_tff.txt: %i\n", test("ffffffffffffffff", "628d9fbd4218e5b4", "ffffffffffffffffffffffffffffffff"));
}
