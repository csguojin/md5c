#include <stdio.h>
#include <string.h>  // strcmp, strlen

#include "hmac_md5.h"

int main(int argc, char *argv[]) {
    unsigned char *key = NULL;
    unsigned char *input_string = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-k") == 0 || strcmp(argv[i], "-key") == 0) {
            if (i + 1 < argc) {
                key = (unsigned char *)argv[i + 1];
                i++;
            } else {
                printf("Missing key after -k\n");
                return 1;
            }
        } else {
            if (input_string == NULL) {
                input_string = (unsigned char *)argv[i];
            }
        }
    }

    if (key == NULL || input_string == NULL) {
        printf("Usage: %s <input_string> -k <key>\n", argv[0]);
        return 1;
    }

    unsigned int key_length = strlen((const char *)key);
    unsigned long long input_length = strlen((const char *)input_string);

    unsigned char HMAC_MD5_output[16];

    HMAC_MD5(HMAC_MD5_output, key, key_length, input_string, input_length);

    for (int i = 0; i < 16; i++) {
        printf("%02x", HMAC_MD5_output[i]);
    }
    putchar(10);

    return 0;
}
