# LightMAC C Implementations

These folders contain implementations of the following variants of the LightMAC family
- PRESENT128, with 64 bit tag length and 32 bit counter length
- PRESENT128, with 64 bit tag length and 8 bit counter length
- PRESENT80, with 64 bit tag length and 32 bit counter length
- PRESENT80, with 64 bit tag length and 8 bit counter length
- the identity function operating on 128 bit blocks, with 128 bit tag length and 64 bit counter length

Included are an implementation of RC4 by Brad Conte which is only used to generate random strings for testing purposes, and an implementation of PRESENT in C by Thomas Siebert.

Example compilation using gcc from the subfolder id_lightmac_64_128:
```SHELL
gcc ../src/arcfour.c ../src/lightmac.c ../src/test_lightmac.c ../src/identity.c -I../include/ -I./
```
