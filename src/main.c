#include <stdio.h>
#include <stdlib.h>

#include "md5.h"

void test(void) {
    putchar(10);
    unsigned long long len = 8;
    unsigned char input[8] = "12345678";
    printf("Message: ");
    for (unsigned int i = 0; i < len; ++i) {
        printf("%c", input[i]);
    }
    putchar(10);
    unsigned char* output = (unsigned char*)malloc(16 * sizeof(unsigned char));
    MD5(output, input, len);

    printf("MD5    : ");
    for (int i = 0; i < 16; ++i) {
        printf("%02x", output[i]);
    }
    putchar(10);

    free(output);
}

int main() {
    test();
    return 0;
}
