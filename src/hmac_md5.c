#include "hmac_md5.h"

#include <stdlib.h>
#include <string.h>

#include "md5.h"

#define BLOCK_SIZE 512   // The block size of the hash function
#define OUTPUT_SIZE 128  // The output size of the hash function

void geneKPlus(unsigned char *KPlus, const unsigned char *key,
               unsigned int keylen) {
    if (keylen < BLOCK_SIZE / 8) {
        // Keys shorter than blockSize are padded to blockSize by padding with
        // zeros on the right
        memcpy(KPlus, key, keylen);
        unsigned int FillByte = BLOCK_SIZE / 8 - keylen;
        // Pad key with zeros to make it blockSize bytes long
        memset(&KPlus[keylen], 0, FillByte);
    } else {
        // Keys longer than blockSize are shortened by hashing them
        // key is outputSize bytes long
        MD5(KPlus, key, keylen);
        unsigned int FillByte = BLOCK_SIZE / 8 - (OUTPUT_SIZE / 8);
        memset(&KPlus[keylen], 0, FillByte);
    }
}
void geneSi(unsigned char *Si, unsigned char *KPlus) {
    memcpy(Si, KPlus, (BLOCK_SIZE / 8));
    for (int i = 0; i < BLOCK_SIZE / 8; ++i) {
        Si[i] = KPlus[i] ^ 0x36;  // Inner padded key 00110110
    }
}
void geneSo(unsigned char *So, unsigned char *KPlus) {
    memcpy(So, KPlus, (BLOCK_SIZE / 8));
    for (int i = 0; i < BLOCK_SIZE / 8; ++i) {
        So[i] = KPlus[i] ^ 0x5c;  // Outer padded key 01011100
    }
}

unsigned char *connectUnsignedCharString(unsigned char *dest,
                                         const unsigned char *str1,
                                         unsigned long long len1,
                                         const unsigned char *str2,
                                         unsigned long long len2) {
    memcpy(dest, str1, len1);
    memcpy(&dest[len1], str2, len2);
    return dest;
}

void HMAC_MD5(unsigned char *output, const unsigned char *key,
              unsigned int keylen, const unsigned char *str,
              unsigned long long len) {
    unsigned char *Kplus =
        (unsigned char *)malloc((BLOCK_SIZE / 8) * sizeof(unsigned char));
    geneKPlus(Kplus, key, keylen);

    unsigned char *Si =
        (unsigned char *)malloc((BLOCK_SIZE / 8 + len) * sizeof(unsigned char));
    geneSi(Si, Kplus);

    unsigned char *Si_M = Si;
    connectUnsignedCharString(Si_M, Si, (BLOCK_SIZE / 8), str, len);

    unsigned char *H_Si_M =
        (unsigned char *)malloc((OUTPUT_SIZE / 8) * sizeof(unsigned char));
    MD5(H_Si_M, Si_M, (BLOCK_SIZE / 8 + len));

    unsigned char *So = (unsigned char *)malloc(
        (BLOCK_SIZE / 8 + (OUTPUT_SIZE / 8)) * sizeof(unsigned char));
    geneSo(So, Kplus);

    unsigned char *So_H_Si_M = So;
    connectUnsignedCharString(So_H_Si_M, So, (BLOCK_SIZE / 8), H_Si_M,
                              (OUTPUT_SIZE / 8));

    MD5(output, So_H_Si_M, (BLOCK_SIZE / 8 + (OUTPUT_SIZE / 8)));

    free(So);
    free(H_Si_M);
    free(Si);
    free(Kplus);
    return;
}
