#include "md5.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void md5AssertPrint(const unsigned char *got, const unsigned char *want,
                    short numb) {
    int wrongIdx = -1;
    for (int i = 0; i < 16; ++i) {
        if (got[i] != want[i]) {
            wrongIdx = i;
            break;
        }
    }

    if (wrongIdx != -1) {
        printf("MD5 test #%d failed!\n", numb);

        printf("Want : ");
        for (int i = 0; i < 16; ++i) {
            printf("%02x", want[i]);
        }
        putchar(10);

        printf("Got  : ");
        for (int i = 0; i < 16; ++i) {
            printf("%02x", got[i]);
        }
        putchar(10);

        assert(got[wrongIdx] == want[wrongIdx]);
        return;
    }
    printf("MD5 test #%d passed!\n", numb);
}

void test_MD5_1() {
    unsigned char got[16];
    const char *testStr = "12345678";
    const unsigned long long testLen = strlen(testStr);

    // 25d55ad283aa400af464c76d713c07ad
    unsigned char want[17] = {0x25, 0xd5, 0x5a, 0xd2, 0x83, 0xaa, 0x40, 0x0a,
                              0xf4, 0x64, 0xc7, 0x6d, 0x71, 0x3c, 0x07, 0xad};
    want[16] = 0x00;

    MD5(got, (const unsigned char *)testStr, testLen);

    md5AssertPrint(got, want, 1);
}

void test_MD5_2() {
    unsigned char got[16];
    const char *testStr = "";
    const unsigned long long testLen = strlen(testStr);

    // d41d8cd98f00b204e9800998ecf8427e
    unsigned char want[17] = {0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04,
                              0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e};
    want[16] = 0x00;

    MD5(got, (const unsigned char *)testStr, testLen);

    md5AssertPrint(got, want, 2);
}

int main() {
    test_MD5_1();
    test_MD5_2();
    return 0;
}
