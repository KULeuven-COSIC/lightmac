#include <stdint.h>
#include "utils.h"
#include "present.h"

void BCEncrypt(const uint8_t input[8], uint8_t output[8], const uint8_t key[16]) {
  uint64_t key_high = 0;
  uint64_t key_low = 0;
  uint64_t *subkey;
  uint64_t result = 0;
  uint64_t bcinput = 0;
  
  key_high = uint8ArrayToUint64(key);
  key_low = uint8ArrayToUint64(key+8);
  bcinput = uint8ArrayToUint64(input);
  
  subkey = key_schedule(key_high, key_low, 32, 0, 0);
  result = encrypt(bcinput, subkey, 32, 0);

  uint64ToUint8Array(result, output);
}

