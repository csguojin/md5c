#include <stdio.h>
#include <string.h>  // strlen

#include "md5.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_string>\n", argv[0]);
        return 1;
    }

    unsigned char *input_string = (unsigned char *)argv[1];
    unsigned long long input_length = strlen((const char *)input_string);

    unsigned char MD5_output[16];

    MD5(MD5_output, input_string, input_length);

    for (int i = 0; i < 16; i++) {
        printf("%02x", MD5_output[i]);
    }
    putchar(10);

    return 0;
}