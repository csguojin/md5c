#ifndef HMAC_MD5_H
#define HMAC_MD5_H

#include <stdio.h>

void HMAC_MD5(unsigned char *output, const unsigned char *key,
              unsigned int keylen, const unsigned char *str,
              unsigned long long len);

#endif  // HMAC_MD5_H
