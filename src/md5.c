#include "md5.h"

#include <limits.h>  // ULLONG_MAX
#include <stdio.h>
#include <stdlib.h>  // malloc, free
#include <string.h>  // memcpy, memset

// Initialize variables
static unsigned int A = 0x67452301;
static unsigned int B = 0xEFCDAB89;
static unsigned int C = 0x98BADCFE;
static unsigned int D = 0x10325476;

unsigned long long calcLenAfterPad(unsigned long long len);

/*
 * Padding the message
 * The padding works as follows:
 * first a single bit, 1, is appended to the end of the message.
 * This is followed by as many zeros as are required to bring the length of the
 * message up to 64 bits fewer than a multiple of 512. The remaining bits are
 * filled up with 64 bits representing the length of the original message
 */
void MD5Pad(unsigned char *buf, unsigned long long len_buf,
            const unsigned char *input, unsigned long long len_input);

/*
 * The processing of a message chunk consists of four similar stages, termed
 * rounds; each round is composed of 16 similar operations based on a non-linear
 * function F, modular addition, and left rotation.
 */
void MD5Process(unsigned int *stage, const unsigned int *chunk);

// Connect the four pieces of information in the stage in little-endian
void MD5Final(unsigned char *output, unsigned int *stage);

void MD5(unsigned char *output, const unsigned char *input,
         unsigned long long len_input) {
    if (len_input > ULLONG_MAX / 8) {
        printf("Error: The Length of input is too long!\n");
        return;
    }
    unsigned long long len_buf = calcLenAfterPad(len_input);
    unsigned char *buf =
        (unsigned char *)malloc(len_buf * sizeof(unsigned char));

    // Padding the message
    MD5Pad(buf, len_buf, input, len_input);

    unsigned int *stage = (unsigned int *)malloc(4 * sizeof(unsigned int));
    stage[0] = A;
    stage[1] = D;
    stage[2] = C;
    stage[3] = B;
    unsigned char *chunk = (unsigned char *)malloc(64 * sizeof(unsigned char));
    // Process the message in successive 512-bit chunks
    for (unsigned int i = 0; i < len_buf / 64; ++i) {
        // The input message is broken up into chunks of 512-bit blocks
        memcpy(chunk, &buf[i * 64], 64);
        MD5Process(stage, (unsigned int *)((void *)chunk));
    }

    MD5Final(output, stage);

    free(chunk);
    free(stage);
    free(buf);
    return;
}

int is_little_endian() {
    unsigned int n = 0x01;
    return (*(char *)&n);
}
void ULL2StringWithLittleEndian(unsigned char *dest, unsigned long long n) {
    memcpy(dest, &n, 8);
    if (!is_little_endian()) {
        for (int i = 0; i < 4; ++i) {
            unsigned char temp = dest[i];
            dest[i] = dest[7 - i];
            dest[7 - i] = temp;
        }
    }
}
void UI2StringWithLittleEndian(unsigned char *dest, unsigned int n) {
    memcpy(dest, &n, 4);
    if (!is_little_endian()) {
        for (int i = 0; i < 2; ++i) {
            unsigned char temp = dest[i];
            dest[i] = dest[3 - i];
            dest[3 - i] = temp;
        }
    }
}

unsigned int F(unsigned int X, unsigned int Y, unsigned int Z) {
    return (X & Y) | ((~X) & Z);
}
unsigned int G(unsigned int X, unsigned int Y, unsigned int Z) {
    return (X & Z) | (Y & (~Z));
}
unsigned int H(unsigned int X, unsigned int Y, unsigned int Z) {
    return X ^ Y ^ Z;
}
unsigned int I(unsigned int X, unsigned int Y, unsigned int Z) {
    return Y ^ (X | (~Z));
}
typedef unsigned int (*RoundFunctionType)(unsigned int X, unsigned int Y,
                                          unsigned int Z);
static RoundFunctionType RoundFunction[4] = {F, G, H, I};

// s specifies the per-round shift amounts
static unsigned short s_table[4][4] = {
    {7, 12, 17, 22}, {5, 9, 14, 20}, {4, 11, 16, 23}, {6, 10, 15, 21}};
static const unsigned short X_table[4][16] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
    {1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12},
    {5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2},
    {0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9}};

// Use binary integer part of the sines of integers (Radians) as constants
static const unsigned int T_table[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

unsigned long long calcLenAfterPad(unsigned long long len) {
    int FillSize = 448 - ((len * 8) % 512);
    if (FillSize <= 0) {
        FillSize += 512;
    }
    unsigned int FSbyte = (unsigned int)FillSize / 8;
    return (len + 8 + FSbyte);
}

void MD5Pad(unsigned char *buf, unsigned long long len_buf,
            const unsigned char *input, unsigned long long len_input) {
    // Pre-processing: padding with zeros
    memset(buf, 0, len_buf);
    memcpy(buf, input, len_input);
    // Pre-processing: adding a single 1 bit
    buf[len_input] = 0x80;

    unsigned long long lenBit = len_input * 8ULL;
    ULL2StringWithLittleEndian(&buf[len_buf - 8], lenBit);
}

// leftrotate function definition
void CLS(unsigned int *s, unsigned short cx) {
    if (cx > 32) cx %= 32;
    *s = (*s << cx) | (*s >> (32 - cx));
    return;
}

// Process the message in successive 512-bit chunks
void MD5Process(unsigned int *stage, const unsigned int *chunk) {
    unsigned int *p1 = NULL, *p2 = NULL, *p3 = NULL, *p4 = NULL, temp = 0;
    // Initialize hash value for this chunk:
    unsigned int buf[4];
    for (int i = 0; i < 4; ++i) {
        buf[i] = stage[i];
    }
    // Main loop:
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 16; ++j) {
            p1 = stage + ((0 + j) % 4);
            p2 = stage + ((3 + j) % 4);
            p3 = stage + ((2 + j) % 4);
            p4 = stage + ((1 + j) % 4);
            // Be wary of the below definitions of a,b,c,d
            // A+F(B,C,D)+M[i]+T[i+1]
            temp = *p1;
            temp += RoundFunction[i](*p2, *p3, *p4);
            temp += chunk[(X_table[i][j])];
            temp += T_table[i * 16 + j];
            CLS(&temp, s_table[i][j % 4]);
            *p1 = *p2 + temp;
        }
    }
    // Add this chunk's hash to result so far:
    for (int i = 0; i < 4; ++i) {
        stage[i] += buf[i];
    }
}

// Output is in little-endian
void MD5Final(unsigned char *output, unsigned int *stage) {
    UI2StringWithLittleEndian(&output[0], stage[0]);
    UI2StringWithLittleEndian(&output[4], stage[3]);
    UI2StringWithLittleEndian(&output[8], stage[2]);
    UI2StringWithLittleEndian(&output[12], stage[1]);
}