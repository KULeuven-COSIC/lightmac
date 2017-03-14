#include <stdio.h>
#include "lightmac.h"
#include "arcfour.h"

// These are basic tests which are independent of the underlying block cipher

#define L_BLOCK_LENGTH (BLOCK_LENGTH-COUNTER_LENGTH) 

void basicTests(const uint8_t* message, unsigned long long messageLength, const uint8_t* key, const char* testName) {
  uint8_t output[TAG_LENGTH];
  
  /**
   * Test 1: Tag a message and check to see if it verifies
   */
  tag(message,messageLength,output,key);
  if(verify(message,messageLength,output,key) != 1) {
    printf("%s: Test 1 failed: verification failed.\n", testName);
  }

  /**
   * Test 2: Check to see if verification fails when the wrong tag is passed.
   **/
  output[0] += 1;
  if(verify(message,messageLength,output,key) != 0) {
    printf("%s: Test 2 failed: verification succeeded\n", testName);
  }
}

int main(void) {
  uint8_t rc4State[256]; // RC4 state
  const uint8_t rc4Key[132] = "This is a key that I am typing in so that I have something to test with, and then I can start testing. I just need more characters.";

  uint8_t key[LIGHTMAC_KEY_LENGTH];

  uint8_t singleblock[L_BLOCK_LENGTH];
  unsigned int singleblockLength = L_BLOCK_LENGTH;

  uint8_t multiblock[4*L_BLOCK_LENGTH];
  unsigned int multiblockLength = 4*L_BLOCK_LENGTH;

  uint8_t empty[0] = {};

  unsigned int smallfractionalLength = L_BLOCK_LENGTH-1;
  uint8_t smallfractional[L_BLOCK_LENGTH-1];

  unsigned int largefractionalLength = 4*L_BLOCK_LENGTH-1;
  uint8_t largefractional[4*L_BLOCK_LENGTH-1];

  printf("Maximum message length: %llu bytes.\n", maxMessageLength());

  arcfour_key_setup(rc4State, rc4Key, 131);

  arcfour_generate_stream(rc4State, key, LIGHTMAC_KEY_LENGTH);
  arcfour_generate_stream(rc4State, singleblock, singleblockLength);
  basicTests(singleblock, singleblockLength, key, "Single block");

  arcfour_generate_stream(rc4State, key, LIGHTMAC_KEY_LENGTH);
  arcfour_generate_stream(rc4State, multiblock, multiblockLength);
  basicTests(multiblock, multiblockLength, key, "Multi block");

  arcfour_generate_stream(rc4State, key, LIGHTMAC_KEY_LENGTH);
  basicTests(empty, 0, key, "Empty string");

  arcfour_generate_stream(rc4State, key, LIGHTMAC_KEY_LENGTH);
  arcfour_generate_stream(rc4State, smallfractional, smallfractionalLength);
  basicTests(smallfractional, smallfractionalLength, key, "Small fractional message");

  arcfour_generate_stream(rc4State, key, LIGHTMAC_KEY_LENGTH);
  arcfour_generate_stream(rc4State, largefractional, largefractionalLength);
  basicTests(largefractional, largefractionalLength, key, "Large fractional message");

}
