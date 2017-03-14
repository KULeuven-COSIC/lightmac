#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"
#include "lightmac.h"

#define LOOPS 64

static char *expectedOutputs[LOOPS] = {"c3c863d3e954788b", 
				       "021dff180cad82f9", 
				       "3f89441aa4492cb9", 
				       "858fffa6d1d238cc", 
				       "7aa56f920da21d54", 
				       "cead42e8f4022b71", 
				       "939bdb089f993715", 
				       "0870a1d98336d9fe", 
				       "cf7fdf180e2494a2", 
				       "957872ffd8474fcc", 
				       "ef35a618df40a363", 
				       "ea690e1ec15c8817", 
				       "fd0bc212a16867a4", 
				       "09bb68677241b04d", 
				       "28eb99831d08b850", 
				       "44cef839dcd7c1cb", 
				       "7f30f1f88d151ff5", 
				       "6af5747fed140aa8", 
				       "11274bd5394d388d", 
				       "99c3a296a8c8e548", 
				       "dc05c29ee8b9ab4e", 
				       "95e0f4fa3774a8cf", 
				       "102ab0337011a2f3", 
				       "88d68a1c48bbc0b7", 
				       "5f028d396326567a", 
				       "6f30b07a025fa7ef", 
				       "e5ecbaba994a36bc", 
				       "409b49af04f3c184", 
				       "dc88786e74c298ad", 
				       "76e6526af8c125ce", 
				       "942cfea168710b4c", 
				       "6f5703a4048145cb", 
				       "4734b059b872d41f", 
				       "6f8f32974cb44284", 
				       "12d537fe00bb6046", 
				       "cd00fa52a649e50a", 
				       "5a0cdf1f6e11f546", 
				       "36c79cbe956ed91a", 
				       "9c8545327b31c585", 
				       "a9a2eeed33cee786", 
				       "b3343218da6aa666", 
				       "4f34e65dba08b06a", 
				       "eebe3228f7f21ed4", 
				       "326d89cebad3f651", 
				       "ff99dbfc72b919d4", 
				       "0b7224abc148de6c", 
				       "97928b8df5c5f048", 
				       "9e6e039aa6209f07", 
				       "a10deb9041205b21", 
				       "0e8208be90e1e10b", 
				       "de0e24164f616f79", 
				       "1761603ea4fbbd80", 
				       "eda5d05fb67b528d", 
				       "48f4d47e485b47d2", 
				       "df07346bbcff6eac", 
				       "52d6b140588dd5ff", 
				       "cc680cb4b8b8a9b0", 
				       "100160dcfb743c20", 
				       "d2ee268ec27309d6", 
				       "dcdcba02e6b8a4b3", 
				       "100e1ae6ced7fede", 
				       "d394ff09dea2010c", 
				       "6e4b1fa630d3acc9", 
				       "431f7d967c0bc59e"};

int test(uint8_t *input, int inputLength, char expectedOutputStr[TAG_LENGTH*2], char keyStr[LIGHTMAC_KEY_LENGTH*2]) {
  uint8_t output[TAG_LENGTH];
  uint8_t expectedOutput[TAG_LENGTH];
  uint8_t key[LIGHTMAC_KEY_LENGTH];

  stringToUint8Array(expectedOutputStr, TAG_LENGTH*2, expectedOutput);
  stringToUint8Array(keyStr, LIGHTMAC_KEY_LENGTH*2, key);

  tag(input, inputLength, output, key);

  for(int i = 0; i < 8; i++) {
    if(output[i] != expectedOutput[i]) {
      for(int j = 0; j < TAG_LENGTH; j++) {
	printf("%02x", output[j]);
      }
      printf(" (Actual) \n");
      return 0;
    }
  }

  return 1;
}

int main(void) {
  uint8_t input[LOOPS];
  
  for(uint8_t i = 0; i < LOOPS; i++) {
    input[i] = i;
  }

  int result;
  for(int i = 0; i < LOOPS; i++) {
    result = test(input, i, expectedOutputs[i], "00112233445566778899aabbccddeeff833d3433009f389f2398e64f417acf39");

    if(!result) {
      printf("%s (Expected)\n", expectedOutputs[i]);
      printf("Test %i failed.\n\n", i);
    }

  }
  
}
