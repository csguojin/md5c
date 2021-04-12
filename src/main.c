#include <stdio.h>
#include <stdlib.h>

#include "hmac_md5.h"
#include "md5.h"

void test_md5(void) {
    putchar(10);
    unsigned long long len = 8;
    unsigned char input[8] = "12345678";
    printf("Message : ");
    for (unsigned int i = 0; i < len; ++i) {
        printf("%c", input[i]);
    }
    putchar(10);
    unsigned char* output = (unsigned char*)malloc(16 * sizeof(unsigned char));
    MD5(output, input, len);

    printf("MD5     : ");
    for (int i = 0; i < 16; ++i) {
        printf("%02x", output[i]);
    }
    putchar(10);

    free(output);
}

void test_hmac_md5(void) {
    putchar(10);
    unsigned int keylen = 3;
    unsigned char key[12] = "abc";
    unsigned long long len = 8;
    unsigned char input[8] = "12345678";

    printf("key       : ");
    for (unsigned int i = 0; i < keylen; ++i) {
        printf("%c", key[i]);
    }
    putchar(10);

    printf("message   : ");
    for (unsigned int i = 0; i < len; ++i) {
        printf("%c", input[i]);
    }
    putchar(10);

    unsigned char* output = (unsigned char*)malloc(16 * sizeof(unsigned char));
    HMAC_MD5(output, key, keylen, input, len);
    printf("HMAC_MD5  : ");
    for (int i = 0; i < 16; ++i) {
        printf("%02x", output[i]);
    }
    putchar(10);

    free(output);
}

int main() {
    test_md5();
    test_hmac_md5();
    return 0;
}
