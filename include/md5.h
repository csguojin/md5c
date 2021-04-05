#ifndef MD5_H
#define MD5_H

#include <stdio.h>

void MD5(unsigned char *output, const unsigned char *str,
         unsigned long long len);

#endif  // MD5_H